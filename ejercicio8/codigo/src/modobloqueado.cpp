#include "modobloqueado.h"

#include "logger.h"

#include <QCloseEvent>
#include <QFocusEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QVBoxLayout>

ModoBloqueado::ModoBloqueado(const AppConfig &config, QWidget *parent)
    : Pantalla(config, parent)
{
    inicializarUI();
    conectarEventos();
    cargarDatos();
}

QString ModoBloqueado::nombrePantalla() const
{
    return "ModoBloqueado";
}

void ModoBloqueado::inicializarUI()
{
    setObjectName("blockedScreen");
    setWindowTitle("Ejercicio 08 - Modo bloqueado");
    resize(760, 420);
    setMinimumSize(860, 580);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "QWidget#blockedScreen {"
        "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "    stop:0 #2d1217, stop:0.45 #4a1e25, stop:1 #7b332e);"
        "}"
        "QFrame#blockedCard {"
        "  background-color: rgba(252, 243, 237, 235);"
        "  border-radius: 28px;"
        "  border: 1px solid rgba(255, 255, 255, 85);"
        "}"
        "QProgressBar {"
        "  border: 1px solid #d8beb5;"
        "  border-radius: 10px;"
        "  text-align: center;"
        "  background: #f8ebe5;"
        "  color: #73352e;"
        "  padding: 2px;"
        "}"
        "QProgressBar::chunk {"
        "  border-radius: 8px;"
        "  background: #bf6648;"
        "}"
    );

    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(60, 30, 60, 30);
    pageLayout->setSpacing(18);
    pageLayout->addStretch();

    QFrame *card = new QFrame(this);
    card->setObjectName("blockedCard");
    card->setMaximumWidth(1000);
    card->setMinimumHeight(460);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(40, 30, 40, 30);
    cardLayout->setSpacing(16);

    m_badgeLabel = new QLabel("Seguridad activa", card);
    m_badgeLabel->setStyleSheet("color: #a54c36; font-size: 12px; font-weight: 700;");

    m_titleLabel = new QLabel("Acceso bloqueado temporalmente", card);
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setStyleSheet("color: #3d2026; font-weight: 800;");

    m_subtitleLabel = new QLabel(
        "Se detectaron tres intentos fallidos. El sistema desactiva el login por un tiempo y luego vuelve a habilitarlo.",
        card);
    m_subtitleLabel->setWordWrap(true);
    m_subtitleLabel->setStyleSheet("color: #6d4c49; font-size: 14px;");

    m_countdownLabel = new QLabel(card);
    m_countdownLabel->setAlignment(Qt::AlignCenter);
    m_countdownLabel->setStyleSheet("color: #7b332e; font-weight: 800, font-size: 50px ;");

    m_progressBar = new QProgressBar(card);
    m_progressBar->setMinimumHeight(26);
    m_progressBar->setTextVisible(true);

    m_statusLabel = new QLabel(card);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet(
        "background-color: #f6e4dd;"
        "color: #6d4c49;"
        "border-radius: 16px;"
        "padding: 12px 14px;"
    );

    cardLayout->addWidget(m_badgeLabel);
    cardLayout->addWidget(m_titleLabel);
    cardLayout->addWidget(m_subtitleLabel);
    cardLayout->addWidget(m_countdownLabel);
    cardLayout->addWidget(m_progressBar);
    cardLayout->addWidget(m_statusLabel);

    pageLayout->addWidget(card, 0, Qt::AlignHCenter);
    pageLayout->addStretch();

    QFont titleFont("Bahnschrift SemiBold", 18);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    QFont counterFont("Bahnschrift SemiBold", 28);
    counterFont.setBold(true);
    m_countdownLabel->setFont(counterFont);
}

void ModoBloqueado::conectarEventos()
{
    connect(&m_timer, &QTimer::timeout, this, &ModoBloqueado::actualizarCuentaRegresiva);
    m_timer.setInterval(1000);
}

void ModoBloqueado::cargarDatos()
{
    actualizarVista();
}

