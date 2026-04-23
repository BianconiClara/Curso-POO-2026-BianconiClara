#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include "appconfig.h"

#include <QObject>
#include <QNetworkAccessManager>

class NetworkService : public QObject
{
    Q_OBJECT

public:
    explicit NetworkService(const AppConfig &config, QObject *parent = nullptr);

    void requestWeather();
    void requestBackground();

signals:
    void weatherUpdated(WeatherInfo info);
    void backgroundReady(QString localPath, bool fromInternet);
    void backgroundError(QString message);

private slots:
    void onWeatherReply();
    void onBackgroundReply();

private:
    void applyProxy();
    bool isNetworkAvailable() const;
    void emitOfflineWeather(const QString &message);
    void useFallbackBackground(const QString &message);
    bool saveImageToCache(const QByteArray &data, QString &savedPath);
    bool copyFallbackBackground(QString &savedPath);

    AppConfig m_config;
    QNetworkAccessManager m_manager;
    bool m_backgroundPrepared = false;
    QString m_backgroundPath;
};

#endif // NETWORKSERVICE_H
