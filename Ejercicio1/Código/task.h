#ifndef TASK_H
#define TASK_H

#include <QString>

class Task
{
public:
    QString titulo;
    QString estado;
    QString prioridad;
    QString nota;

    Task(QString t, QString e, QString p, QString n);

    QString toString(); // para guardar en archivo
    static Task fromString(QString line);
};

#endif