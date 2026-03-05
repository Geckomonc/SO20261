# Sistema de Tickets de Reclamación

## Descripción
Este proyecto implementa un sistema para registrar tickets de reclamación, con las siguientes funcionalidades:
- Solicitar identificación, correo electrónico y tipo de reclamación.
- Validación de entradas.
- Generación de un número de radicado único.
- Creación de archivos con los datos del ticket.

## Estructura del Proyecto
El proyecto sigue una estructura modularizada, con los siguientes directorios y archivos:

<img width="132" height="320" alt="image" src="https://github.com/user-attachments/assets/8d982991-66c1-4fa9-89d8-de169aec40c7" />

## Cómo compilar el programa
Para compilar el programa, sigue estos pasos:
1. Clona este repositorio:
   ```bash
   git clone https://github.com/Geckomonc/SO20261.git
   ```
2. Entrar en el directorio del proyecto:
    ```bash
   cd ticket_system
   ```
3. Compila el programa utilizando el Makefile: (En la carpeta donde se encuentre el Makefile escribir el siguiente comando)
   ```bash
   make
   ```
4. Para ejecutar el programa después de compilar, utiliza el siguiente comando:
   ```bash
   make run
   ```
   El programa te pedirá la identificación, correo electrónico y tipo de reclamación.
   
6. Para limpiar los archivos después de la compilación
   ```bash
   make clean
   ```

Explicación.

Uso de punteros

El programa hace uso de punteros para trabajar con la estructura Ticket. Usamos punteros para crear e interactuar con los tickets de manera eficiente. Además, los punteros permiten pasar las estructuras por referencia a las funciones, evitando copias innecesarias de datos.

Manejo de memoria

En el código, utilizamos malloc para asignar memoria dinámica a los tickets, y free para liberar esa memoria cuando ya no se necesita. Este manejo adecuado de la memoria garantiza que no haya fugas de memoria.

Generación del radicado

El número de radicado se genera utilizando la función time(NULL), lo que asegura que cada radicado sea único, ya que se basa en el tiempo actual del sistema.

Manejo de errores

El programa valida todas las entradas del usuario para asegurarse de que sean correctas. Si el usuario ingresa datos inválidos, el programa muestra un mensaje de error y termina de manera adecuada.

Funcionamiento del Makefile

El Makefile se encarga de la compilación del proyecto. El comando make compila los archivos fuente y genera el ejecutable ticket_app. El comando make run ejecuta el programa, y make clean elimina los archivos generados durante la compilación.

Requisitos

Para compilar y ejecutar el proyecto, necesitas tener instalado gcc y make.
