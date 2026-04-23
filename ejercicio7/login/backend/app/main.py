from datetime import datetime, timedelta, timezone
import os
from typing import List

from fastapi import FastAPI, HTTPException, Request, status, Depends
from fastapi.middleware.cors import CORSMiddleware

from auth import get_current_user, get_user_by_username, verify_password, create_access_token
from crud import (
    get_columns, create_column, update_column, delete_column,
    create_card, update_card, delete_card, move_card, reorder_cards
)
from schemas import (
    LoginRequest, LoginResponse, Column, ColumnWithCards, ColumnBase,
    CreateCardRequest, CardBase, Card, MoveCardRequest, ReorderCardsRequest
)

app = FastAPI(title="Kanban API", root_path="/api")
JWT_SECRET = os.getenv("JWT_SECRET", "dev_secret_change_me")
JWT_EXPIRE_MINUTES = int(os.getenv("JWT_EXPIRE_MINUTES", "60"))

cors_origins = [o.strip() for o in os.getenv("CORS_ORIGINS", "").split(",") if o.strip()]
if not cors_origins:
    cors_origins = ["*"]

app.add_middleware(
    CORSMiddleware,
    allow_origins=cors_origins,
    allow_credentials=False,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/health")
def health():
    return {"status": "ok"}

@app.post("/auth/login", response_model=LoginResponse)
def login(payload: LoginRequest):
    user = get_user_by_username(payload.username)
    if not user or not verify_password(payload.password, user["password"]) or not user["activo"]:
        raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail="Credenciales invalidas")

    token_payload = {
        "sub": user["username"],
        "user_id": user["id"],
        "nombre": user["nombre"],
        "apellido": user["apellido"],
        "email": user["email"],
    }
    access_token = create_access_token(token_payload)

    return {
        "access_token": access_token,
        "token_type": "bearer",
        "user": {
            "id": user["id"],
            "username": user["username"],
            "nombre": user["nombre"],
            "apellido": user["apellido"],
            "email": user["email"],
        },
    }

@app.get("/auth/me")
def me(current_user: dict = Depends(get_current_user)):
    return {
        "username": current_user.get("sub"),
        "nombre": current_user.get("nombre"),
        "apellido": current_user.get("apellido"),
        "email": current_user.get("email"),
    }

# Kanban endpoints

@app.get("/columns", response_model=List[ColumnWithCards])
def read_columns(current_user: dict = Depends(get_current_user)):
    return get_columns()

@app.post("/columns", response_model=Column)
def create_new_column(column: ColumnBase, current_user: dict = Depends(get_current_user)):
    result = create_column(column.name, column.position)
    if not result:
        raise HTTPException(status_code=400, detail="Error creating column")
    return result

@app.put("/columns/{column_id}", response_model=Column)
def update_existing_column(column_id: int, column: ColumnBase, current_user: dict = Depends(get_current_user)):
    result = update_column(column_id, column.name, column.position)
    if not result:
        raise HTTPException(status_code=404, detail="Column not found")
    return result

@app.delete("/columns/{column_id}")
def delete_existing_column(column_id: int, current_user: dict = Depends(get_current_user)):
    if not delete_column(column_id):
        raise HTTPException(status_code=404, detail="Column not found")
    return {"message": "Column deleted"}

@app.post("/cards", response_model=Card)
def create_new_card(card: CreateCardRequest, current_user: dict = Depends(get_current_user)):
    result = create_card(card)
    if not result:
        raise HTTPException(status_code=404, detail="Column not found")
    return result

@app.put("/cards/{card_id}", response_model=Card)
def update_existing_card(card_id: int, card: CardBase, current_user: dict = Depends(get_current_user)):
    result = update_card(card_id, card.title, card.description)
    if not result:
        raise HTTPException(status_code=404, detail="Card not found")
    return result

@app.delete("/cards/{card_id}")
def delete_existing_card(card_id: int, current_user: dict = Depends(get_current_user)):
    if not delete_card(card_id):
        raise HTTPException(status_code=404, detail="Card not found")
    return {"message": "Card deleted"}

@app.post("/cards/{card_id}/move")
def move_existing_card(card_id: int, move: MoveCardRequest, current_user: dict = Depends(get_current_user)):
    if not move_card(card_id, move):
        raise HTTPException(status_code=404, detail="Card or column not found")
    return {"message": "Card moved"}

@app.post("/columns/{column_id}/reorder")
def reorder_column_cards(column_id: int, reorder: ReorderCardsRequest, current_user: dict = Depends(get_current_user)):
    if not reorder_cards(column_id, reorder):
        raise HTTPException(status_code=404, detail="Column not found")
    return {"message": "Cards reordered"}
