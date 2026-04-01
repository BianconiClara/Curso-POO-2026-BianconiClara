# 🖥️ Panel de Monitoreo VPS – Qt

## 🧾 Descripción

Aplicación de escritorio desarrollada en **Qt (Widgets)** que actúa como un panel de monitoreo para un servidor VPS. La aplicación consume un endpoint del servidor que provee métricas de salud y las presenta en una interfaz clara, organizada y profesional.

---

## 🎯 Objetivo

Diseñar una aplicación que combine:

* Consumo de datos desde un servidor remoto.
* Visualización clara de métricas.
* Interactividad para control de monitoreo.
* Arquitectura modular en Qt.

---

## ⚙️ Funcionalidades

### 📡 Monitoreo del Servidor

* Consumo de endpoint HTTP que devuelve:

  * Uptime
  * Uso de CPU (carga)
  * Memoria utilizada
  * Espacio en disco
  * Estado general del sistema

### 🚦 Estado General

* Indicador visual del estado:

### 📊 Métricas Principales

* Visualización de:

  * CPU
  * RAM
  * Disco
  * Uptime
* Presentadas mediante `QLabel` y organizadas en layout claro.

### ⏱️ Último Chequeo

* Timestamp del último request al servidor.

### 📜 Historial de Eventos

* Registro corto de eventos recientes:

  * Cambios de estado
  * Errores de conexión
  * Alertas

---

## 🔄 Controles de Monitoreo

* Botón de **refresh manual**.
* Configuración de:

  * Intervalo de chequeo automático.
  * Umbral de alerta (por ejemplo, CPU > 80%).

---

## 🧱 Estructura del Proyecto

El sistema está organizado en clases para separar responsabilidades:

* `MonitorManager`

  * Lógica de monitoreo.
  * Requests al endpoint.
  * Procesamiento de datos.
* `DashboardWidget`

  * Interfaz gráfica principal.
* Clases auxiliares

  * Modelos de datos / historial.

> ⚠️ Restricción: No concentrar la lógica en `main.cpp`.

---

## 🌐 Comunicación con el Servidor

* Uso de requests HTTP (por ejemplo con `QNetworkAccessManager`).
* Endpoint devuelve datos en formato JSON.

Ejemplo:

```json
{
  "status": "OK",
  "cpu": 35,
  "memory": 60,
  "disk": 70,
  "uptime": "48h"
}
```

---

## 🛠️ Tecnologías Utilizadas

* C++
* Qt (Widgets)
* QMake (Empty Project)
* HTTP / JSON

---


## ▶️ Ejecución

1. Abrir el proyecto en Qt Creator.
2. Configurar el endpoint del servidor.
3. Compilar el proyecto.
4. Ejecutar la aplicación.
5. Monitorear el estado del VPS en tiempo real.

---
## 📷 Capturas Solicitadas

