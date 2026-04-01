#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QJsonObject>
#include "vpsmonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MonitorWidget; }
QT_END_NAMESPACE

class MonitorWidget : public QWidget {
    Q_OBJECT

public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    ~MonitorWidget();

private slots:
    void on_btnCerrar_clicked();
    void updateUI(QJsonObject data);
    void showError(QString msg);
    void on_btnRefrescar_clicked();
    void updateTimerInterval();

private:
    Ui::MonitorWidget *ui;
    VpsMonitor *monitor;
    QTimer *timer;
};

#endif