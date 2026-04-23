#include "appconfig.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

namespace
{
QString ensureDirectory(const QString &path)
{
    QDir directory;
    directory.mkpath(path);
    return path;
}

QString readNonEmptySetting(QSettings &settings, const QString &key, const QString &fallback)
{
    const QString value = settings.value(key, fallback).toString().trimmed();
    return value.isEmpty() ? fallback : value;
}

QString defaultConfigPath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("config/app.ini");
}

QString findConfigFromBasePath(const QString &basePath)
{
    if (basePath.trimmed().isEmpty()) {
        return QString();
    }

    QDir directory(basePath);
    while (true) {
        const QString candidate = directory.filePath("config/app.ini");
        if (QFileInfo::exists(candidate)) {
            return QFileInfo(candidate).absoluteFilePath();
        }

        if (!directory.cdUp()) {
            break;
        }
    }

    return QString();
}
}

QString AppConfigLoader::configFilePath()
{
    const QString fromApplicationDir = findConfigFromBasePath(QCoreApplication::applicationDirPath());
    if (!fromApplicationDir.isEmpty()) {
        return fromApplicationDir;
    }

    const QString fromCurrentDir = findConfigFromBasePath(QDir::currentPath());
    if (!fromCurrentDir.isEmpty()) {
        return fromCurrentDir;
    }

    return defaultConfigPath();
}

QString AppConfigLoader::dataRootPath()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataPath.isEmpty()) {
        dataPath = QDir(QCoreApplication::applicationDirPath()).filePath("appdata");
    }
    return ensureDirectory(dataPath);
}

QString AppConfigLoader::cacheDirectoryPath()
{
    return ensureDirectory(QDir(dataRootPath()).filePath("cache"));
}

QString AppConfigLoader::logsDirectoryPath()
{
    return ensureDirectory(QDir(dataRootPath()).filePath("logs"));
}

QString AppConfigLoader::backgroundCachePath()
{
    return QDir(cacheDirectoryPath()).filePath("background-cache.jpg");
}

QString AppConfigLoader::fallbackBackgroundResourcePath()
{
    return ":/images/offline-background.jpg";
}

AppConfig AppConfigLoader::load()
{
    AppConfig config;
    config.configPath = configFilePath();

    QSettings settings(config.configPath, QSettings::IniFormat);
    config.apiBaseUrl = readNonEmptySetting(settings, "api/base_url", config.apiBaseUrl);
    config.apiKey = settings.value("api/key", config.apiKey).toString().trimmed();
    config.city = readNonEmptySetting(settings, "api/city", config.city);
    config.units = readNonEmptySetting(settings, "api/units", config.units);
    config.language = readNonEmptySetting(settings, "api/language", config.language);

    config.backgroundUrl = readNonEmptySetting(settings, "background/url", config.backgroundUrl);

    config.lockSeconds = settings.value("login/lock_seconds", config.lockSeconds).toInt();
    if (config.lockSeconds <= 0) {
        config.lockSeconds = 15;
    }

    config.offlineDescription = readNonEmptySetting(settings, "offline/description", config.offlineDescription);

    bool offlineTemperatureOk = false;
    const double offlineTemperature = settings.value("offline/temperature",
                                                     config.offlineTemperature).toDouble(&offlineTemperatureOk);
    if (offlineTemperatureOk) {
        config.offlineTemperature = offlineTemperature;
    }

    config.offlineTimezoneOffsetSeconds = settings.value("offline/timezone_offset_seconds",
                                                         config.offlineTimezoneOffsetSeconds).toInt();

    config.proxy.enabled = settings.value("proxy/enabled", config.proxy.enabled).toBool();
    config.proxy.host = settings.value("proxy/host", config.proxy.host).toString().trimmed();
    config.proxy.port = settings.value("proxy/port", config.proxy.port).toInt();
    config.proxy.username = settings.value("proxy/username", config.proxy.username).toString().trimmed();
    config.proxy.password = settings.value("proxy/password", config.proxy.password).toString();
    if (config.proxy.port <= 0) {
        config.proxy.port = 8080;
    }

    return config;
}
