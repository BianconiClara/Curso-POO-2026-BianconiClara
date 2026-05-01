/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTitulo;
    QFormLayout *formLayout;
    QLabel *labelUsuario;
    QLineEdit *leUsuario;
    QLabel *labelClave;
    QLineEdit *leClave;
    QLabel *lblCredenciales;
    QLabel *lblImagenFlores;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbIngresar;
    QPushButton *pbSalir;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(380, 380);
        Login->setMinimumSize(QSize(380, 380));
        Login->setMaximumSize(QSize(380, 380));
        Login->setAutoFillBackground(false);
        Login->setStyleSheet(QString::fromUtf8("background-color: black;"));
        verticalLayout = new QVBoxLayout(Login);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(22, 22, 22, 22);
        lblTitulo = new QLabel(Login);
        lblTitulo->setObjectName("lblTitulo");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        lblTitulo->setFont(font);
        lblTitulo->setStyleSheet(QString::fromUtf8("color: white;"));
        lblTitulo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(lblTitulo);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setLabelAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        labelUsuario = new QLabel(Login);
        labelUsuario->setObjectName("labelUsuario");
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        labelUsuario->setFont(font1);
        labelUsuario->setStyleSheet(QString::fromUtf8("color:white;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelUsuario);

        leUsuario = new QLineEdit(Login);
        leUsuario->setObjectName("leUsuario");
        leUsuario->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;\n"
"    border: 2px solid #ccc;\n"
"    border-radius: 12px;   /* redondeado */\n"
"    padding: 6px 10px;\n"
"    color: black;          /* texto */\n"
"}"));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, leUsuario);

        labelClave = new QLabel(Login);
        labelClave->setObjectName("labelClave");
        labelClave->setFont(font1);
        labelClave->setStyleSheet(QString::fromUtf8("color:white;"));

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelClave);

        leClave = new QLineEdit(Login);
        leClave->setObjectName("leClave");
        leClave->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;\n"
"    border: 2px solid #ccc;\n"
"    border-radius: 12px;   /* redondeado */\n"
"    padding: 6px 10px;\n"
"    color: black;          /* texto */\n"
"}"));
        leClave->setEchoMode(QLineEdit::EchoMode::Password);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, leClave);


        verticalLayout->addLayout(formLayout);

        lblCredenciales = new QLabel(Login);
        lblCredenciales->setObjectName("lblCredenciales");
        QFont font2;
        font2.setBold(true);
        lblCredenciales->setFont(font2);
        lblCredenciales->setStyleSheet(QString::fromUtf8("color: white;"));
        lblCredenciales->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(lblCredenciales);

        lblImagenFlores = new QLabel(Login);
        lblImagenFlores->setObjectName("lblImagenFlores");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblImagenFlores->sizePolicy().hasHeightForWidth());
        lblImagenFlores->setSizePolicy(sizePolicy);
        lblImagenFlores->setMinimumSize(QSize(110, 110));
        lblImagenFlores->setMaximumSize(QSize(110, 110));
        lblImagenFlores->setMouseTracking(false);
        lblImagenFlores->setTextFormat(Qt::TextFormat::PlainText);
        lblImagenFlores->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/flores.png")));
        lblImagenFlores->setScaledContents(true);
        lblImagenFlores->setAlignment(Qt::AlignmentFlag::AlignCenter);
        lblImagenFlores->setOpenExternalLinks(false);

        verticalLayout->addWidget(lblImagenFlores, 0, Qt::AlignmentFlag::AlignHCenter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbIngresar = new QPushButton(Login);
        pbIngresar->setObjectName("pbIngresar");
        QFont font3;
        font3.setBold(true);
        font3.setItalic(false);
        pbIngresar->setFont(font3);
        pbIngresar->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: pink;     /* estado normal */\n"
"    color: black;               /* letras negras */\n"
"    border: none;\n"
"    border-radius: 15px;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ffb6c1;  /* rosa un poco m\303\241s intenso */\n"
"    color: black;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #ff69b4;  /* rosa m\303\241s oscuro al presionar */\n"
"    color: black;\n"
"}"));

        horizontalLayout->addWidget(pbIngresar);

        pbSalir = new QPushButton(Login);
        pbSalir->setObjectName("pbSalir");
        pbSalir->setFont(font2);
        pbSalir->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: pink;     /* estado normal */\n"
"    color: black;               /* letras negras */\n"
"    border: none;\n"
"    border-radius: 15px;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ffb6c1;  /* rosa un poco m\303\241s intenso */\n"
"    color: black;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #ff69b4;  /* rosa m\303\241s oscuro al presionar */\n"
"    color: black;\n"
"}"));

        horizontalLayout->addWidget(pbSalir);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Login", nullptr));
        lblTitulo->setText(QCoreApplication::translate("Login", "Ejercicio 09 - Login SQLite", nullptr));
        labelUsuario->setText(QCoreApplication::translate("Login", "Usuario", nullptr));
        labelClave->setText(QCoreApplication::translate("Login", "Clave", nullptr));
        lblCredenciales->setText(QCoreApplication::translate("Login", "Credenciales semilla: cbianconi / 123   o   mlopez / poo2026", nullptr));
        lblImagenFlores->setText(QString());
        pbIngresar->setText(QCoreApplication::translate("Login", "Ingresar", nullptr));
        pbSalir->setText(QCoreApplication::translate("Login", "Salir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
