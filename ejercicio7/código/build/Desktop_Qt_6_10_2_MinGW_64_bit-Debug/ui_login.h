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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacerLeft;
    QFrame *cardFrame;
    QHBoxLayout *cardLayout;
    QFrame *leftPanel;
    QVBoxLayout *leftLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *hintLabel;
    QFrame *weatherHostFrame;
    QSpacerItem *verticalSpacerLeft;
    QFrame *rightPanel;
    QVBoxLayout *rightLayout;
    QLabel *formTitleLabel;
    QLabel *userFieldLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordFieldLabel;
    QLineEdit *passwordLineEdit;
    QLabel *statusLabel;
    QPushButton *loginButton;
    QSpacerItem *horizontalSpacerRight;

    void setupUi(QWidget *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName("LoginForm");
        LoginForm->resize(960, 620);
        LoginForm->setStyleSheet(QString::fromUtf8("QWidget#LoginForm {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f7f3ec, stop:0.55 #eef5fb, stop:1 #dde8f4);\n"
"}\n"
"QFrame#cardFrame {\n"
"    background-color: rgba(255, 255, 255, 236);\n"
"    border: 1px solid rgba(40, 62, 88, 28);\n"
"    border-radius: 28px;\n"
"}\n"
"QFrame#leftPanel {\n"
"    background-color: rgba(230, 238, 247, 210);\n"
"    border-radius: 22px;\n"
"}\n"
"QLabel#titleLabel {\n"
"    color: #1f2f43;\n"
"    font-size: 30px;\n"
"    font-weight: 700;\n"
"}\n"
"QLabel#subtitleLabel, QLabel#hintLabel, QLabel#credentialsLabel {\n"
"    color: #425466;\n"
"    font-size: 13px;\n"
"}\n"
"QLabel#formTitleLabel {\n"
"    color: #1f2f43;\n"
"    font-size: 22px;\n"
"    font-weight: 700;\n"
"}\n"
"QLabel#fieldLabel {\n"
"    color: #31465b;\n"
"    font-size: 13px;\n"
"    font-weight: 600;\n"
"}\n"
"QLineEdit {\n"
"    background-color: rgba(255, 255, 255, 248);\n"
"    border: 1px solid #c6d2df;\n"
"    border-radius: 14px;\n"
"    padding: 12px;\n"
"    color: #1f2f"
                        "43;\n"
"    font-size: 14px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border: 1px solid #7ca9d6;\n"
"}\n"
"QPushButton {\n"
"    background-color: #d0674f;\n"
"    border: none;\n"
"    border-radius: 16px;\n"
"    color: white;\n"
"    font-size: 15px;\n"
"    font-weight: 700;\n"
"    padding: 13px 18px;\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgba(208, 103, 79, 150);\n"
"    color: rgba(255, 255, 255, 210);\n"
"}\n"
"QLabel#statusLabel {\n"
"    font-size: 13px;\n"
"    color: #31465b;\n"
"}"));
        horizontalLayout = new QHBoxLayout(LoginForm);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(28, 28, 28, 28);
        horizontalSpacerLeft = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacerLeft);

        cardFrame = new QFrame(LoginForm);
        cardFrame->setObjectName("cardFrame");
        cardFrame->setMinimumSize(QSize(860, 520));
        cardFrame->setFrameShape(QFrame::Shape::StyledPanel);
        cardFrame->setFrameShadow(QFrame::Shadow::Raised);
        cardLayout = new QHBoxLayout(cardFrame);
        cardLayout->setSpacing(24);
        cardLayout->setObjectName("cardLayout");
        cardLayout->setContentsMargins(26, 26, 26, 26);
        leftPanel = new QFrame(cardFrame);
        leftPanel->setObjectName("leftPanel");
        leftPanel->setMinimumSize(QSize(350, 0));
        leftPanel->setFrameShape(QFrame::Shape::NoFrame);
        leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setSpacing(14);
        leftLayout->setObjectName("leftLayout");
        leftLayout->setContentsMargins(24, 24, 24, 24);
        titleLabel = new QLabel(leftPanel);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setWordWrap(true);

        leftLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(leftPanel);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setWordWrap(true);

        leftLayout->addWidget(subtitleLabel);

        hintLabel = new QLabel(leftPanel);
        hintLabel->setObjectName("hintLabel");

        leftLayout->addWidget(hintLabel);

        weatherHostFrame = new QFrame(leftPanel);
        weatherHostFrame->setObjectName("weatherHostFrame");
        weatherHostFrame->setMinimumSize(QSize(0, 210));
        QFont font;
        font.setBold(true);
        weatherHostFrame->setFont(font);
        weatherHostFrame->setAutoFillBackground(false);
        weatherHostFrame->setFrameShape(QFrame::Shape::NoFrame);

        leftLayout->addWidget(weatherHostFrame);

        verticalSpacerLeft = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        leftLayout->addItem(verticalSpacerLeft);


        cardLayout->addWidget(leftPanel);

        rightPanel = new QFrame(cardFrame);
        rightPanel->setObjectName("rightPanel");
        rightPanel->setFrameShape(QFrame::Shape::NoFrame);
        rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->setSpacing(12);
        rightLayout->setObjectName("rightLayout");
        rightLayout->setContentsMargins(4, 10, 4, 10);
        formTitleLabel = new QLabel(rightPanel);
        formTitleLabel->setObjectName("formTitleLabel");

        rightLayout->addWidget(formTitleLabel);

        userFieldLabel = new QLabel(rightPanel);
        userFieldLabel->setObjectName("userFieldLabel");
        userFieldLabel->setStyleSheet(QString::fromUtf8("color: #31465b; font-size: 13px; font-weight: 600;"));

        rightLayout->addWidget(userFieldLabel);

        usernameLineEdit = new QLineEdit(rightPanel);
        usernameLineEdit->setObjectName("usernameLineEdit");

        rightLayout->addWidget(usernameLineEdit);

        passwordFieldLabel = new QLabel(rightPanel);
        passwordFieldLabel->setObjectName("passwordFieldLabel");
        passwordFieldLabel->setStyleSheet(QString::fromUtf8("color: #31465b; font-size: 13px; font-weight: 600;"));

        rightLayout->addWidget(passwordFieldLabel);

        passwordLineEdit = new QLineEdit(rightPanel);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        rightLayout->addWidget(passwordLineEdit);

        statusLabel = new QLabel(rightPanel);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setWordWrap(true);

        rightLayout->addWidget(statusLabel);

        loginButton = new QPushButton(rightPanel);
        loginButton->setObjectName("loginButton");
        loginButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        rightLayout->addWidget(loginButton);


        cardLayout->addWidget(rightPanel);


        horizontalLayout->addWidget(cardFrame);

        horizontalSpacerRight = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacerRight);


        retranslateUi(LoginForm);

        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QWidget *LoginForm)
    {
        LoginForm->setWindowTitle(QCoreApplication::translate("LoginForm", "Login", nullptr));
        titleLabel->setText(QCoreApplication::translate("LoginForm", "Login", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("LoginForm", "Autenticacion, red, cache local y una experiencia cuidada para presentar en Qt.", nullptr));
        hintLabel->setText(QCoreApplication::translate("LoginForm", "Temperatura y hora local de Cordoba", nullptr));
        formTitleLabel->setText(QCoreApplication::translate("LoginForm", "Iniciar sesion", nullptr));
        userFieldLabel->setText(QCoreApplication::translate("LoginForm", "Usuario", nullptr));
        usernameLineEdit->setPlaceholderText(QCoreApplication::translate("LoginForm", "Ingresa admin", nullptr));
        passwordFieldLabel->setText(QCoreApplication::translate("LoginForm", "Contrasena", nullptr));
        passwordLineEdit->setPlaceholderText(QCoreApplication::translate("LoginForm", "Ingresa 1234", nullptr));
        statusLabel->setText(QString());
        loginButton->setText(QCoreApplication::translate("LoginForm", "Ingresar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
