# Ejercicio 04 - Tablero Kanban colaborativo

Backend FastAPI para tablero Kanban con autenticación y persistencia en MySQL.

## Estructura

- `backend/`: API FastAPI
- `db/`: Scripts de base de datos
- `docker-compose.yml`: Configuración de contenedores

## Inicio rápido

1. Clona o copia el proyecto
2. Ejecuta `docker-compose up --build`
3. La API estará disponible en http://localhost:8000/api

## Endpoints

### Autenticación
- `POST /api/auth/login` - Login
- `GET /api/auth/me` - Usuario actual

### Columnas
- `GET /api/columns` - Listar columnas con tarjetas
- `POST /api/columns` - Crear columna
- `PUT /api/columns/{id}` - Actualizar columna
- `DELETE /api/columns/{id}` - Eliminar columna

### Tarjetas
- `POST /api/cards` - Crear tarjeta
- `PUT /api/cards/{id}` - Actualizar tarjeta
- `DELETE /api/cards/{id}` - Eliminar tarjeta
- `POST /api/cards/{id}/move` - Mover tarjeta entre columnas

### Reordenar
- `POST /api/columns/{id}/reorder` - Reordenar tarjetas en columna

## Base de datos

Tablas:
- `usuarios`: Usuarios para autenticación
- `columns`: Columnas del tablero
- `cards`: Tarjetas
- `card_order`: Orden de tarjetas en columnas