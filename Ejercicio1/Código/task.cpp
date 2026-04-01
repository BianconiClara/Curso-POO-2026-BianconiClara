#include "task.h"
#include <QStringList>

Task::Task(QString t, QString e, QString p, QString n)
{
    titulo = t;
    estado = e;
    prioridad = p;
    nota = n;
}

QString Task::toString()
{
    return "titulo:" + titulo +
           ",estado:" + estado +
           ",prioridad:" + prioridad +
           ",nota:" + nota + ";";
}

Task Task::fromString(QString line)
{
    QStringList parts = line.split(",");

    QString t = parts[0].split(":")[1];
    QString e = parts[1].split(":")[1];
    QString p = parts[2].split(":")[1];
    QString n = parts[3].split(":")[1].replace(";", "");

    return Task(t, e, p, n);
}