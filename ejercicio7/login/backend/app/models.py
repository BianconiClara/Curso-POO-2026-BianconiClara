# Models for Kanban board
# Using raw MySQL queries instead of ORM

# Tables:
# - usuarios: id, username, password, nombre, apellido, email, activo, created_at
# - columns: id, name, position, created_at, updated_at
# - cards: id, title, description, created_at, updated_at
# - card_order: id, column_id, card_id, position
