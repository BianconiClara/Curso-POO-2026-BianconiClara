#include "loginwindow.h"
#include "logger.h"
#include "ui_login.h"

#include <QLayoutItem>
#include <QVBoxLayout>

LoginWindow::LoginWindow(int lockSeconds, QWidget *parent)
    : Pantalla(parent)
    , ui(new Ui::LoginForm)
    , m_lockSeconds(lockSeconds)
{
    ui->setupUi(this);
    inicializarPantalla();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

QString LoginWindow::nombrePantalla() const
{
    return "Login";
}

void LoginWindow::setWeatherWidget(ClimaWidget *weatherWidget)
{
    if (weatherWidget == nullptr) {
        return;
    }

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->weatherHostFrame->layout());
    if (layout == nullptr) {
        layout = new QVBoxLayout(ui->weatherHostFrame);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    while (layout->count() > 0) {
        QLayoutItem *item = layout->takeAt(0);
        if (item->widget() != nullptr) {
            item->widget()->setParent(nullptr);
        }
        delete item;
    }

    layout->addWidget(weatherWidget);
}

void LoginWindow::inicializarPantalla()
{
    setWindowTitle("Ejercicio 07 - Login con clima");
    resize(960, 620);
    setMinimumSize(900, 580);

    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(handleLoginClicked()));
    connect(&m_lockTimer, SIGNAL(timeout()), this, SLOT(handleLockTick()));

    m_lockTimer.setInterval(1000);
    updateButtonState();
    ui->statusLabel->clear();
    ui->usernameLineEdit->setFocus();
}

void LoginWindow::actualizarContenido()
{
    updateButtonState();
}

void LoginWindow::showUserMessage(const QString &message, bool error)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(error ? "color: #8f2d23;" : "color: #24573d;");
}

void LoginWindow::setBackgroundPending(bool pending)
{
    m_waitingForBackground = pending;
    if (pending) {
        showUserMessage("Usuario valido. Descargando imagen principal...", false);
    }
    updateButtonState();
}

void LoginWindow::handleLoginClicked()
{
    if (m_waitingForBackground) {
        showUserMessage("La ventana principal se esta preparando. Espera un instante...", false);
        return;
    }

    if (m_lockTimer.isActive()) {
        updateButtonState();
        return;
    }

    const QString username = ui->usernameLineEdit->text().trimmed();
    const QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        Logger::instance().log("Intento de login con campos incompletos.");
        showUserMessage("Completa usuario y contrasena antes de continuar.", true);
        return;
    }

    if (username == "admin" && password == "1234") {
        Logger::instance().log("Login exitoso para el usuario admin.");
        m_failedAttempts = 0;
        showUserMessage("Credenciales correctas. Preparando acceso...", false);
        emit loginValidated();
        return;
    }

    ++m_failedAttempts;
    Logger::instance().log(QString("Login fallido para '%1'. Intento %2.")
                           .arg(username)
                           .arg(m_failedAttempts));

    ui->passwordLineEdit->clear();
    ui->passwordLineEdit->setFocus();

    if (m_failedAttempts >= 3) {
        startTemporaryLock();
        return;
    }

    showUserMessage(QString("Usuario o contrasena incorrectos. Intentos restantes: %1.")
                    .arg(3 - m_failedAttempts), true);
}

void LoginWindow::handleLockTick()
{
    --m_remainingLockSeconds;

    if (m_remainingLockSeconds <= 0) {
        m_lockTimer.stop();
        m_failedAttempts = 0;
        showUserMessage("El bloqueo termino. Ya puedes volver a intentar.", false);
    }

    updateButtonState();
}

void LoginWindow::startTemporaryLock()
{
    m_remainingLockSeconds = m_lockSeconds;
    m_lockTimer.start();
    Logger::instance().log(QString("Login bloqueado durante %1 segundos por tres intentos fallidos.")
                           .arg(m_lockSeconds));
    showUserMessage(QString("Demasiados intentos fallidos. Bloqueado por %1 segundos.")
                    .arg(m_lockSeconds), true);
    updateButtonState();
}

void LoginWindow::updateButtonState()
{
    const bool locked = m_lockTimer.isActive();
    const bool enabled = !locked && !m_waitingForBackground;

    ui->usernameLineEdit->setEnabled(enabled);
    ui->passwordLineEdit->setEnabled(enabled);
    ui->loginButton->setEnabled(enabled);

    if (m_waitingForBackground) {
        ui->loginButton->setText("Preparando...");
    } else if (locked) {
        ui->loginButton->setText(QString("Bloqueado %1s").arg(m_remainingLockSeconds));
    } else {
        ui->loginButton->setText("Ingresar");
    }
}
