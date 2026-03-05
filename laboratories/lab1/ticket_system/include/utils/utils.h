#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Funciones para validar entradas
bool validar_identificacion(const char* id);
bool validar_correo(const char* correo);
bool validar_cadena_no_vacia(const char* cadena);

#endif // UTILS_H
