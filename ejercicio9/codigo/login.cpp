#include "login.h"

#include "pintura.h"
#include "ui_login.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

Login::Login(AdminDB *adminDB, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Login),
      adminDB(adminDB),
      pintura(nullptr) {
    ui->setupUi(this);

    setWindowTitle("Ejercicio 09 - Login");

    connect(ui->pbIngresar, &QPushButton::clicked, this, &Login::slotValidar);
    connect(ui->pbSalir, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->leUsuario, &QLineEdit::returnPressed, this, &Login::slotValidar);
    connect(ui->leClave, &QLineEdit::returnPressed, this, &Login::slotValidar);

    ui->leUsuario->setFocus();
}

Login::~Login() {
    delete ui;
}

void Login::slotValidar() {
    const QString username = ui->leUsuario->text().trimmed();
    const QString password = ui->leClave->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Ingresa usuario y clave.");
        return;
    }

    const Usuario usuario = adminDB->validarUsuario(username, password);

    if (usuario.esValido()) {
        adminDB->registrarAcceso(username, true, "Acceso exitoso");

        pintura = new Pintura(adminDB, usuario);
        connect(pintura, &QWidget::destroyed, this, [this](QObject *) {
            pintura = nullptr;
            show();
            ui->leClave->clear();
            ui->leUsuario->setFocus();
        });

        ui->leClave->clear();
        hide();
        pintura->show();
        pintura->raise();
        pintura->activateWindow();
        return;
    }

    adminDB->registrarAcceso(username, false, "Intento fallido");
    ui->leClave->clear();
    QMessageBox::critical(this, "Sin permisos", "Usuario o clave invalidos.");
}
