#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include "loginwidget.h"
#include "dashboardwidget.h"

class AppController : public QObject
{
    Q_OBJECT

private:
    LoginWidget *login;
    DashboardWidget *dashboard;

public:
    explicit AppController(QObject *parent = nullptr);
    void start();

private:
    bool sessionValida();
    void mostrarLogin();
    void mostrarDashboard(QString usuario);
};

#endif