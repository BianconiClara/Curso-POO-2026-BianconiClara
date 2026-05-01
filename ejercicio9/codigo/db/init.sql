PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS usuarios (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    nombre TEXT NOT NULL,
    apellido TEXT NOT NULL,
    email TEXT NOT NULL,
    activo INTEGER NOT NULL DEFAULT 1,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS logs_acceso (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL,
    exito INTEGER NOT NULL,
    detalle TEXT NOT NULL,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS trazos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    usuario_id INTEGER NOT NULL,
    color TEXT NOT NULL,
    grosor INTEGER NOT NULL,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (usuario_id) REFERENCES usuarios(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS puntos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    trazo_id INTEGER NOT NULL,
    orden INTEGER NOT NULL,
    x INTEGER NOT NULL,
    y INTEGER NOT NULL,
    FOREIGN KEY (trazo_id) REFERENCES trazos(id) ON DELETE CASCADE
);

INSERT OR IGNORE INTO usuarios (username, password, nombre, apellido, email, activo)
VALUES
    ('cbianconi', 'a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3', 'Clara', 'Bianconi', 'cbianconi@example.com', 1),
    ('mlopez', '4f34ff56e9e07118812eabf4cd42d49ecadfbfb9a3f087d9349ab5bdc5282ccf', 'Maria', 'Lopez', 'mlopez@example.com', 1);
