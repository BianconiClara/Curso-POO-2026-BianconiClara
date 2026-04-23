#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "climawidget.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "networkservice.h"

#include <QObject>

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QObject *parent = nullptr);
    void start();

private slots:
    void onLoginValidated();
    void onBackgroundReady(QString localPath, bool fromInternet);
    void onBackgroundError(QString message);

private:
    void openMainWindow();

    AppConfig m_config;
    ClimaWidget *m_climaWidget = nullptr;
    LoginWindow *m_loginWindow = nullptr;
    MainWindow *m_mainWindow = nullptr;
    NetworkService *m_networkService = nullptr;
    bool m_loginApproved = false;
    bool m_backgroundPrepared = false;
    QString m_backgroundPath;
};

#endif // APPCONTROLLER_H
