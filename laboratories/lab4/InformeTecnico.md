# Informe técnico - Laboratorio de concurrencia con contenedores

## 1. Introducción

En este laboratorio se implementó un sistema concurrente basado en múltiples contenedores que comparten una misma base de datos PostgreSQL y un sistema de archivos común. El propósito fue evaluar problemas clásicos de concurrencia, como condiciones de carrera, duplicidad de registros, colisiones de escritura y consistencia de datos.

El sistema se diseñó de manera que varios contenedores worker ejecutaran tareas simultáneamente, tomando registros pendientes desde una tabla de entrada, procesándolos de forma independiente y almacenando los resultados en una tabla común.

## 2. Arquitectura del sistema

La solución fue construida con Docker Compose y está compuesta por tres tipos de servicios:

- Un contenedor de base de datos PostgreSQL
- Un contenedor de inicialización que crea las tablas y carga datos de prueba
- Múltiples contenedores worker que ejecutan el procesamiento concurrente

Todos los workers comparten dos recursos principales:

- La misma base de datos
- Un directorio compartido montado como volumen para registrar evidencias en archivo

## 3. Reparto del trabajo entre contenedores

Cada worker ejecuta el mismo programa, pero el reparto del trabajo se hace dinámicamente desde la base de datos.

Cuando un worker necesita procesar un nuevo registro, ejecuta una transacción que selecciona una fila con estado `pending`, la bloquea y la marca inmediatamente como `in_process`. Para esto se usó `FOR UPDATE SKIP LOCKED`.

Con este mecanismo, si un worker ya bloqueó una fila, los demás la omiten y continúan buscando otra disponible. Así se evita que dos contenedores procesen el mismo dato.

## 4. Mecanismo para evitar condiciones de carrera

El principal riesgo del laboratorio era que dos workers leyeran el mismo registro pendiente al mismo tiempo y ambos intentaran insertarlo en la tabla de resultados.

Para evitar esto, se aplicaron dos niveles de protección:

### 4.1 Control transaccional en la lectura
Se usó la sentencia:
```sql
FOR UPDATE SKIP LOCKED
```
Esto permitió bloquear la fila seleccionada dentro de la transacción y evitar que otro contenedor pudiera reclamarla simultáneamente.

### 4.2 Restricción de unicidad en la tabla de resultados

La tabla result tiene una restricción única sobre input_id, lo que garantiza que un mismo dato de entrada no pueda insertarse dos veces.

De forma complementaria, la inserción se realizó con:
```sql
ON CONFLICT (input_id) DO NOTHING
```
Así, incluso si ocurriera un intento duplicado, la base de datos lo rechaza sin corromper la consistencia.

### 5. Manejo autoincremental de la base de datos

El identificador principal de la tabla result fue delegado completamente al motor PostgreSQL mediante el tipo SERIAL.

Esto es importante porque evita una condición de carrera muy común: calcular manualmente el siguiente ID consultando el último valor almacenado. Al dejar esta responsabilidad en la base de datos, cada inserción obtiene un identificador único y consistente.

### 6. Manejo de concurrencia sobre el sistema de archivos

Además de la base de datos, todos los contenedores compartieron un mismo archivo de salida para registrar evidencia de procesamiento.

Como varios workers podían intentar escribir al mismo tiempo, se utilizó un mecanismo de exclusión mutua mediante fcntl.flock() con bloqueo exclusivo. De esta manera, cada proceso obtiene acceso exclusivo al archivo durante la escritura, evitando mezcla de líneas o corrupción del contenido.

### 7. Resultados obtenidos

Durante la ejecución se levantaron cinco contenedores worker concurrentes. Cada uno procesó registros distintos desde la tabla input, generó resultados en la tabla result y escribió eventos en el archivo compartido.

Los resultados finales mostraron que:

- Todos los registros pendientes fueron procesados
- No se presentaron duplicados en la tabla result
- Los IDs fueron generados automáticamente por PostgreSQL
- Los logs evidenciaron ejecución simultánea mediante timestamps intercalados
- el archivo compartido registró correctamente las operaciones de múltiples workers

### 8. Dificultades encontradas

Las principales dificultades encontradas fueron:

- Coordinar el inicio de los workers con la disponibilidad real de la base de datos
- Garantizar que dos contenedores no leyeran el mismo registro pendiente
- Asegurar escrituras seguras sobre un archivo compartido
- Validar correctamente la ausencia de duplicados e inconsistencias al finalizar

Estas dificultades se resolvieron mediante healthchecks, transacciones con bloqueo pesimista, restricciones de unicidad y bloqueo de archivo.

### 9. Conclusiones

El laboratorio permitió comprobar que la concurrencia en sistemas distribuidos no consiste únicamente en ejecutar varios procesos al tiempo, sino en controlar correctamente el acceso a los recursos compartidos.

La base de datos cumplió un papel fundamental en la coordinación del trabajo mediante transacciones y bloqueos, mientras que el uso de un índice único y escritura sincronizada sobre archivo permitió preservar la consistencia de la información.

La solución final cumplió con los requisitos funcionales planteados y demostró un manejo adecuado de concurrencia, exclusión mutua y sincronización entre contenedores.
```sql
FOR UPDATE SKIP LOCKED
