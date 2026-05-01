# Ejercicio 09 - Coordenadas en base de datos 🖼️

# Requisitos 📌

- Login hecho con Qt Designer (`login.ui`)
- Validación de usuarios en SQLite
- Registro de accesos exitosos y fallidos sin guardar contraseñas en logs
- Clase `Pintura` derivada de `QWidget`
- Cambio de grosor con rueda del mouse
- Cambio de color con teclas `R`, `G` y `B`
- Borrado total con `Escape`
- Deshacer con `Ctrl+Z` limitado a las últimas 10 acciones
- Persistencia de trazos y coordenadas para reconstrucción del dibujo

## Credenciales de prueba 💁

- `cbianconi / 123`
- `mlopez / poo2026`

## Como abrirlo ⏯️

1. Abrir `ejercicio09_qt.pro` en Qt Creator.
2. Compilar y ejecutar.
3. La base SQLite se crea sola en `data/ejercicio09.sqlite` junto al ejecutable.

## Archivo SQLite 🗂️
- En el archivo init.sql solo se encuentra el esquema de referencia mientras que el documento sqlite se encuentra dentro de  [sqlite](ejercicio9/codigo/build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/debug/data/ejercicio09.sqlite)

## Imágenes y videos del Proyecto
### LOGIN
[video del Login](recursos/login.mp4)
[imagn](recursos/img-login.png)

### PANTALLA PARA DIBUJAR
[video del dibujo](recursos/dibujo.mp4)
[imgen](recursos/img-dibujo.png)
