#ifndef VPSMONITOR_H
#define VPSMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class VpsMonitor : public QObject {
    Q_OBJECT
public:
    explicit VpsMonitor(QObject *parent = nullptr);
    void checkServer(const QString &url);

signals:
    // Señal que enviará los datos procesados a la Ventana Principal
    void statsUpdated(QJsonObject stats);
    void errorDetected(QString msg);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *netManager;
};

#endif