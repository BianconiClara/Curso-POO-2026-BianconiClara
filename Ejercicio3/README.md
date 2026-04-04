# 🎨 Lienzo Colaborativo en Tiempo Real (Qt + VPS)

Aplicación de escritorio desarrollada en **Qt (C++)** que permite dibujar a mano alzada sobre un lienzo con suavizado avanzado, incorporando sincronización colaborativa mediante un servidor VPS.

---

## 🚀 Características principales

### 🖌️ Sistema de dibujo

* Dibujo libre a mano alzada
* Trazo continuo (sin cortes visibles)
* Suavizado visual (antialiasing + líneas redondeadas)
* Independiente de la velocidad del mouse
* Interpolación de puntos para mejorar la fluidez

### 🖱️ Interacción

* **Click izquierdo (mantener):** dibujar
* **Click derecho (mantener):** borrar (goma)
* **Scroll del mouse:** ajustar grosor del trazo
* **Teclas 1–9:** cambiar color dinámicamente

### 🎨 Sistema de color

Interpolación progresiva entre:

* Color inicial: `RGB(192, 19, 76)`
* Color final: `RGB(24, 233, 199)`

El color cambia en función de la tecla presionada (1 → 9).

---

## 🧠 Arquitectura del sistema


---

## 🌍 Funcionalidad colaborativa

El sistema permite múltiples usuarios trabajando sobre el mismo lienzo.

### 🔄 Sincronización

* Al presionar **Guardar**:

  * Se envían los trazos al servidor
* Al iniciar la aplicación:

  * Se recupera el estado actual del dibujo

### 🧩 Estrategia utilizada

* Modelo **incremental**
* Fusión de trazos (**merge**)
* No se sobrescribe el contenido existente
* Evita pérdida de información

---

## 🖥️ Backend (VPS)

Servidor implementado en Python con Flask.

### Endpoints

#### `POST /save`

Guarda nuevos trazos en el servidor

#### `GET /load`

Devuelve el dibujo actual

### Estrategia de almacenamiento

* Archivo JSON (`drawing.json`)
* Fusión de datos existente + nuevos trazos

---

## ⚙️ Instalación y ejecución

### 🔹 Cliente (Qt)

1. Abrir el proyecto `.pro` en Qt Creator
2. Configurar kit de compilación
3. Ejecutar la aplicación

---

### 🔹 Servidor (VPS)

1. Instalar dependencias:

```bash
pip install flask
```

2. Ejecutar:

```bash
python app.py
```

3. Configurar IP en:

```
syncmanager.cpp
```

---

## 🔧 Configuración importante

Reemplazar en el cliente:

```cpp
http://TU_VPS:5000
```

Por la IP real del servidor.

---

## 🎯 Requisitos cumplidos

✔ Lienzo con `paintEvent`
✔ Dibujo continuo y suavizado
✔ Interpolación de puntos
✔ Control dinámico de grosor
✔ Interpolación de color
✔ Interfaz con toolbar
✔ Guardado en VPS
✔ Recuperación de datos
✔ Sincronización colaborativa
✔ Modelo incremental
✔ Fusión de trazos

