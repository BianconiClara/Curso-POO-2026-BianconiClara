#include "networkservice.h"
#include "logger.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkProxy>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QNetworkConfigurationManager>
#endif
#include <QStringList>
#include <QTimeZone>
#include <QUrlQuery>

namespace
{
bool isMissingApiKey(const QString &apiKey)
{
    return apiKey.trimmed().isEmpty() || apiKey.contains("REEMPLAZAR") || apiKey.contains("TU_API_KEY");
}

QString cleanCityName(const QString &city)
{
    const QStringList parts = city.split(',', Qt::SkipEmptyParts);
    return parts.isEmpty() ? QString("Cordoba") : parts.first().trimmed();
}

QString capitalizeText(const QString &text)
{
    if (text.isEmpty()) {
        return QString();
    }

    QString capitalized = text;
    capitalized[0] = capitalized[0].toUpper();
    return capitalized;
}

bool useImperialUnits(const AppConfig &config)
{
    return config.units.compare("imperial", Qt::CaseInsensitive) == 0;
}

QString temperatureFieldName(const AppConfig &config)
{
    return useImperialUnits(config) ? QString("temp_f") : QString("temp_c");
}

QString temperatureUnitLabel(const AppConfig &config)
{
    return useImperialUnits(config) ? QString("F") : QString("C");
}
}

NetworkService::NetworkService(const AppConfig &config, QObject *parent)
    : QObject(parent)
    , m_config(config)
{
    applyProxy();
}

void NetworkService::requestWeather()
{
    Logger::instance().log(QString("Solicitando clima para %1.").arg(m_config.city));

    if (isMissingApiKey(m_config.apiKey)) {
        emitOfflineWeather("No se encontro una API key valida. Se muestran datos simulados.");
        return;
    }

    if (!isNetworkAvailable()) {
        emitOfflineWeather("No hay internet disponible. Se activo el modo offline.");
        return;
    }

    QUrl url(m_config.apiBaseUrl);
    QUrlQuery query;
    query.addQueryItem("key", m_config.apiKey);
    query.addQueryItem("q", m_config.city);
    query.addQueryItem("lang", m_config.language);
    query.addQueryItem("aqi", "no");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "UBP-Ejercicio7-Qt");

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, this, &NetworkService::onWeatherReply);
}

void NetworkService::requestBackground()
{
    if (m_backgroundPrepared && QFileInfo::exists(m_backgroundPath)) {
        emit backgroundReady(m_backgroundPath, false);
        return;
    }

    const QString cachedPath = AppConfigLoader::backgroundCachePath();
    if (QFileInfo::exists(cachedPath)) {
        m_backgroundPrepared = true;
        m_backgroundPath = cachedPath;
        Logger::instance().log(QString("Usando imagen de fondo desde cache: %1").arg(cachedPath));
        emit backgroundReady(cachedPath, false);
        return;
    }

    if (!isNetworkAvailable()) {
        useFallbackBackground("Sin internet no se pudo descargar el fondo. Se usa la imagen local.");
        return;
    }

    Logger::instance().log(QString("Descargando imagen de fondo desde %1").arg(m_config.backgroundUrl));

    QNetworkRequest request{QUrl(m_config.backgroundUrl)};
    request.setHeader(QNetworkRequest::UserAgentHeader, "UBP-Ejercicio7-Qt");

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, this, &NetworkService::onBackgroundReply);
}

void NetworkService::onWeatherReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }

    const QByteArray payload = reply->readAll();
    const QString errorString = reply->errorString();
    const bool success = reply->error() == QNetworkReply::NoError;
    reply->deleteLater();

    if (!success) {
        Logger::instance().log(QString("Error de red al consultar clima: %1").arg(errorString));
        emitOfflineWeather("No pudimos consultar el clima real. Se muestran datos simulados.");
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        Logger::instance().log("La respuesta del clima no pudo parsearse correctamente.");
        emitOfflineWeather("La API devolvio datos invalidos. Se activo el modo offline.");
        return;
    }

    const QJsonObject root = document.object();
    const QJsonObject locationObject = root.value("location").toObject();
    const QJsonObject currentObject = root.value("current").toObject();
    const QJsonObject conditionObject = currentObject.value("condition").toObject();
    const QString tempField = temperatureFieldName(m_config);

    if (locationObject.isEmpty()
        || currentObject.isEmpty()
        || !currentObject.value(tempField).isDouble()
        || conditionObject.value("text").toString().trimmed().isEmpty()) {
        Logger::instance().log("La respuesta del clima llego incompleta.");
        emitOfflineWeather("La API devolvio datos incompletos. Se activo el modo offline.");
        return;
    }

    WeatherInfo info;
    info.valid = true;
    info.offline = false;
    info.city = locationObject.value("name").toString(cleanCityName(m_config.city));

    const QString country = locationObject.value("country").toString().trimmed();
    if (!country.isEmpty()) {
        info.city += ", " + country;
    }

    info.description = capitalizeText(conditionObject.value("text").toString("clima actual"));
    info.temperature = currentObject.value(tempField).toDouble();
    info.temperatureUnit = temperatureUnitLabel(m_config);
    info.fetchedAtUtc = QDateTime::currentDateTimeUtc();
    info.sourceLabel = "WeatherAPI.com";

    const QString timeZoneId = locationObject.value("tz_id").toString().trimmed();
    if (!timeZoneId.isEmpty()) {
        const QTimeZone zone(timeZoneId.toUtf8());
        if (zone.isValid()) {
            info.timezoneOffsetSeconds = zone.offsetFromUtc(QDateTime::currentDateTimeUtc());
        } else {
            info.timezoneOffsetSeconds = m_config.offlineTimezoneOffsetSeconds;
        }
    } else {
        info.timezoneOffsetSeconds = m_config.offlineTimezoneOffsetSeconds;
    }

    Logger::instance().log(QString("Clima actualizado: %1 - %2 %3.")
                           .arg(info.city)
                           .arg(info.temperature, 0, 'f', 1)
                           .arg(info.temperatureUnit));
    emit weatherUpdated(info);
}

