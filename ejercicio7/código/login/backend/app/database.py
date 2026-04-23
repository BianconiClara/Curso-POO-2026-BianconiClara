import os
import mysql.connector
from mysql.connector import MySQLConnection

def get_db() -> MySQLConnection:
    config = {
        "host": os.getenv("DB_HOST", "db"),
        "port": int(os.getenv("DB_PORT", "3306")),
        "user": os.getenv("DB_USER", "poo_user"),
        "password": os.getenv("DB_PASSWORD", "poo_pass"),
        "database": os.getenv("DB_NAME", "kanban"),
    }
    return mysql.connector.connect(**config)
