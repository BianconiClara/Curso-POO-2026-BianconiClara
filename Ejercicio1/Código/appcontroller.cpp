#include "appcontroller.h"
#include "filemanager.h"
#include <QDateTime>

AppController::AppController(QObject *parent) : QObject(parent)
{
    login = nullptr;
    dashboard = nullptr;
}

void AppController::start()
{
    if (sessionValida())
    {
        QString user = FileManager::leerUsuarioSesion();
        mostrarDashboard(user);
    }
    else
    {
        mostrarLogin();
    }
}

bool AppController::sessionValida()
{
    if (!FileManager::existeSesion())
        return false;

    qint64 timestamp = FileManager::leerTimestamp();
    qint64 ahora = QDateTime::currentSecsSinceEpoch();

    return (ahora - timestamp) <= 300; // 5 minutos
}

void AppController::mostrarLogin()
{
    login = new LoginWidget();
    login->show();

    connect(login, &LoginWidget::loginExitoso, [=](QString user){
        login->close();
        mostrarDashboard(user);
    });
}

void AppController::mostrarDashboard(QString usuario)
{
    dashboard = new DashboardWidget(usuario);
    dashboard->show();
}