# Laboratorio de Administración de Usuarios en Linux

## Descripción

Este proyecto implementa un sistema de administración de usuarios en Linux, ejecutado dentro de un contenedor Docker basado en Debian Slim.  
El sistema fue desarrollado para operar completamente desde consola y centraliza las funciones administrativas en un script principal llamado `labctl.sh`.

El objetivo es permitir la gestión básica de usuarios del sistema y aplicar políticas de seguridad relacionadas con contraseñas, vencimiento, bloqueo y privilegios restringidos.

---

## Objetivo del sistema

El sistema permite al administrador realizar operaciones de gestión de usuarios desde consola, cumpliendo con los requisitos del laboratorio:

- Crear usuarios del sistema
- Listar usuarios
- Bloquear usuarios
- Desbloquear usuarios
- Eliminar usuarios
- Asignar contraseña por defecto
- Forzar cambio de contraseña en el primer inicio de sesión
- Aplicar reglas de complejidad para contraseñas
- Mostrar advertencia cuando la contraseña está próxima a vencer
- Bloquear automáticamente usuarios con contraseña vencida
- Asignar permisos sudo restringidos únicamente a las acciones del laboratorio

---

## Tecnologías utilizadas

- Docker
- Debian Slim
- Bash Scripting
- PAM (`pam_pwquality`)
- Comandos de administración de usuarios en Linux (`useradd`, `passwd`, `chage`, `userdel`, `sudo`)

---

## Estructura del proyecto

```bash
/opt/lab_usuarios
├── labctl.sh
├── validate_password.sh
├── password_warning.sh
├── configure_pam.sh
└── setup.sh
```
---
Ahora mostraré un paso a paso de la ejecución del laboratorio y las pruebas necesarias

1. Crear el archivo Dockerfile para crear la imagen y luego correr el contenedor

2. Usar el comando para crear la imagen

```bash
docker build -t lab-usuarios .
```

<img width="844" height="275" alt="image" src="https://github.com/user-attachments/assets/c3c13ded-010e-4ca2-b18a-01dff0799176" />


3. Usar el comando para correr el contenedor

```bash
docker run -it --name contenedor-lab-usuarios lab-usuarios
```

<img width="780" height="216" alt="image" src="https://github.com/user-attachments/assets/5e3cefaa-7923-4c72-b1c8-4085f7858f19" />

4. Crear las carpetas y archivos necesarios para ser un administrador de linux

<img width="453" height="132" alt="image" src="https://github.com/user-attachments/assets/ad002c4f-42ea-471b-916a-d8b51488be91" />

5. Dar permisos de ejecución a los archivos

<img width="783" height="135" alt="image" src="https://github.com/user-attachments/assets/edeec1b1-f1ae-4019-88ce-553cee81884d" />

6. Crear validar contraseña con nano y hacer pruebas

<img width="506" height="71" alt="image" src="https://github.com/user-attachments/assets/4db0fe11-d30f-404a-a61c-9cc65acecc05" />

7. Crear señales de alerta de contraseña

<img width="434" height="22" alt="image" src="https://github.com/user-attachments/assets/46ecf9af-debf-40b1-956d-884c8c2d0996" />

Luego darle permiso de ejecución

<img width="569" height="19" alt="image" src="https://github.com/user-attachments/assets/574cbe18-ef5b-49b8-a5cb-4af8e80b47f4" />

Crear un usuario desde consola, cambiar cuando vence su contraseña para probar el mensaje

<img width="633" height="406" alt="image" src="https://github.com/user-attachments/assets/047e03b0-c7d7-4fde-88c4-0b45bb22540b" />

8. Crear la configuración pam para que nuestro linux tenga contraseñas más fuertes

<img width="560" height="20" alt="image" src="https://github.com/user-attachments/assets/63e51a12-1c70-487c-b8e9-86b6b6c6d0c0" />

<img width="552" height="49" alt="image" src="https://github.com/user-attachments/assets/62286766-7280-4a3e-973a-e8e70d2f14f9" />

Revisamos que si se haya guardado la configuración

<img width="773" height="39" alt="image" src="https://github.com/user-attachments/assets/a083899d-493f-4761-b3cd-dbb814f68364" />

9. Creamos el archivo setup en el cual dejaremos lista la configuración como: copiar el aviso de vencimiento, dar permisos, ejecutar la configuración PAM.

