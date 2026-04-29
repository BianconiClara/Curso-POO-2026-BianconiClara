# Ejercicio 08 - Editor multilenguaje en Qt

Resolucion completa de la consigna en `ejercicio8/codigo`, siguiendo el enfoque visto en clase con:

- `Pantalla` abstracta y uso real de polimorfismo.
- signals/slots para coordinar la interfaz.
- validadores de sintaxis por jerarquia (`ValidadorSintaxis`, `ValidadorCpp`, `ValidadorPython`, `ValidadorJava`).
- configuracion leida desde archivo `INI`.
- registro de eventos en archivo de log.
- funcionamiento offline, sin servicios remotos.

## Ubicacion del ejercicio

Todo el codigo fuente del ejercicio esta en:

- `codigo/`

## Credenciales iniciales

Se leen desde `codigo/config/app.ini`.

Valores por defecto:

- Usuario: `admin`
- Clave: `1234`

## Requisitos de la consigna cubiertos

1. Login inicial con bloqueo temporal luego de 3 intentos fallidos.
2. Clase base abstracta `Pantalla` con metodos virtuales puros:
   `inicializarUI()`, `conectarEventos()`, `cargarDatos()`, `validarEstado()` y `registrarEvento()`.
3. Pantallas derivadas concretas:
   `LoginScreen`, `EditorPrincipal` y `ModoBloqueado`.
4. Flujo controlado por `AppController` usando punteros a la clase base `Pantalla`.
5. Selector de lenguaje con soporte para `C++`, `Python` y `Java`.
6. Validacion de sintaxis por linea al abandonar la linea editada.
7. Resaltado de errores en rojo y mensaje diagnostico amigable en UI.
8. Redefinicion de eventos:
   `keyPressEvent`, `mousePressEvent`, `resizeEvent`, `closeEvent`, `focusInEvent` y `focusOutEvent`.
9. Registro de eventos relevantes en `codigo/logs/editor_multilenguaje.log`.
10. Lectura de configuracion desde `codigo/config/app.ini`.
11. Funcionamiento offline.
12. Apertura de la ventana principal en full screen despues del login valido.
13. Exportacion a un unico archivo JPG legible con todo el codigo y sus saltos de linea.
14. Panel lateral estilo LinkedIn con foto, descripcion, habilidades y contacto.

## Estructura principal

```text
codigo/
|-- assets/profile/professional-profile.jpeg
|-- config/app.ini
|-- resources/resources.qrc
|-- editor_multilenguaje.pro
`-- src/
    |-- appconfig.*
    |-- appcontroller.*
    |-- editorprincipal.*
    |-- loginscreen.*
    |-- logger.*
    |-- modobloqueado.*
    |-- models.h
    |-- pantalla.*
    `-- validadorsintaxis.*
```

## Como compilar

El proyecto esta preparado con `qmake` (`.pro`) y Qt Widgets.

Ejemplo general:

```bash
cd codigo
qmake editor_multilenguaje.pro
make
```

En Windows con MinGW suele ser:

```bash
cd codigo
qmake editor_multilenguaje.pro
mingw32-make
```

Tambien se puede abrir directamente `codigo/editor_multilenguaje.pro` desde Qt Creator.

## Configuracion

Archivo:

- `codigo/config/app.ini`

Claves principales:

- `login/usuario_inicial`
- `login/clave_inicial`
- `login/tiempo_bloqueo_segundos`
- `editor/lenguaje_por_defecto`
- `editor/ruta_exportacion`

## Atajos implementados

- `Enter`: enviar login.
- `Escape`: limpiar clave en login.
- `Ctrl + S`: exportar el codigo a JPG.
- `Ctrl + Enter`: validar manualmente la linea actual.
- `Ctrl + L`: enfocar el selector de lenguaje.
- `Ctrl + R`: recargar la plantilla base del lenguaje activo.

## Comportamiento del editor

- La validacion corre cuando se abandona la linea actual.
- Cada error queda resaltado en rojo.
- El diagnostico aparece debajo del editor.
- La exportacion genera un solo JPG dentro de la carpeta configurada.

## Observaciones

- La aplicacion no depende de internet.
- Los logs se guardan en `codigo/logs/`.
- La carpeta de exportacion por defecto es `codigo/exports/`.
