#include "loginscreen.h"

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

LoginScreen::LoginScreen(const AppConfig &config, QWidget *parent)
    : Pantalla(config, parent)
{
    LoginScreen::inicializarUI();
    LoginScreen::conectarEventos();
    cargarDatos();
}

QString LoginScreen::nombrePantalla() const
{
    return "Login";
}

void LoginScreen::inicializarUI()
{
    setObjectName("loginScreen");
    setWindowTitle("Ejercicio 08 - Login inicial");
    setMinimumSize(760, 520);
    resize(900, 600);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "QWidget#loginScreen {"
        "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "    stop:0 #10212b, stop:0.55 #173845, stop:1 #244f55);"
        "}"
        "QFrame#loginCard {"
        "  background-color: rgba(247, 243, 237, 235);"
        "  border: 1px solid rgba(255, 255, 255, 90);"
        "  border-radius: 28px;"
        "}"
        "QLineEdit {"
        "  background: #fffdf9;"
        "  border: 1px solid #d1c5b6;"
        "  border-radius: 14px;"
        "  padding: 12px 14px;"
        "  font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "  border: 1px solid #2f7d74;"
        "}"
        "QPushButton {"
        "  background-color: #be6a45;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 15px;"
        "  padding: 12px 18px;"
        "  font-size: 14px;"
        "  font-weight: 700;"
        "}"
        "QPushButton:hover {"
        "  background-color: #cb7c58;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #c6b7a8;"
        "  color: #f3ece4;"
        "}"
    );

    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(60, 48, 60, 48);
    pageLayout->setSpacing(20);

    pageLayout->addStretch();

    QFrame *card = new QFrame(this);
    card->setObjectName("loginCard");
    card->setMaximumWidth(1000);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(20, 32, 20, 32);
    cardLayout->setSpacing(16);



    m_titleLabel = new QLabel("Editor multilenguaje", card);
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setStyleSheet("color: #18313a; font-weight: 800;");

    m_subtitleLabel = new QLabel(
        "Inicia sesion con las credenciales "
        "Despues de tres intentos fallidos se activa un bloqueo temporal.",
        card);
    m_subtitleLabel->setWordWrap(true);
    m_subtitleLabel->setStyleSheet("color: #4b5f67; font-size: 14px;");

    m_userHintLabel = new QLabel(card);
    m_userHintLabel->setStyleSheet("color: #6b5d52; font-size: 13px;");

    m_userEdit = new QLineEdit(card);
    m_userEdit->setPlaceholderText("Usuario");

    m_passwordEdit = new QLineEdit(card);
    m_passwordEdit->setPlaceholderText("Clave");
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton("Ingresar", card);
    m_loginButton->setMinimumHeight(60);

    m_statusLabel = new QLabel(card);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setMinimumHeight(60);
    m_statusLabel->setStyleSheet(
        "background-color: #efe5d9;"
        "color: #6b5d52;"
        "border-radius: 16px;"
        "padding: 12px 14px;"
    );

    cardLayout->addWidget(m_badgeLabel);
    cardLayout->addWidget(m_titleLabel);
    cardLayout->addWidget(m_subtitleLabel);
    cardLayout->addWidget(m_userHintLabel);
    cardLayout->addWidget(m_userEdit);
    cardLayout->addWidget(m_passwordEdit);
    cardLayout->addWidget(m_loginButton);
    cardLayout->addWidget(m_statusLabel);

    pageLayout->addWidget(card, 0, Qt::AlignHCenter);
    pageLayout->addStretch();

    actualizarTipografia();
}

void LoginScreen::conectarEventos()
{
    connect(m_loginButton, &QPushButton::clicked, this, &LoginScreen::intentarLogin);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginScreen::intentarLogin);
    connect(m_userEdit, &QLineEdit::returnPressed, m_passwordEdit, qOverload<>(&QWidget::setFocus));
    connect(m_userEdit, &QLineEdit::textChanged, this, [this]() { validarEstado(); });
    connect(m_passwordEdit, &QLineEdit::textChanged, this, [this]() { validarEstado(); });
}

void LoginScreen::cargarDatos()
{
    if (m_userEdit->text().trimmed().isEmpty()) {
        m_userEdit->setText(m_config.initialUser);
    }

    m_userHintLabel->setText(QString("Usuario inicial: %1 | Clave inicial: %2")
                             .arg(m_config.initialUser, m_config.initialPassword));
    actualizarMensaje("Listo para autenticar. Puedes usar Enter para enviar el formulario.");
    LoginScreen::validarEstado();
}

