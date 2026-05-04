#include "appcontroller.h"

#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("EditorMultilenguaje");
    app.setOrganizationName("UBP");
    app.setFont(QFont("Aptos", 10));

    AppController controller;
    controller.start();

    return app.exec();
}
