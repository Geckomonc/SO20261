# Simulador de Scheduler MLFQ

## Descripción
Este proyecto implementa un simulador de planificación de CPU con política Multi-Level Feedback Queue (MLFQ) en lenguaje C. El sistema permite:

- Leer procesos desde un archivo CSV.
- Organizar los procesos en múltiples colas con distinta prioridad.
- Aplicar Round Robin dentro de cada cola.
- Usar quantums diferentes por nivel.
- Realizar democión de procesos cuando consumen todo su quantum.
- Aplicar priority boost periódico para evitar inanición.
- Calcular métricas de rendimiento por proceso.
- Exportar los resultados finales a un archivo results.csv.

## Estructura del Proyecto
El proyecto sigue una estructura modularizada, con los siguientes directorios y archivos:

<img width="150" height="99" alt="image" src="https://github.com/user-attachments/assets/fb4c50bd-64b9-4a17-976f-c73ac59016af" />

## Cómo compilar el programa
Para compilar el programa, sigue estos pasos:
1. Clona este repositorio:
   ```bash
   git clone https://github.com/Geckomonc/SO20261.git
   ```
2. Entrar en el directorio del proyecto:
    ```bash
   cd mlfq_scheduler
   ```
3. Compila el programa utilizando el Makefile: (En la carpeta donde se encuentre el Makefile escribir el siguiente comando)
   ```bash
   make
   ```
4. Para ejecutar el programa después de compilar, utiliza el siguiente comando:
   ```bash
   make run
   ```
6. Para limpiar los archivos después de la compilación
   ```bash
   make clean
   ```


## Explicación

El programa toma como entrada un archivo assets/processes.csv, en el cual se define cada proceso mediante:

Formato esperado

<img width="154" height="108" alt="image" src="https://github.com/user-attachments/assets/cdf39f8c-e3a1-4d67-8a8b-0d7267c57fc9" />

NOTA: Puede entrar al archivo y cambiar los datos para realizar el proceso con los datos que quiera.

Cada proceso inicia en la cola de mayor prioridad (Q0).
El scheduler evalúa el sistema por ciclos de reloj y siempre selecciona el proceso listo de la cola con mayor prioridad disponible.
Las colas funcionan así:

- Q0: prioridad alta, quantum = 2
- Q1: prioridad media, quantum = 4
- Q2: prioridad baja, quantum = 8

Si un proceso consume completamente su quantum y no termina, es demovido a una cola inferior.
Si termina antes de agotar el quantum, conserva su comportamiento normal sin bajar de nivel.
Además, cada cierto número de ciclos configurado por el usuario en el código, se aplica un priority boost, devolviendo todos los procesos listos a la cola de mayor prioridad.

## Qué muestra el programa

1. Traza de ejecución

En consola se imprime una secuencia detallada del comportamiento del scheduler ciclo por ciclo, indicando eventos como:
- llegada de procesos, despacho a CPU, democión entre colas, finalización, preempción, boost de prioridad, ciclos ociosos.

<img width="517" height="489" alt="image" src="https://github.com/user-attachments/assets/a666734f-6922-4eb0-9b86-7c863ec663da" />

2. Tabla de resultados

Al final, el programa muestra en consola una tabla con las métricas de cada proceso:
- PID, Arrival, Burst, Start, Finish, Response, Turnaround, Waiting

<img width="610" height="114" alt="image" src="https://github.com/user-attachments/assets/43391bdf-da20-4fbe-80a4-ecad15c6b936" />

NOTA: Estas métricas también se exportan automáticamente al archivo.
assets/results.csv

<img width="385" height="52" alt="image" src="https://github.com/user-attachments/assets/93949aff-61f8-4b51-87cf-4d677b4e5375" />

¿Qué ocurre si el boost es muy frecuente?

Si el priority boost ocurre muy seguido, los procesos regresan constantemente a Q0, por lo que las colas bajas casi no conservan su función.
En la práctica, el scheduler se comporta más parecido a un Round Robin con mucha prioridad alta, reduciendo el efecto de la retroalimentación entre niveles.

¿Qué ocurre si no existe boost?

Si no hay boost, los procesos largos que van bajando a Q1 o Q2 pueden quedarse esperando mucho tiempo, sobre todo si siguen llegando procesos nuevos a Q0.
Esto hace que aumente el riesgo de inanición y empeore el tiempo de espera de los procesos de baja prioridad.

¿Cómo afecta un quantum pequeño en la cola de mayor prioridad?

Un quantum pequeño en la cola de mayor prioridad mejora el tiempo de respuesta de procesos nuevos o cortos, porque pueden ejecutarse rápido apenas llegan.
Sin embargo, también provoca más cambios de contexto y hace que los procesos largos sean demovidos más rápido a colas inferiores.

¿Puede haber starvation?

Sí, puede haber starvation si no existe priority boost o si este ocurre muy tarde.
Esto pasa porque un proceso en colas bajas puede ser postergado repetidamente mientras siguen llegando procesos a colas de prioridad superior.
