#ifndef TICKET_H
#define TICKET_H

#include <stdio.h>

// Definir la estructura Ticket
typedef struct Ticket {
    long radicado;
    int id;
    char correo[100];
    char tipo[100];
} Ticket;

// Funciones para manejar tickets
Ticket* crear_ticket(int id, const char* correo, const char* tipo);
void guardar_ticket(Ticket* ticket);
void liberar_ticket(Ticket* ticket);

#endif // TICKET_H
