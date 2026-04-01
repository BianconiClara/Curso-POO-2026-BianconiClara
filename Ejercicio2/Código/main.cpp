
#include "monitorwidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MonitorWidget w;
    w.setWindowTitle("VPS Dashboard - Monitor");

    w.show();

    return a.exec();
}