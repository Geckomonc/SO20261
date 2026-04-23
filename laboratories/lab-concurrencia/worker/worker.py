import os
import time
import random
import socket
from datetime import datetime

import psycopg
import fcntl


DB_HOST = os.getenv("DB_HOST", "db")
DB_PORT = os.getenv("DB_PORT", "5432")
DB_NAME = os.getenv("DB_NAME", "concurrencia")
DB_USER = os.getenv("DB_USER", "postgres")
DB_PASSWORD = os.getenv("DB_PASSWORD", "postgres")
SHARED_FILE = os.getenv("SHARED_FILE", "/shared/results.log")
WORKER_ID = socket.gethostname()


def log(message: str) -> None:
    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    print(f"{now} [{WORKER_ID}] {message}", flush=True)


def append_shared_file(line: str) -> None:
    os.makedirs(os.path.dirname(SHARED_FILE), exist_ok=True)

    with open(SHARED_FILE, "a", encoding="utf-8") as file:
        fcntl.flock(file.fileno(), fcntl.LOCK_EX)
        file.write(line + "\n")
        file.flush()
        os.fsync(file.fileno())
        fcntl.flock(file.fileno(), fcntl.LOCK_UN)


def get_connection():
    return psycopg.connect(
        host=DB_HOST,
        port=DB_PORT,
        dbname=DB_NAME,
        user=DB_USER,
        password=DB_PASSWORD,
    )


def wait_until_ready() -> None:
    while True:
        try:
            with get_connection() as conn:
                with conn.cursor() as cur:
                    cur.execute("SELECT to_regclass('public.input');")
                    table_name = cur.fetchone()[0]
                    if table_name == "input":
                        cur.execute("SELECT COUNT(*) FROM input;")
                        cur.fetchone()
                        log("Base de datos lista.")
                        return
        except Exception as error:
            log(f"Esperando base de datos: {error}")

        time.sleep(2)


def claim_job(conn):
    with conn.cursor() as cur:
        cur.execute(
            """
            WITH picked AS (
                SELECT id, description
                FROM input
                WHERE status = 'pending'
                ORDER BY id
                LIMIT 1
                FOR UPDATE SKIP LOCKED
            )
            UPDATE input AS i
            SET status = 'in_process'
            FROM picked
            WHERE i.id = picked.id
            RETURNING picked.id, picked.description;
            """
        )
        row = cur.fetchone()
        conn.commit()
        return row


def save_result(conn, input_id: int, processed_text: str):
    with conn.cursor() as cur:
        cur.execute(
            """
            INSERT INTO result (input_id, worker_identifier, result)
            VALUES (%s, %s, %s)
            ON CONFLICT (input_id) DO NOTHING
            RETURNING id, date;
            """,
            (input_id, WORKER_ID, processed_text),
        )
        inserted = cur.fetchone()

        cur.execute(
            """
            UPDATE input
            SET status = 'processed'
            WHERE id = %s;
            """,
            (input_id,),
        )

        conn.commit()
        return inserted


def main():
    wait_until_ready()

    while True:
        try:
            with get_connection() as conn:
                job = claim_job(conn)

                if not job:
                    log("No hay más registros pendientes. Finalizando.")
                    break

                input_id, description = job
                log(f"Tomé input_id={input_id}, description='{description}'")

                process_time = random.uniform(1.0, 4.0)
                time.sleep(process_time)

                processed_text = f"Procesado por {WORKER_ID}: {description.upper()}"

                result_info = save_result(conn, input_id, processed_text)

                if result_info:
                    result_id, result_date = result_info
                    log(
                        f"Inserté result_id={result_id} para input_id={input_id} en {result_date}"
                    )
                else:
                    log(f"input_id={input_id} ya tenía resultado. No se duplicó.")

                append_shared_file(
                    f"{datetime.now().isoformat()} | worker={WORKER_ID} | input_id={input_id} | result={processed_text}"
                )

        except Exception as error:
            log(f"Error: {error}")
            time.sleep(2)


if __name__ == "__main__":
    main()