#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

#include "admindb.h"

namespace Ui {
class Login;
}

class Pintura;

class Login : public QWidget {
    Q_OBJECT

public:
    explicit Login(AdminDB *adminDB, QWidget *parent = nullptr);
    ~Login();

private slots:
    void slotValidar();

private:
    Ui::Login *ui;
    AdminDB *adminDB;
    Pintura *pintura;
};

#endif // LOGIN_H
