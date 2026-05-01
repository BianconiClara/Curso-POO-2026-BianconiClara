#include "admindb.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>

AdminDB::AdminDB(QObject *parent)
    : QObject(parent),
      connectionName("ejercicio09_connection") {
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }
}

AdminDB::~AdminDB() {
    if (db.isOpen()) {
        db.close();
    }
}

bool AdminDB::conectar(const QString &archivoSqlite) {
    db.setDatabaseName(archivoSqlite);

    if (!db.open()) {
        qWarning() << "No se pudo abrir la base:" << db.lastError();
        return false;
    }

    return crearTablas() && insertarUsuariosSemilla();
}

QSqlDatabase AdminDB::getDB() const {
    return db;
}

Usuario AdminDB::validarUsuario(const QString &username, const QString &password) {
    Usuario usuario;

    if (!db.isOpen()) {
        return usuario;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT id, username, nombre, apellido, email, activo, password "
        "FROM usuarios "
        "WHERE username = :username "
        "LIMIT 1"
    );
    query.bindValue(":username", username.trimmed());

    if (!query.exec()) {
        qWarning() << "Error al validar usuario:" << query.lastError();
        return usuario;
    }

    if (!query.next()) {
        return usuario;
    }

    const QSqlRecord record = query.record();
    const int idCol = record.indexOf("id");
    const int usernameCol = record.indexOf("username");
    const int nombreCol = record.indexOf("nombre");
    const int apellidoCol = record.indexOf("apellido");
    const int emailCol = record.indexOf("email");
    const int activoCol = record.indexOf("activo");
    const int passwordCol = record.indexOf("password");

    const bool activo = query.value(activoCol).toBool();
    const QString passwordGuardado = query.value(passwordCol).toString();

    if (!activo || passwordGuardado != hashPassword(password)) {
        return usuario;
    }

    usuario.id = query.value(idCol).toInt();
    usuario.username = query.value(usernameCol).toString();
    usuario.nombre = query.value(nombreCol).toString();
    usuario.apellido = query.value(apellidoCol).toString();
    usuario.email = query.value(emailCol).toString();
    usuario.activo = activo;

    return usuario;
}

bool AdminDB::registrarAcceso(const QString &username, bool exito, const QString &detalle) {
    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO logs_acceso (username, exito, detalle) "
        "VALUES (:username, :exito, :detalle)"
    );
    query.bindValue(":username", username.trimmed());
    query.bindValue(":exito", exito ? 1 : 0);
    query.bindValue(":detalle", detalle);

    const bool ok = query.exec();
    if (!ok) {
        qWarning() << "Error al registrar acceso:" << query.lastError();
    }

    return ok;
}

QVector<Trazo> AdminDB::cargarTrazos(int userId) {
    QVector<Trazo> trazos;

    QSqlQuery trazosQuery(db);
    trazosQuery.prepare(
        "SELECT id, color, grosor "
        "FROM trazos "
        "WHERE usuario_id = :usuario_id "
        "ORDER BY id"
    );
    trazosQuery.bindValue(":usuario_id", userId);

    if (!trazosQuery.exec()) {
        qWarning() << "Error al cargar trazos:" << trazosQuery.lastError();
        return trazos;
    }

    while (trazosQuery.next()) {
        const QSqlRecord record = trazosQuery.record();
        const int idCol = record.indexOf("id");
        const int colorCol = record.indexOf("color");
        const int grosorCol = record.indexOf("grosor");

        Trazo trazo;
        trazo.id = trazosQuery.value(idCol).toInt();
        trazo.color = QColor(trazosQuery.value(colorCol).toString());
        trazo.grosor = trazosQuery.value(grosorCol).toInt();

        QSqlQuery puntosQuery(db);
        puntosQuery.prepare(
            "SELECT x, y "
            "FROM puntos "
            "WHERE trazo_id = :trazo_id "
            "ORDER BY orden"
        );
        puntosQuery.bindValue(":trazo_id", trazo.id);

        if (!puntosQuery.exec()) {
            qWarning() << "Error al cargar puntos:" << puntosQuery.lastError();
            continue;
        }

        while (puntosQuery.next()) {
            const QSqlRecord puntosRecord = puntosQuery.record();
            const int xCol = puntosRecord.indexOf("x");
            const int yCol = puntosRecord.indexOf("y");
            const int x = puntosQuery.value(xCol).toInt();
            const int y = puntosQuery.value(yCol).toInt();
            trazo.puntos.append(QPoint(x, y));
        }

        if (!trazo.puntos.isEmpty()) {
            trazos.append(trazo);
        }
    }

    return trazos;
}

