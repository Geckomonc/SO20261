#include "../include/utils/utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Validar que la identificación sea numérica
bool validar_identificacion(const char* id) {
    for (int i = 0; id[i] != '\0'; i++) {
        if (!isdigit(id[i])) {
            return false;
        }
    }
    return true;
}

// Validar que el correo contenga al menos un '@'
bool validar_correo(const char* correo) {
    return strchr(correo, '@') != NULL;
}

// Validar que la cadena no esté vacía
bool validar_cadena_no_vacia(const char* cadena) {
    return cadena != NULL && strlen(cadena) > 0;
}
