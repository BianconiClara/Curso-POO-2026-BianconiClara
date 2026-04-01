#include "loginwidget.h"
#include "filemanager.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    // 🔹 CONTENEDOR CENTRAL (card)
    QFrame *card = new QFrame();
    card->setObjectName("card");
    card->setFixedWidth(300);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(15);

    // 🔹 TÍTULO
    QLabel *title = new QLabel("Iniciar sesión");
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);

    // 🔹 INPUTS
    userEdit = new QLineEdit();
    userEdit->setPlaceholderText("Usuario");

    passEdit = new QLineEdit();
    passEdit->setPlaceholderText("Contraseña");
    passEdit->setEchoMode(QLineEdit::Password);

    // 🔹 BOTÓN
    loginBtn = new QPushButton("Ingresar");
    loginBtn->setObjectName("loginBtn");
    loginBtn->setMinimumHeight(35);

    // 🔹 ARMADO CARD
    cardLayout->addWidget(title);
    cardLayout->addWidget(userEdit);
    cardLayout->addWidget(passEdit);
    cardLayout->addWidget(loginBtn);

    // 🔹 LAYOUT PRINCIPAL (centrado)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(card, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setLayout(mainLayout);

    // 🔹 EVENTO
    connect(loginBtn, &QPushButton::clicked, this, &LoginWidget::intentarLogin);

    // 🔹 ESTILOS
    setStyleSheet(R"(
        QWidget {
            background-color: #1e1e2f;
            color: #e0e0e0;
            font-family: Segoe UI;
            font-size: 14px;
        }

        QFrame#card {
            background-color: #2a2a40;
            border-radius: 12px;
            padding: 20px;
        }

        QLabel#title {
            font-size: 20px;
            font-weight: bold;
        }

        QLineEdit {
            background-color: #1e1e2f;
            border: 1px solid #3a3a5a;
            border-radius: 6px;
            padding: 8px;
        }

        QLineEdit:focus {
            border: 1px solid #5a5aff;
        }

        QPushButton {
            background-color: #5a5aff;
            border-radius: 8px;
            padding: 8px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #7070ff;
        }

        QPushButton:pressed {
            background-color: #4040cc;
        }
    )");
}

void LoginWidget::intentarLogin()
{
    QString user = userEdit->text();
    QString pass = passEdit->text();

    if (FileManager::validarUsuario(user, pass))
    {
        FileManager::guardarSesion(user);
        emit loginExitoso(user);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Credenciales incorrectas");
    }
}