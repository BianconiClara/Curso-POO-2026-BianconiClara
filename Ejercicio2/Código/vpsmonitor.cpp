#include "vpsmonitor.h"
#include <QJsonDocument>

VpsMonitor::VpsMonitor(QObject *parent) : QObject(parent) {
    netManager = new QNetworkAccessManager(this);
    // Conexión centralizada para todas las peticiones
    connect(netManager, &QNetworkAccessManager::finished, this, &VpsMonitor::onFinished);
}

void VpsMonitor::checkServer(const QString &url) {
    netManager->get(QNetworkRequest(QUrl(url)));
}

void VpsMonitor::onFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        emit statsUpdated(doc.object());
    } else {
        emit errorDetected(reply->errorString());
    }
    reply->deleteLater();
}