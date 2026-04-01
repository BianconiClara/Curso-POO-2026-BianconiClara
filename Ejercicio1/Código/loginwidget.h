#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>


class LoginWidget : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *userEdit;
    QLineEdit *passEdit;
    QPushButton *loginBtn;

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginExitoso(QString usuario);

private slots:
    void intentarLogin();
};

#endif