int AdminDB::guardarTrazo(int userId, const QColor &color, int grosor, const QVector<QPoint> &puntos) {
    if (puntos.isEmpty()) {
        return -1;
    }

    if (!db.transaction()) {
        qWarning() << "No se pudo iniciar la transaccion para guardar el trazo:" << db.lastError();
        return -1;
    }

    QSqlQuery trazoQuery(db);
    trazoQuery.prepare(
        "INSERT INTO trazos (usuario_id, color, grosor) "
        "VALUES (:usuario_id, :color, :grosor)"
    );
    trazoQuery.bindValue(":usuario_id", userId);
    trazoQuery.bindValue(":color", color.name());
    trazoQuery.bindValue(":grosor", grosor);

    if (!trazoQuery.exec()) {
        qWarning() << "Error al guardar trazo:" << trazoQuery.lastError();
        db.rollback();
        return -1;
    }

    const int trazoId = trazoQuery.lastInsertId().toInt();

    QSqlQuery puntoQuery(db);
    puntoQuery.prepare(
        "INSERT INTO puntos (trazo_id, orden, x, y) "
        "VALUES (:trazo_id, :orden, :x, :y)"
    );

    for (int i = 0; i < puntos.size(); ++i) {
        puntoQuery.bindValue(":trazo_id", trazoId);
        puntoQuery.bindValue(":orden", i);
        puntoQuery.bindValue(":x", puntos.at(i).x());
        puntoQuery.bindValue(":y", puntos.at(i).y());

        if (!puntoQuery.exec()) {
            qWarning() << "Error al guardar punto:" << puntoQuery.lastError();
            db.rollback();
            return -1;
        }
    }

    if (!db.commit()) {
        qWarning() << "No se pudo confirmar el guardado del trazo:" << db.lastError();
        db.rollback();
        return -1;
    }

    return trazoId;
}

