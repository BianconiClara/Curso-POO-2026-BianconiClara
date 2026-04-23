from datetime import datetime
from pydantic import BaseModel
from typing import List, Optional

class LoginRequest(BaseModel):
    username: str
    password: str

class LoginResponse(BaseModel):
    access_token: str
    token_type: str
    user: dict

class ColumnBase(BaseModel):
    name: str
    position: int

class Column(ColumnBase):
    id: int
    created_at: datetime
    updated_at: datetime

class CardBase(BaseModel):
    title: str
    description: Optional[str] = None

class Card(CardBase):
    id: int
    created_at: datetime
    updated_at: datetime

class ColumnWithCards(Column):
    cards: List[Card] = []

class CreateCardRequest(BaseModel):
    title: str
    description: Optional[str] = None
    column_id: int

class MoveCardRequest(BaseModel):
    column_id: int
    position: int

class ReorderCardsRequest(BaseModel):
    card_ids: List[int]
