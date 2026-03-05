#include "../include/ticket/ticket.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Crear un ticket con los datos proporcionados
Ticket* crear_ticket(int id, const char* correo, const char* tipo) {
    Ticket* nuevo_ticket = (Ticket*)malloc(sizeof(Ticket));
    if (nuevo_ticket == NULL) {
        printf("Error: No se pudo asignar memoria para el ticket.\n");
        return NULL;
    }

    nuevo_ticket->radicado = (long)time(NULL);
    nuevo_ticket->id = id;
    strncpy(nuevo_ticket->correo, correo, 100);
    strncpy(nuevo_ticket->tipo, tipo, 100);

    return nuevo_ticket;
}

// Guardar el ticket en un archivo
void guardar_ticket(Ticket* ticket) {
    char filename[100];
    snprintf(filename, sizeof(filename), "assets/ticket_%ld.txt", ticket->radicado);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo para guardar el ticket.\n");
        return;
    }

    fprintf(file, "Radicado: %ld\n", ticket->radicado);
    fprintf(file, "Identificación: %d\n", ticket->id);
    fprintf(file, "Correo: %s\n", ticket->correo);
    fprintf(file, "Tipo de reclamación: %s\n", ticket->tipo);

    fclose(file);
    printf("Ticket guardado correctamente. Radicado: %ld\n", ticket->radicado);
}

// Liberar la memoria del ticket
void liberar_ticket(Ticket* ticket) {
    free(ticket);
}