<img width="484" height="26" alt="image" src="https://github.com/user-attachments/assets/3e06a087-cfd0-41cf-b602-ade199da23a0" />

<img width="465" height="75" alt="image" src="https://github.com/user-attachments/assets/5971da0e-4f61-4e8c-9865-bd4e23c6baee" />

10. Ahora crearemos el archivo principal labctl que es el comando con estas opciones:

- listar usuarios
- crear usuario
- bloquear usuario
- desbloquear usuario
- eliminar usuario
- ver estado del usuario
- agregar permiso sudo restringido
- revisar expirados

Hay que darle permisos

<img width="517" height="18" alt="image" src="https://github.com/user-attachments/assets/38c9fdf8-43a8-403f-afa7-32d4ac2c9622" />

Creamos un alias para poder usarlo más facilmente

<img width="661" height="48" alt="image" src="https://github.com/user-attachments/assets/6537abb1-57c1-4986-81d8-bc7e5bc4058e" />

Luego empezamos a probar

(1) Probamos la ayuda

<img width="457" height="189" alt="image" src="https://github.com/user-attachments/assets/3518e9f0-41c3-468b-9b16-9c80b46a5dec" />

(2) Probar lista de usuarios

<img width="405" height="46" alt="image" src="https://github.com/user-attachments/assets/2866d9ad-e7eb-4d5a-9bbb-0618a7990c5c" />

(3) Probar crear un usuario

<img width="409" height="74" alt="image" src="https://github.com/user-attachments/assets/2470aa52-8f9e-4a17-9fbd-c5d940cbaf7b" />

(4) Probar bloquear un usuario

<img width="402" height="48" alt="image" src="https://github.com/user-attachments/assets/bd404871-2528-48d9-a53d-6e51b8fe892d" />

(5) Comprobamos el status del usuario

<img width="589" height="130" alt="image" src="https://github.com/user-attachments/assets/35996daf-6d9d-4028-a34b-26ade3aa1778" />

(6) Ingresamos al usuario creado y comprobamos que al primer inicio debemos cambiar la contraseña

<img width="555" height="168" alt="image" src="https://github.com/user-attachments/assets/c3e39b83-c80d-4422-b4f3-a3d062a1f1fd" />

(7) Comprobamos que solo superusuario puede ejectutar acciones administrativas

<img width="505" height="37" alt="image" src="https://github.com/user-attachments/assets/ad8bc415-0050-4901-a767-e1dfabf749a6" />

(8) Damos permiso de sudo restringido a un usuario

<img width="487" height="269" alt="image" src="https://github.com/user-attachments/assets/d11654d1-e514-4ec3-85aa-98e020af4bd4" />

Comprobamos que no pueda crear un nuevo usuario a menos que use el comando sudo

<img width="706" height="78" alt="image" src="https://github.com/user-attachments/assets/43e18cd2-85fb-4a1b-b090-2d99149ac76d" />

Ahora intentamos con el comando sudo

<img width="552" height="79" alt="image" src="https://github.com/user-attachments/assets/1734169f-077c-47e0-bc68-d17dc12c1e6b" />

(9) Cambio la fecha de vencimiento de la contraseña para mostrar el mensaje de que se va a vencer pronto

<img width="649" height="234" alt="image" src="https://github.com/user-attachments/assets/25b9ecf1-c6d7-401e-b5f7-9bfef530d00c" />

(10) Cambiamos los días para que el usuario aparezca con la contraseña vencida y por lo tanto s eencuentre bloqueado

<img width="634" height="301" alt="image" src="https://github.com/user-attachments/assets/3a72e3ff-0946-44c0-b8d3-6da72adf9201" />

(11) listamos los usuarios actuales

<img width="433" height="72" alt="image" src="https://github.com/user-attachments/assets/263d0f52-439f-48ed-833a-a404dbdfe702" />

(12) eliminamos un usuario

<img width="417" height="113" alt="image" src="https://github.com/user-attachments/assets/64bf0e5b-bb9a-47ae-a880-3d8b9736883d" />

(13) también bloqueamos y desbloqueamos un usuario

<img width="587" height="356" alt="image" src="https://github.com/user-attachments/assets/eebc8c1d-d369-4f34-8a53-1b3294a084c7" />

FIN





