#include "appcontroller.h"
#include "models.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    QApplication::setApplicationName("ejercicio7-qt");
    QApplication::setOrganizationName("UBP");
    QApplication::setApplicationVersion("1.0");

    qRegisterMetaType<WeatherInfo>("WeatherInfo");

    AppController controller;
    controller.start();

    return app.exec();
}