bool AdminDB::eliminarTrazo(int trazoId) {
    if (!db.transaction()) {
        qWarning() << "No se pudo iniciar la transaccion para eliminar el trazo:" << db.lastError();
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM trazos WHERE id = :id");
    query.bindValue(":id", trazoId);

    if (!query.exec()) {
        qWarning() << "Error al eliminar trazo:" << query.lastError();
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        qWarning() << "No se pudo confirmar la eliminacion del trazo:" << db.lastError();
        db.rollback();
        return false;
    }

    return true;
}

bool AdminDB::borrarTrazosUsuario(int userId) {
    if (!db.transaction()) {
        qWarning() << "No se pudo iniciar la transaccion para borrar el lienzo:" << db.lastError();
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM trazos WHERE usuario_id = :usuario_id");
    query.bindValue(":usuario_id", userId);

    if (!query.exec()) {
        qWarning() << "Error al borrar trazos del usuario:" << query.lastError();
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        qWarning() << "No se pudo confirmar el borrado del lienzo:" << db.lastError();
        db.rollback();
        return false;
    }

    return true;
}

bool AdminDB::crearTablas() {
    const QStringList sentencias = {
        "PRAGMA foreign_keys = ON",
        "CREATE TABLE IF NOT EXISTS usuarios ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL UNIQUE, "
        "password TEXT NOT NULL, "
        "nombre TEXT NOT NULL, "
        "apellido TEXT NOT NULL, "
        "email TEXT NOT NULL, "
        "activo INTEGER NOT NULL DEFAULT 1, "
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP"
        ")",
        "CREATE TABLE IF NOT EXISTS logs_acceso ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL, "
        "exito INTEGER NOT NULL, "
        "detalle TEXT NOT NULL, "
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP"
        ")",
        "CREATE TABLE IF NOT EXISTS trazos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "usuario_id INTEGER NOT NULL, "
        "color TEXT NOT NULL, "
        "grosor INTEGER NOT NULL, "
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY (usuario_id) REFERENCES usuarios(id) ON DELETE CASCADE"
        ")",
        "CREATE TABLE IF NOT EXISTS puntos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "trazo_id INTEGER NOT NULL, "
        "orden INTEGER NOT NULL, "
        "x INTEGER NOT NULL, "
        "y INTEGER NOT NULL, "
        "FOREIGN KEY (trazo_id) REFERENCES trazos(id) ON DELETE CASCADE"
        ")"
    };

    for (const QString &sentencia : sentencias) {
        QSqlQuery query(db);
        if (!query.exec(sentencia)) {
            qWarning() << "Error al crear estructura SQLite:" << query.lastError();
            qWarning() << "Sentencia:" << sentencia;
            return false;
        }
    }

    return true;
}

bool AdminDB::insertarUsuariosSemilla() {
    struct UsuarioSemilla {
        QString username;
        QString passwordPlano;
        QString nombre;
        QString apellido;
        QString email;
    };

    const QVector<UsuarioSemilla> usuarios = {
        {"cbianconi", "123", "Clara", "Bianconi", "cbianconi@example.com"},
        {"mlopez", "poo2026", "Maria", "Lopez", "mlopez@example.com"}
    };

    if (!db.transaction()) {
        qWarning() << "No se pudo iniciar la transaccion para usuarios semilla:" << db.lastError();
        return false;
    }

    QSqlQuery migracionQuery(db);
    migracionQuery.prepare(
        "UPDATE usuarios "
        "SET username = :nuevo_username, "
        "    password = :password, "
        "    nombre = :nombre, "
        "    apellido = :apellido, "
        "    email = :email, "
        "    activo = 1 "
        "WHERE username = :username_anterior "
        "  AND NOT EXISTS ("
        "      SELECT 1 FROM usuarios WHERE username = :nuevo_username"
        "  )"
    );
    migracionQuery.bindValue(":nuevo_username", "cbianconi");
    migracionQuery.bindValue(":password", hashPassword("123"));
    migracionQuery.bindValue(":nombre", "Clara");
    migracionQuery.bindValue(":apellido", "Bianconi");
    migracionQuery.bindValue(":email", "cbianconi@example.com");
    migracionQuery.bindValue(":username_anterior", "cponce");

    if (!migracionQuery.exec()) {
        qWarning() << "No se pudo migrar el usuario semilla:" << migracionQuery.lastError();
        db.rollback();
        return false;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare(
        "INSERT OR IGNORE INTO usuarios (username, password, nombre, apellido, email, activo) "
        "VALUES (:username, :password, :nombre, :apellido, :email, 1)"
    );

    QSqlQuery updateQuery(db);
    updateQuery.prepare(
        "UPDATE usuarios "
        "SET password = :password, "
        "    nombre = :nombre, "
        "    apellido = :apellido, "
        "    email = :email, "
        "    activo = 1 "
        "WHERE username = :username"
    );

    for (const UsuarioSemilla &usuario : usuarios) {
        insertQuery.bindValue(":username", usuario.username);
        insertQuery.bindValue(":password", hashPassword(usuario.passwordPlano));
        insertQuery.bindValue(":nombre", usuario.nombre);
        insertQuery.bindValue(":apellido", usuario.apellido);
        insertQuery.bindValue(":email", usuario.email);

        if (!insertQuery.exec()) {
            qWarning() << "No se pudo insertar usuario semilla:" << insertQuery.lastError();
            db.rollback();
            return false;
        }

        updateQuery.bindValue(":username", usuario.username);
        updateQuery.bindValue(":password", hashPassword(usuario.passwordPlano));
        updateQuery.bindValue(":nombre", usuario.nombre);
        updateQuery.bindValue(":apellido", usuario.apellido);
        updateQuery.bindValue(":email", usuario.email);

        if (!updateQuery.exec()) {
            qWarning() << "No se pudo actualizar usuario semilla:" << updateQuery.lastError();
            db.rollback();
            return false;
        }
    }

    if (!db.commit()) {
        qWarning() << "No se pudo confirmar usuarios semilla:" << db.lastError();
        db.rollback();
        return false;
    }

    return true;
}

QString AdminDB::hashPassword(const QString &password) const {
    const QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}
