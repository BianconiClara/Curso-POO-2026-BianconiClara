#include "monitorwidget.h"
#include "ui_monitorwidget.h" // <--- CRITICO: Resuelve el 'incomplete type'
#include <QDateTime>

MonitorWidget::MonitorWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::MonitorWidget) {
    ui->setupUi(this);

    monitor = new VpsMonitor(this);
    timer = new QTimer(this);

    // Configuración inicial
    ui->lineEndpoint->setText("http://147.93.189.45:8000/health");
    ui->spinBoxIntervalo->setRange(1, 60);
    ui->spinBoxIntervalo->setValue(5);

    // Conexiones de señales y slots
    connect(monitor, &VpsMonitor::statsUpdated, this, &MonitorWidget::updateUI);
    connect(monitor, &VpsMonitor::errorDetected, this, &MonitorWidget::showError);
    connect(timer, &QTimer::timeout, this, &MonitorWidget::on_btnRefrescar_clicked);
    connect(ui->spinBoxIntervalo, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MonitorWidget::updateTimerInterval);

    // Inicio automático
    updateTimerInterval();
    on_btnRefrescar_clicked();
}

void MonitorWidget::updateTimerInterval() {
    timer->start(ui->spinBoxIntervalo->value() * 1000);
}

void MonitorWidget::on_btnRefrescar_clicked() {
    monitor->checkServer(ui->lineEndpoint->text());
}

void MonitorWidget::updateUI(QJsonObject data) {
    // Extracción de datos (ajustado a tu JSON del VPS)
    QString status = data["status"].toString("OFFLINE");
    double load = data["load"].toDouble(0.0);
    int memUsed = data["mem_used"].toInt(0);
    int memTotal = data["mem_total"].toInt(0);
    QString uptime = data["uptime"].toString("--");

    // Actualización de Labels
    ui->lblStatus->setText("● " + status.toUpper());
    ui->lblCPU->setText(QString::number(load * 100, 'f', 1) + "% (Load)");
    ui->lblRAM->setText(QString::number(memUsed) + " MB / " + QString::number(memTotal) + " MB");

    // Lógica visual de alertas (Colores Neón)
    if(status == "OK" && load < 0.85) {
        ui->lblStatus->setStyleSheet("color: #2ecc71; font-weight: bold;");
        ui->frame->setStyleSheet("QFrame#frame { border: 2px solid #2ecc71; border-radius: 10px; background-color: #1a2a1a; }");
    } else {
        ui->lblStatus->setStyleSheet("color: #e74c3c; font-weight: bold;");
        ui->frame->setStyleSheet("QFrame#frame { border: 2px solid #e74c3c; border-radius: 10px; background-color: #2a1a1a; }");
    }

    // Historial con formato
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->txtHistorial->append("<span style='color:#7f8c8d;'>[" + time + "]</span> " + status + " - Up: " + uptime);
}

void MonitorWidget::showError(QString msg) {
    ui->txtHistorial->append("<b style='color:#e74c3c;'>Error: " + msg + "</b>");
}

MonitorWidget::~MonitorWidget() {
    delete ui;
}