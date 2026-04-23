# Laboratorio de concurrencia con contenedores

## Descripción

Este proyecto implementa un sistema concurrente usando múltiples contenedores Docker y una base de datos PostgreSQL compartida.

Cada contenedor worker:

- lee un registro distinto desde una tabla común
- procesa el dato de forma independiente
- escribe el resultado en una tabla compartida
- registra evidencia en un archivo compartido entre contenedores

El objetivo es demostrar el manejo de concurrencia, exclusión mutua, sincronización y consistencia de datos en un entorno distribuido.

## Arquitectura

El sistema está compuesto por los siguientes servicios:

- **db**: contenedor PostgreSQL que almacena las tablas `input` y `result`
- **init-db**: contenedor que inicializa la base de datos y carga datos de prueba
- **worker**: servicio replicado en múltiples contenedores concurrentes

Todos los workers comparten:

- la misma base de datos PostgreSQL
- un sistema de archivos compartido montado en `/shared`

## Estructura del proyecto

```text
lab-concurrencia/
│
├── compose.yaml
├── init/
│   └── init.sql
├── worker/
│   ├── Dockerfile
│   ├── requirements.txt
│   └── worker.py
└── shared/
```

## Tecnologías usadas

- Docker Compose
- PostgreSQL 16
- Python 3.12
- psycopg
- flock mediante fcntl

## Modelo de datos
```
CREATE TABLE input (
    id SERIAL PRIMARY KEY,
    description TEXT NOT NULL,
    status VARCHAR(20) NOT NULL DEFAULT 'pending'
);
```

```
CREATE TABLE result (
    id SERIAL PRIMARY KEY,
    input_id INT NOT NULL REFERENCES input(id),
    worker_identifier VARCHAR(50) NOT NULL,
    result TEXT NOT NULL,
    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
```

## Estrategia de concurrencia

Para evitar que dos workers procesen el mismo dato, se usa la siguiente estrategia:

- seleccionar un registro pending
- bloquearlo con FOR UPDATE
- ignorar registros ya bloqueados con SKIP LOCKED
- actualizar inmediatamente el estado a in_process

Consulta usada:
```
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
```
NOTA: Esto garantiza que cada worker tome un registro distinto sin interferir con los demás.

## Prevención de duplicados

Se implementó un índice único sobre input_id en la tabla result:
```
CREATE UNIQUE INDEX uq_result_input_id ON result(input_id);
```
Además,la inserción usa:
```
ON CONFLICT (input_id) DO NOTHING
```
Con esto se evita que un mismo registro procesado quede duplicado.

## Manejo del autoincremental

Los identificadores de la tabla result se generan automáticamente por PostgreSQL mediante SERIAL.
No se calcula el último ID manualmente, lo cual evita condiciones de carrera.

## Archivo compartido

Todos los contenedores escriben en shared/results.log.

Para evitar corrupción o mezcla de escrituras, cada worker usa bloqueo exclusivo con fcntl.flock() antes de escribir en el archivo.

## Ejecución

En la carpeta lab-concurrencia levantar el sistema con el siguiente comando
```
docker compose up --build --scale worker=5 --timestamps
```

Para ver estado de los contenedores mientras procesan
```
docker compose ps
```

## Consultas de validación

Ver el estado de las entradas
```
docker compose exec db psql -U postgres -d concurrencia -c "SELECT id, description, status FROM input ORDER BY id;"
```
Ver los resultados insertados
```
docker compose exec db psql -U postgres -d concurrencia -c "SELECT id, input_id, worker_identifier, result, date FROM result ORDER BY id;"
```
Hacer la verificación de no duplicados
```
docker compose exec db psql -U postgres -d concurrencia -c "SELECT input_id, COUNT(*) FROM result GROUP BY input_id HAVING COUNT(*) > 1;"
```
