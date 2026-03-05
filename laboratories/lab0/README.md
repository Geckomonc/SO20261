# Lab0: Primeros pasos con la Shell y Entrega en GitHub (Operating Systems 2026-1)

## Objetivo
Realizar un reto usando la terminal Bash para crear y manipular directorios/archivos, guardar evidencias (path.txt, output.txt), adjuntar capturas del curso RedHat (módulos 1 a 5) y subir todo a GitHub en la rama main.

## 1) Integrantes:
- Geraldine Acevedo Restrepo
- Steven Alipio Berrio

## 2) Progreso en RedHat (Módulos 1 al 5)
- Evidencias:
  
  <img width="1380" height="682" alt="image" src="https://github.com/user-attachments/assets/e2b20462-2038-4550-9b8e-847e53f35033" />

## 3) Crear el directorio del laboratorio (lab0)
Crear el directorio solicitado en la ruta:
`~/operating-systems-20261/laboratories/lab0`

```bash
mkdir -p ~/operating-systems-20261/laboratories/lab0
```

## 4) Navegar al directorio recién creado

```bash
cd ~/operating-systems-20261/laboratories/lab0
```

## 5) Imprimir la ruta absoluta y guardarla en path.txt
Se imprime la ruta absoluta del directorio actual y se redirige la salida al archivo solicitado:

```bash
pwd > ~/operating-systems-20261/laboratories/lab0/path.txt
```

Verificación:

```bash
cat ~/operating-systems-20261/laboratories/lab0/path.txt
```

## 6) Crear con un solo comando los directorios example, music, photos, projects

```bash
mkdir -p example music photos projects
```

## 7) Crear con un solo comando file1...file100 dentro de cada directorio

```bash
touch example/file{1..100} music/file{1..100} photos/file{1..100} projects/file{1..100}
```

Verificación:

```bash
ls example | wc -l
ls music | wc -l
ls photos | wc -l
ls projects | wc -l
```

## 8) Borrar los primeros 10 y los últimos 20 archivos por cada directorio

```bash
rm {example,music,photos,projects}/file{1..10} {example,music,photos,projects}/file{81..100}
```

## 9) Mover los directorios example, music y photos dentro de projects

```bash
mv example music photos projects/
```

## 10) Eliminar SOLO los archivos (no directorios) de projects con opción verbosa y guardar salida en output.txt

```bash
rm -v projects/file* > ~/operating-systems-20261/laboratories/lab0/output.txt
```

Verificación:

```bash
ls -la projects
head ~/operating-systems-20261/laboratories/lab0/output.txt
tail ~/operating-systems-20261/laboratories/lab0/output.txt
```

## 11) Subir el contenido a GitHub desde ~/operating-systems-20261/

Ir a la raíz del repositorio:

```bash
cd ~/operating-systems-20261
```

Revisar estado:

```bash
git status
```

Agregar cambios del lab:

```bash
git add laboratories/lab0
```

Confirmar cambios:

```bash
git status
```

Hacer commit:

```bash
git commit -m "lab0: reto de shell y evidencias"
```

Subir a la rama main:

```bash
git push -u origin main
```

## 12) Shortcuts usados en Bash

Tab: autocompletar rutas y comandos.

Ctrl + R: búsqueda en el historial de comandos.

Ctrl + A: ir al inicio de la línea.

Ctrl + E: ir al final de la línea.

Ctrl + C: cancelar un comando/proceso.

!!: repetir el último comando.
