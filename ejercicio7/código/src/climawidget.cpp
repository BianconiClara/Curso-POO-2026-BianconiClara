#include "climawidget.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>

ClimaWidget::ClimaWidget(QWidget *parent)
    : Pantalla(parent)
{
    ClimaWidget::inicializarPantalla();
}

QString ClimaWidget::nombrePantalla() const
{
    return "Clima";
}

void ClimaWidget::inicializarPantalla()
{
    setObjectName("climaWidget");

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(8);

    m_cityLabel = new QLabel("Cordoba", this);
    m_cityLabel->setObjectName("cityLabel");

    m_temperatureLabel = new QLabel("-- C", this);
    m_temperatureLabel->setObjectName("temperatureLabel");

    m_descriptionLabel = new QLabel("Esperando datos del clima", this);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setObjectName("descriptionLabel");

    m_timeLabel = new QLabel("Hora local --:--", this);
    m_timeLabel->setObjectName("timeLabel");

    m_sourceLabel = new QLabel("Fuente: pendiente", this);
    m_sourceLabel->setObjectName("sourceLabel");

    m_statusLabel = new QLabel("Consultando clima actual...", this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setObjectName("statusLabel");

    rootLayout->addWidget(m_cityLabel);
    rootLayout->addWidget(m_temperatureLabel);
    rootLayout->addWidget(m_descriptionLabel);
    rootLayout->addWidget(m_timeLabel);
    rootLayout->addWidget(m_sourceLabel);
    rootLayout->addWidget(m_statusLabel);

    setStyleSheet(
        "QWidget#climaWidget {"
        "  background-color: rgba(15, 37, 61, 192);"
        "  border: 1px solid rgba(255, 255, 255, 35);"
        "  border-radius: 18px;"
        "  color: #f4f7fb;"
        "}"
        "QLabel#cityLabel {"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "}"
        "QLabel#temperatureLabel {"
        "  font-size: 32px;"
        "  font-weight: 700;"
        "}"
        "QLabel#descriptionLabel {"
        "  color: #000000;"
        "}"
        "QLabel#timeLabel {"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "}"
        "QLabel#sourceLabel {"
        "  color: #000000;"
        "}"
        "QLabel#statusLabel {"
        "  color: #000000;"
        "}"
    );

    m_clockTimer.setInterval(1000);
    connect(&m_clockTimer, &QTimer::timeout, this, &ClimaWidget::updateClock);
    m_clockTimer.start();
}

void ClimaWidget::actualizarContenido()
{
    updateClock();
}

void ClimaWidget::setWeatherInfo(WeatherInfo info)
{
    m_weather = info;

    m_cityLabel->setText(info.city.isEmpty() ? "Cordoba" : info.city);
    const QString unit = info.temperatureUnit.isEmpty() ? QString("C") : info.temperatureUnit;
    m_temperatureLabel->setText(QString("%1 %2").arg(info.temperature, 0, 'f', 1).arg(unit));
    m_descriptionLabel->setText(info.description.isEmpty() ? "Sin descripcion disponible" : info.description);
    m_sourceLabel->setText(QString("Fuente: %1").arg(info.sourceLabel.isEmpty() ? "servicio externo" : info.sourceLabel));
    m_timeLabel->setText(buildTimeText());

    if (!info.errorMessage.isEmpty()) {
        m_statusLabel->setText(info.errorMessage);
        m_statusLabel->setStyleSheet("color: #000000;");
    } else {
        m_statusLabel->setText(QString("Ultima actualizacion: %1")
                               .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
        m_statusLabel->setStyleSheet("color: #000000;");
    }
}

void ClimaWidget::showLoadingMessage(const QString &message)
{
    m_statusLabel->setText(message);
    m_statusLabel->setStyleSheet("color: #000000;");
}

void ClimaWidget::showFriendlyError(const QString &message)
{
    m_statusLabel->setText(message);
    m_statusLabel->setStyleSheet("color: #000000;");
}

void ClimaWidget::updateClock()
{
    m_timeLabel->setText(buildTimeText());
}

QString ClimaWidget::buildTimeText() const
{
    const QDateTime localTime = QDateTime::currentDateTimeUtc().addSecs(m_weather.timezoneOffsetSeconds);
    return QString("Hora local %1").arg(localTime.toString("hh:mm:ss"));
}
