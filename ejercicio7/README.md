# Ejercicio 07 - Login con clima y fondo de pantalla ⛅

Proyecto de escritorio en Qt Widgets para resolver la consigna de login con clima, imagen de fondo descargada, cache local, proxy, logs y modo offline.

## Qué incluye 📌

- Login hecho con `Qt Designer` en [ui/login.ui](ui/login.ui)
- Usuario demo `admin` con contraseña `1234`
- Bloqueo temporal tras 3 intentos fallidos
- Consulta directa a WeatherAPI usando `QNetworkAccessManager`
- Lectura de API key, ciudad y proxy desde [config/app.ini](config/app.ini)
- Imagen principal descargada desde una URL pública y guardada en caché local
- Soporte offline con datos simulados y fondo de respaldo embebido
- Ventana principal full screen con una tarjeta profesional estilo LinkedIn
- Registro de eventos en archivo de log

## Estructura 🏚️

- `src/`: clases C++
- `ui/`: formulario `.ui` del login
- `resources/`: recursos Qt
- `assets/`: imagen de fondo local y retrato profesional usado en la ventana principal
- `config/`: configuración editable
- `login/`: carpeta previa existente, conservada como base del ejercicio

## Configuracion 🪛

1. Abre [config/app.ini](config/app.ini)
2. Verifica tu API key de WeatherAPI en `config/app.ini`
3. Si usas proxy, completa `host`, `port`, `username` y `password`

Si no hay internet o no hay API key válida, la app cambia automáticamente a modo offline.

## Cómo abrirlo en Qt Creator ⏯️

1. Abre `ejercicio7.pro`
2. Selecciona un kit con `Qt Widgets`
3. Ejecuta el proyecto

## Notas 🗒️

- La imagen descargada se guarda en `AppDataLocation/cache/background-cache.jpg`
- El log se guarda en `AppDataLocation/logs/ejercicio7.log`
- El fondo offline reutiliza la imagen `imagen-fondo.jpg` que ya existia en esta carpeta

## Imagenes 🖼️

