# Ejercicio 09 - Qt + SQLite

Resolucion del ejercicio `Clase14.rst`:

- Login hecho con Qt Designer (`login.ui`)
- Validacion de usuarios en SQLite
- Registro de accesos exitosos y fallidos sin guardar contrasenas en logs
- Clase `Pintura` derivada de `QWidget`
- Cambio de grosor con rueda del mouse
- Cambio de color con teclas `R`, `G` y `B`
- Borrado total con `Escape`
- Deshacer con `Ctrl+Z` limitado a las ultimas 10 acciones
- Persistencia de trazos y coordenadas para reconstruccion del dibujo

## Relacion con `ejercicio4_Bianconi`

Se reutilizo la idea de la tabla `usuarios` con estos campos:

- `username`
- `password`
- `nombre`
- `apellido`
- `email`
- `activo`

En esta version se usa SQLite y la clave se guarda como hash SHA-256.

## Credenciales de prueba

- `cbianconi / 123`
- `mlopez / poo2026`

## Como abrirlo

1. Abrir `ejercicio09_qt.pro` en Qt Creator.
2. Compilar y ejecutar.
3. La base SQLite se crea sola en `data/ejercicio09.sqlite` junto al ejecutable.

## Archivo SQL

El esquema de referencia tambien queda en `db/init.sql`.
