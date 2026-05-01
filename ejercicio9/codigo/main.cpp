#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include "admindb.h"
#include "login.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QDir dir(QCoreApplication::applicationDirPath());
    dir.mkpath("data");
    const QString dbPath = dir.filePath("data/ejercicio09.sqlite");

    AdminDB adminDB;
    if (!adminDB.conectar(dbPath)) {
        QMessageBox::critical(
            nullptr,
            "Base de datos",
            "No se pudo abrir la base SQLite en:\n" + dbPath
        );
        return 1;
    }

    Login login(&adminDB);
    login.show();

    return app.exec();
}