bool ModoBloqueado::validarEstado()
{
    return m_progressBar != nullptr
        && m_countdownLabel != nullptr
        && m_totalSeconds >= 0
        && m_remainingSeconds >= 0;
}

void ModoBloqueado::registrarEvento(const QString &descripcion)
{
    Logger::instance().log(QString("[%1] %2").arg(nombrePantalla(), descripcion));
}

void ModoBloqueado::iniciarBloqueo(int seconds)
{
    m_totalSeconds = qMax(1, seconds);
    m_remainingSeconds = m_totalSeconds;
    m_timer.start();
    actualizarVista();
    registrarEvento(QString("Bloqueo iniciado por %1 segundos.").arg(seconds));
}

void ModoBloqueado::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        m_statusLabel->setText("Espera a que termine la cuenta regresiva para volver al login.");
        registrarEvento("Se intercepto una tecla mientras el sistema seguia bloqueado.");
        event->accept();
        return;
    }

    QWidget::keyPressEvent(event);
}

void ModoBloqueado::mousePressEvent(QMouseEvent *event)
{
    m_statusLabel->setText(QString("Quedan %1 segundo(s) para reactivar el login.")
                           .arg(m_remainingSeconds));
    QWidget::mousePressEvent(event);
}

void ModoBloqueado::resizeEvent(QResizeEvent *event)
{
    if (m_titleLabel != nullptr) {
        QFont titleFont("Bahnschrift SemiBold", width() > 820 ? 22 : 18);
        titleFont.setBold(true);
        m_titleLabel->setFont(titleFont);
    }

    if (m_countdownLabel != nullptr) {
        QFont counterFont("Bahnschrift SemiBold", width() > 820 ? 34 : 28);
        counterFont.setBold(true);
        m_countdownLabel->setFont(counterFont);
    }

    QWidget::resizeEvent(event);
}

void ModoBloqueado::closeEvent(QCloseEvent *event)
{
    const auto answer = QMessageBox::question(this,
                                              "Salir",
                                              "Deseas cerrar la aplicacion aunque el acceso siga bloqueado?",
                                              QMessageBox::Yes | QMessageBox::No,
                                              QMessageBox::No);

    if (answer == QMessageBox::Yes) {
        registrarEvento("Cierre confirmado durante el bloqueo.");
        event->accept();
        return;
    }

    registrarEvento("Se cancelo el cierre durante el bloqueo.");
    event->ignore();
}

void ModoBloqueado::focusInEvent(QFocusEvent *event)
{
    registrarEvento("La pantalla de bloqueo recibio el foco.");
    QWidget::focusInEvent(event);
}

void ModoBloqueado::focusOutEvent(QFocusEvent *event)
{
    registrarEvento("La pantalla de bloqueo perdio el foco.");
    QWidget::focusOutEvent(event);
}

void ModoBloqueado::actualizarCuentaRegresiva()
{
    if (m_remainingSeconds > 0) {
        --m_remainingSeconds;
    }

    actualizarVista();

    if (m_remainingSeconds <= 0) {
        m_timer.stop();
        registrarEvento("El bloqueo temporal finalizo.");
        emit bloqueoFinalizado();
    }
}

void ModoBloqueado::actualizarVista()
{
    if (m_totalSeconds <= 0) {
        m_progressBar->setRange(0, 1);
        m_progressBar->setValue(0);
        m_progressBar->setFormat("0 / 0 s");
        m_countdownLabel->setText("-- s");
        m_statusLabel->setText("El modo bloqueado esta listo, pero todavia no fue activado.");
        return;
    }

    const int elapsed = m_totalSeconds - m_remainingSeconds;
    m_progressBar->setRange(0, qMax(1, m_totalSeconds));
    m_progressBar->setValue(elapsed);
    m_progressBar->setFormat(QString("%1 / %2 s").arg(elapsed).arg(qMax(1, m_totalSeconds)));

    m_countdownLabel->setText(QString("%1 s").arg(m_remainingSeconds));
    m_statusLabel->setText(
        m_remainingSeconds > 0
            ? QString("El editor seguira protegido durante %1 segundo(s).").arg(m_remainingSeconds)
            : "Bloqueo finalizado. Regresando al login.");
}
