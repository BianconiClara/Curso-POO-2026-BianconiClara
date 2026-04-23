#ifndef MODELS_H
#define MODELS_H

#include <QDateTime>
#include <QString>

struct ProxySettings
{
    bool enabled = false;
    QString host;
    int port = 8080;
    QString username;
    QString password;
};

struct AppConfig
{
    QString configPath;
    QString apiBaseUrl = "https://api.weatherapi.com/v1/current.json";
    QString apiKey;
    QString city = "Cordoba,Argentina";
    QString units = "metric";
    QString language = "es";
    QString backgroundUrl = "https://upload.wikimedia.org/wikipedia/commons/a/a5/Cordoba_vista_aerea.JPG";
    int lockSeconds = 15;
    QString offlineDescription = "cielo parcialmente nublado";
    double offlineTemperature = 21.5;
    int offlineTimezoneOffsetSeconds = -10800;
    ProxySettings proxy;
};

struct WeatherInfo
{
    bool valid = false;
    bool offline = false;
    QString city;
    QString description;
    double temperature = 0.0;
    QString temperatureUnit = "C";
    int timezoneOffsetSeconds = -10800;
    QDateTime fetchedAtUtc;
    QString errorMessage;
    QString sourceLabel;
};

Q_DECLARE_METATYPE(WeatherInfo)

#endif // MODELS_H
