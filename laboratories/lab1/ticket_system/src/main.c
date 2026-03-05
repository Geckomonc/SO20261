#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ticket/ticket.h"
#include "utils/utils.h"

int main() {
    int id;
    char correo[100], tipo[100];

    // Solicitar identificación
    printf("Ingrese su identificación (numérica): ");
    char id_str[20];
    fgets(id_str, 20, stdin);
    id_str[strcspn(id_str, "\n")] = 0;

    if (!validar_identificacion(id_str)) {
        printf("Error: La identificación debe ser numérica.\n");
        return 1;
    }
    id = atoi(id_str);

    // Solicitar correo electrónico
    printf("Ingrese su correo electrónico: ");
    fgets(correo, 100, stdin);
    correo[strcspn(correo, "\n")] = 0; // Eliminar el salto de línea
    if (!validar_correo(correo)) {
        printf("Error: El correo debe contener un '@'.\n");
        return 1;
    }

    // Solicitar tipo de reclamación
    printf("Ingrese el tipo de reclamación: ");
    fgets(tipo, 100, stdin);
    tipo[strcspn(tipo, "\n")] = 0; // Eliminar el salto de línea
    if (!validar_cadena_no_vacia(tipo)) {
        printf("Error: El tipo de reclamación no puede estar vacío.\n");
        return 1;
    }

    // Crear el ticket
    Ticket* ticket = crear_ticket(id, correo, tipo);
    if (ticket == NULL) {
        return 1; // Error al crear el ticket
    }

    // Guardar el ticket
    guardar_ticket(ticket);

    // Liberar memoria
    liberar_ticket(ticket);

    return 0;
}