void NetworkService::onBackgroundReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }

    const QByteArray payload = reply->readAll();
    const QString errorString = reply->errorString();
    const bool success = reply->error() == QNetworkReply::NoError;
    reply->deleteLater();

    if (!success) {
        Logger::instance().log(QString("Fallo la descarga del fondo: %1").arg(errorString));
        useFallbackBackground("No se pudo descargar el fondo principal. Se usa la imagen local.");
        return;
    }

    QString savedPath;
    if (!saveImageToCache(payload, savedPath)) {
        Logger::instance().log("La imagen descargada no era valida o no pudo guardarse.");
        useFallbackBackground("La imagen descargada no era valida. Se usa la imagen local.");
        return;
    }

    m_backgroundPrepared = true;
    m_backgroundPath = savedPath;
    Logger::instance().log(QString("Imagen principal descargada y guardada en cache: %1").arg(savedPath));
    emit backgroundReady(savedPath, true);
}

void NetworkService::applyProxy()
{
    if (!m_config.proxy.enabled || m_config.proxy.host.isEmpty()) {
        m_manager.setProxy(QNetworkProxy::NoProxy);
        Logger::instance().log("Proxy desactivado.");
        return;
    }

    QNetworkProxy proxy(QNetworkProxy::HttpProxy,
                        m_config.proxy.host,
                        m_config.proxy.port,
                        m_config.proxy.username,
                        m_config.proxy.password);
    m_manager.setProxy(proxy);

    Logger::instance().log(QString("Proxy configurado en %1:%2")
                           .arg(m_config.proxy.host)
                           .arg(m_config.proxy.port));
}

bool NetworkService::isNetworkAvailable() const
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QNetworkConfigurationManager manager;
    return manager.isOnline();
#else
    Q_UNUSED(m_manager)
    return true;
#endif
}

void NetworkService::emitOfflineWeather(const QString &message)
{
    WeatherInfo info;
    info.valid = true;
    info.offline = true;
    info.city = cleanCityName(m_config.city);
    info.description = capitalizeText(m_config.offlineDescription);
    info.temperature = useImperialUnits(m_config)
        ? ((m_config.offlineTemperature * 9.0) / 5.0) + 32.0
        : m_config.offlineTemperature;
    info.temperatureUnit = temperatureUnitLabel(m_config);
    info.timezoneOffsetSeconds = m_config.offlineTimezoneOffsetSeconds;
    info.fetchedAtUtc = QDateTime::currentDateTimeUtc();
    info.errorMessage = message;
    info.sourceLabel = "Modo offline";

    Logger::instance().log(QString("Modo offline activado para el clima. Motivo: %1").arg(message));
    emit weatherUpdated(info);
}

void NetworkService::useFallbackBackground(const QString &message)
{
    QString fallbackPath;
    if (!copyFallbackBackground(fallbackPath)) {
        Logger::instance().log("No se pudo preparar la imagen local de respaldo.");
        emit backgroundError("No se pudo preparar ninguna imagen de fondo.");
        return;
    }

    m_backgroundPrepared = true;
    m_backgroundPath = fallbackPath;

    Logger::instance().log(QString("Usando imagen de fondo local de respaldo. Motivo: %1").arg(message));
    emit backgroundError(message);
    emit backgroundReady(fallbackPath, false);
}

bool NetworkService::saveImageToCache(const QByteArray &data, QString &savedPath)
{
    QImage image;
    if (!image.loadFromData(data)) {
        return false;
    }

    savedPath = AppConfigLoader::backgroundCachePath();
    QFile::remove(savedPath);
    return image.save(savedPath, "JPG", 92);
}

bool NetworkService::copyFallbackBackground(QString &savedPath)
{
    QFile resourceFile(AppConfigLoader::fallbackBackgroundResourcePath());
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QByteArray data = resourceFile.readAll();
    return saveImageToCache(data, savedPath);
}
