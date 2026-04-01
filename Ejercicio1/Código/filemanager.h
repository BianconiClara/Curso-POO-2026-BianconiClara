#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QList>
#include "task.h"

class FileManager
{
public:
    // 🔐 Login
    static bool validarUsuario(QString user, QString pass);

    // 🧠 Sesión
    static void guardarSesion(QString user);
    static bool existeSesion();
    static QString leerUsuarioSesion();
    static qint64 leerTimestamp();

    // 📋 Trabajos prácticos
    static void guardarTasks(QList<Task> lista);
    static QList<Task> leerTasks();

    // 📜 Historial
    static void guardarHistorial(QString accion, QString nombreTP);
};

#endif