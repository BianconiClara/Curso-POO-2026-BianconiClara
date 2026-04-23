#ifndef CLIMAWIDGET_H
#define CLIMAWIDGET_H

#include "models.h"
#include "pantalla.h"

#include <QLabel>
#include <QTimer>

class ClimaWidget : public Pantalla
{
    Q_OBJECT

public:
    explicit ClimaWidget(QWidget *parent = nullptr);
    QString nombrePantalla() const override;

public slots:
    void inicializarPantalla() override;
    void actualizarContenido() override;
    void setWeatherInfo(WeatherInfo info);
    void showLoadingMessage(const QString &message);
    void showFriendlyError(const QString &message);

private slots:
    void updateClock();

private:
    QString buildTimeText() const;

    QLabel *m_cityLabel = nullptr;
    QLabel *m_temperatureLabel = nullptr;
    QLabel *m_descriptionLabel = nullptr;
    QLabel *m_timeLabel = nullptr;
    QLabel *m_sourceLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QTimer m_clockTimer;
    WeatherInfo m_weather;
};

#endif // CLIMAWIDGET_H
