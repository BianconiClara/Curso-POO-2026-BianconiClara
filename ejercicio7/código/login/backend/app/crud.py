from typing import List
from database import get_db
from schemas import Column, ColumnWithCards, Card, CreateCardRequest, MoveCardRequest, ReorderCardsRequest

def get_columns() -> List[ColumnWithCards]:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        # Get columns ordered by position
        cursor.execute("SELECT id, name, position, created_at, updated_at FROM columns ORDER BY position")
        columns = cursor.fetchall()
        
        for col in columns:
            # Get cards for this column ordered by position
            cursor.execute("""
                SELECT c.id, c.title, c.description, c.created_at, c.updated_at
                FROM cards c
                JOIN card_order co ON c.id = co.card_id
                WHERE co.column_id = %s
                ORDER BY co.position
            """, (col['id'],))
            col['cards'] = cursor.fetchall()
        
        return columns
    finally:
        conn.close()

def create_column(name: str, position: int) -> Column:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        cursor.execute(
            "INSERT INTO columns (name, position) VALUES (%s, %s)",
            (name, position)
        )
        conn.commit()
        column_id = cursor.lastrowid
        cursor.execute("SELECT id, name, position, created_at, updated_at FROM columns WHERE id = %s", (column_id,))
        return cursor.fetchone()
    finally:
        conn.close()

def update_column(column_id: int, name: str, position: int) -> Column:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        cursor.execute(
            "UPDATE columns SET name = %s, position = %s WHERE id = %s",
            (name, position, column_id)
        )
        if cursor.rowcount == 0:
            return None
        conn.commit()
        cursor.execute("SELECT id, name, position, created_at, updated_at FROM columns WHERE id = %s", (column_id,))
        return cursor.fetchone()
    finally:
        conn.close()

def delete_column(column_id: int) -> bool:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        cursor.execute("DELETE FROM columns WHERE id = %s", (column_id,))
        if cursor.rowcount == 0:
            return False
        conn.commit()
        return True
    finally:
        conn.close()

def create_card(card: CreateCardRequest) -> Card:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        # Check if column exists
        cursor.execute("SELECT id FROM columns WHERE id = %s", (card.column_id,))
        if not cursor.fetchone():
            return None
        
        # Insert card
        cursor.execute(
            "INSERT INTO cards (title, description) VALUES (%s, %s)",
            (card.title, card.description)
        )
        card_id = cursor.lastrowid
        
        # Get max position in column
        cursor.execute("SELECT MAX(position) FROM card_order WHERE column_id = %s", (card.column_id,))
        max_pos_row = cursor.fetchone()
        max_pos = max_pos_row['MAX(position)'] if max_pos_row['MAX(position)'] else 0
        position = max_pos + 1
        
        # Insert into card_order
        cursor.execute(
            "INSERT INTO card_order (column_id, card_id, position) VALUES (%s, %s, %s)",
            (card.column_id, card_id, position)
        )
        
        conn.commit()
        cursor.execute("SELECT id, title, description, created_at, updated_at FROM cards WHERE id = %s", (card_id,))
        return cursor.fetchone()
    finally:
        conn.close()

def update_card(card_id: int, title: str, description: str) -> Card:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        cursor.execute(
            "UPDATE cards SET title = %s, description = %s WHERE id = %s",
            (title, description, card_id)
        )
        if cursor.rowcount == 0:
            return None
        conn.commit()
        cursor.execute("SELECT id, title, description, created_at, updated_at FROM cards WHERE id = %s", (card_id,))
        return cursor.fetchone()
    finally:
        conn.close()

def delete_card(card_id: int) -> bool:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        # Delete from card_order first due to foreign key
        cursor.execute("DELETE FROM card_order WHERE card_id = %s", (card_id,))
        cursor.execute("DELETE FROM cards WHERE id = %s", (card_id,))
        if cursor.rowcount == 0:
            return False
        conn.commit()
        return True
    finally:
        conn.close()

def move_card(card_id: int, move: MoveCardRequest) -> bool:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        # Check if card exists
        cursor.execute("SELECT id FROM cards WHERE id = %s", (card_id,))
        if not cursor.fetchone():
            return False
        
        # Check if column exists
        cursor.execute("SELECT id FROM columns WHERE id = %s", (move.column_id,))
        if not cursor.fetchone():
            return False
        
        # Get current column and position
        cursor.execute("SELECT column_id, position FROM card_order WHERE card_id = %s", (card_id,))
        current = cursor.fetchone()
        if current:
            # Remove from current column, shift positions down
            cursor.execute("UPDATE card_order SET position = position - 1 WHERE column_id = %s AND position > %s", (current['column_id'], current['position']))
        
        # Update position in new column, shift others up
        cursor.execute("UPDATE card_order SET position = position + 1 WHERE column_id = %s AND position >= %s", (move.column_id, move.position))
        cursor.execute("UPDATE card_order SET column_id = %s, position = %s WHERE card_id = %s", (move.column_id, move.position, card_id))
        
        conn.commit()
        return True
    finally:
        conn.close()

def reorder_cards(column_id: int, reorder: ReorderCardsRequest) -> bool:
    conn = get_db()
    try:
        cursor = conn.cursor(dictionary=True)
        # Check if column exists
        cursor.execute("SELECT id FROM columns WHERE id = %s", (column_id,))
        if not cursor.fetchone():
            return False
        
        # Update positions
        for pos, card_id in enumerate(reorder.card_ids, start=1):
            cursor.execute("UPDATE card_order SET position = %s WHERE column_id = %s AND card_id = %s", (pos, column_id, card_id))
        
        conn.commit()
        return True
    finally:
        conn.close()
