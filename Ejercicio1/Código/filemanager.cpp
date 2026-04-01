#include "filemanager.h"
#include "task.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

// ================= LOGIN =================

bool FileManager::validarUsuario(QString user, QString pass)
{
    QString path = "../../usuarios.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No se pudo abrir el archivo de usuarios";
        return false;
    }

    QTextStream in(&file);
    QString data = in.readAll();

    QString patron = "usuario:" + user + ",contraseña:" + pass + ";";

    return data.contains(patron);
}

// ================= SESION =================

void FileManager::guardarSesion(QString user)
{
    QFile file("usuario.txt");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "No se pudo abrir el archivo de usuario";
        return;
    }

    QTextStream out(&file);

    qint64 timestamp = QDateTime::currentSecsSinceEpoch();

    out << "usuario:" << user << ";\n";
    out << "timestamp:" << timestamp << ";\n";
}

bool FileManager::existeSesion()
{
    QFile file("usuario.txt");
    return file.exists();
}

QString FileManager::leerUsuarioSesion()
{
    QFile file("usuario.txt");

    if (!file.open(QIODevice::ReadOnly))
        return "";

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (line.startsWith("usuario:"))
        {
            return line.split(":")[1].replace(";", "");
        }
    }

    return "";
}

qint64 FileManager::leerTimestamp()
{
    QFile file("usuario.txt");

    if (!file.open(QIODevice::ReadOnly))
        return 0;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (line.startsWith("timestamp:"))
        {
            return line.split(":")[1].replace(";", "").toLongLong();
        }
    }

    return 0;
}

// ================= TASKS =================

void FileManager::guardarTasks(QList<Task> lista)
{
    QFile file("tasks.txt");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error al abrir archivo tasks.txt";
        return;
    }

    QTextStream out(&file);

    for (Task t : lista)
        out << t.toString() << "\n";
}

QList<Task> FileManager::leerTasks()
{
    QList<Task> lista;

    QFile file("tasks.txt");

    if (!file.open(QIODevice::ReadOnly))
        return lista;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (!line.isEmpty())
            lista.append(Task::fromString(line));
    }

    return lista;
}

// ================= HISTORIAL =================

void FileManager::guardarHistorial(QString accion, QString nombreTP)
{
    QFile file("historial.txt");

    if (!file.open(QIODevice::Append)) {
        qDebug() << "No se pudo abrir historial.txt";
        return;
    }

    QTextStream out(&file);

    QString fechaHora = QDateTime::currentDateTime()
                            .toString("dd/MM/yyyy HH:mm:ss");

    out << "[" << fechaHora << "] "
        << accion << " TP: \"" << nombreTP << "\"\n";

    file.close();
}