#ifndef ADMINDB_H
#define ADMINDB_H

#include <QColor>
#include <QPoint>
#include <QSqlDatabase>
#include <QString>
#include <QVector>
#include <QObject>

struct Usuario {
    int id = -1;
    QString username;
    QString nombre;
    QString apellido;
    QString email;
    bool activo = false;

    bool esValido() const {
        return id > 0 && activo;
    }
};

struct Trazo {
    int id = -1;
    QColor color = Qt::black;
    int grosor = 2;
    QVector<QPoint> puntos;
};

class AdminDB : public QObject {
    Q_OBJECT

public:
    explicit AdminDB(QObject *parent = nullptr);
    ~AdminDB();

    bool conectar(const QString &archivoSqlite);
    QSqlDatabase getDB() const;

    Usuario validarUsuario(const QString &username, const QString &password);
    bool registrarAcceso(const QString &username, bool exito, const QString &detalle);

    QVector<Trazo> cargarTrazos(int userId);
    int guardarTrazo(int userId, const QColor &color, int grosor, const QVector<QPoint> &puntos);
    bool eliminarTrazo(int trazoId);
    bool borrarTrazosUsuario(int userId);

private:
    bool crearTablas();
    bool insertarUsuariosSemilla();
    QString hashPassword(const QString &password) const;

    QString connectionName;
    QSqlDatabase db;
};

#endif // ADMINDB_H
