#include <QApplication>
#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AppController controller;
    controller.start();

    return a.exec();
}