bool LoginScreen::validarEstado()
{
    const bool widgetsReady = m_userEdit != nullptr
        && m_passwordEdit != nullptr
        && m_loginButton != nullptr;
    const bool hasContent = widgetsReady
        && !m_userEdit->text().trimmed().isEmpty()
        && !m_passwordEdit->text().isEmpty();

    if (widgetsReady) {
        m_loginButton->setEnabled(hasContent);
    }

    return widgetsReady;
}

void LoginScreen::registrarEvento(const QString &descripcion)
{
    Logger::instance().log(QString("[%1] %2").arg(nombrePantalla(), descripcion));
}

void LoginScreen::intentarLogin()
{
    const QString userName = m_userEdit->text().trimmed();
    const QString password = m_passwordEdit->text();

    if (userName.isEmpty() || password.isEmpty()) {
        registrarEvento("Intento de login con campos incompletos.");
        actualizarMensaje("Completa usuario y clave antes de continuar.", true);
        validarEstado();
        return;
    }

    if (userName == m_config.initialUser && password == m_config.initialPassword) {
        m_failedAttempts = 0;
        registrarEvento(QString("Login exitoso para %1.").arg(userName));
        actualizarMensaje("Credenciales correctas. Abriendo el editor principal.");
        emit loginExitoso(userName);
        return;
    }

    ++m_failedAttempts;
    registrarEvento(QString("Login fallido para %1. Intento %2 de 3.")
                    .arg(userName)
                    .arg(m_failedAttempts));

    m_passwordEdit->clear();
    m_passwordEdit->setFocus();

    if (m_failedAttempts >= 3) {
        actualizarMensaje(QString("Se alcanzaron 3 intentos fallidos. Bloqueo por %1 segundos.")
                          .arg(m_config.lockSeconds), true);
        emit bloqueoSolicitado(m_config.lockSeconds);
        return;
    }

    actualizarMensaje(QString("Credenciales invalidas. Quedan %1 intento(s).")
                      .arg(3 - m_failedAttempts), true);
    validarEstado();
}

void LoginScreen::restablecerFormulario()
{
    m_failedAttempts = 0;
    m_passwordEdit->clear();
    m_userEdit->setText(m_config.initialUser);
    m_userEdit->setFocus();
    actualizarMensaje("El bloqueo termino. Puedes volver a intentar.");
    validarEstado();
    registrarEvento("Formulario restaurado despues del bloqueo.");
}

void LoginScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        intentarLogin();
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        m_passwordEdit->clear();
        actualizarMensaje("Clave borrada. Vuelve a escribirla cuando quieras.");
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_F1) {
        actualizarMensaje("Ayuda rapida: Enter envia, Escape limpia la clave y el usuario inicial se lee desde app.ini.");
        registrarEvento("Se mostro ayuda rapida del login.");
        event->accept();
        return;
    }

    QWidget::keyPressEvent(event);
}

void LoginScreen::mousePressEvent(QMouseEvent *event)
{
    if (childAt(event->position().toPoint()) == this) {
        m_userEdit->setFocus();
        actualizarMensaje("Haz click en los campos para editar o presiona Enter para autenticar.");
    }

    QWidget::mousePressEvent(event);
}

void LoginScreen::resizeEvent(QResizeEvent *event)
{
    actualizarTipografia();
    QWidget::resizeEvent(event);
}

void LoginScreen::closeEvent(QCloseEvent *event)
{
    const auto answer = QMessageBox::question(this,
                                              "Salir",
                                              "Deseas cerrar la aplicacion desde la pantalla de login?",
                                              QMessageBox::Yes | QMessageBox::No,
                                              QMessageBox::No);

    if (answer == QMessageBox::Yes) {
        registrarEvento("Cierre confirmado por el usuario.");
        event->accept();
        return;
    }

    registrarEvento("Cierre cancelado por el usuario.");
    event->ignore();
}

void LoginScreen::focusInEvent(QFocusEvent *event)
{
    actualizarMensaje("Pantalla de login activa. Ingresa las credenciales para continuar.");
    registrarEvento("La pantalla recibio el foco.");
    QWidget::focusInEvent(event);
}

void LoginScreen::focusOutEvent(QFocusEvent *event)
{
    registrarEvento("La pantalla perdio el foco.");
    QWidget::focusOutEvent(event);
}

void LoginScreen::actualizarMensaje(const QString &mensaje, bool error)
{
    m_statusLabel->setText(mensaje);
    m_statusLabel->setStyleSheet(QString(
        "background-color:%1;"
        "color:%2;"
        "border-radius:16px;"
        "padding:12px 14px;")
        .arg(error ? "#f9d9d5" : "#efe5d9",
             error ? "#7e3024" : "#6b5d52"));
}

void LoginScreen::actualizarTipografia()
{
    if (m_titleLabel == nullptr) {
        return;
    }

    QFont titleFont("Bahnschrift SemiBold", width() > 900 ? 22 : 18);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
}
