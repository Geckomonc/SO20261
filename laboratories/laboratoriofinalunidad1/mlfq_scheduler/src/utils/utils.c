#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../../include/utils/utils.h"

int ensure_directory(const char *path)
{
    struct stat st;

    if (stat(path, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
        {
            return 1;
        }

        fprintf(stderr, "Error: '%s' existe pero no es un directorio.\n", path);
        return 0;
    }

    if (mkdir(path, 0755) != 0)
    {
        perror("Error al crear el directorio");
        return 0;
    }

    return 1;
}

int export_results_csv(const char *filename, const Process *processes, int count)
{
    FILE *file = NULL;
    int i;

    file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error al abrir results.csv");
        return 0;
    }

    fprintf(file, "PID,Arrival,Burst,Start,Finish,Response,Turnaround,Waiting\n");

    for (i = 0; i < count; i++)
    {
        fprintf(file, "%s,%d,%d,%d,%d,%d,%d,%d\n",
                processes[i].pid,
                processes[i].arrival_time,
                processes[i].burst_time,
                processes[i].start_time,
                processes[i].finish_time,
                processes[i].response_time,
                processes[i].turnaround_time,
                processes[i].waiting_time);
    }

    fclose(file);
    return 1;
}

int count_processes_in_csv(const char *filename)
{
    FILE *file = NULL;
    char line[256];
    int count = 0;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error al abrir el archivo de procesos");
        return -1;
    }

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        fprintf(stderr, "Error: el archivo CSV está vacío o no tiene cabecera.\n");
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strlen(line) > 1)
        {
            count++;
        }
    }

    fclose(file);
    return count;
}

Process *load_processes_from_csv(const char *filename, int *count)
{
    FILE *file = NULL;
    char line[256];
    int total;
    int index = 0;
    Process *processes = NULL;

    if (count == NULL)
    {
        fprintf(stderr, "Error: puntero count inválido.\n");
        return NULL;
    }

    total = count_processes_in_csv(filename);
    if (total <= 0)
    {
        fprintf(stderr, "Error: no hay procesos válidos en el archivo CSV.\n");
        return NULL;
    }

    processes = malloc(sizeof(Process) * (size_t) total);
    if (processes == NULL)
    {
        fprintf(stderr, "Error: no se pudo reservar memoria para los procesos.\n");
        return NULL;
    }

    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error al abrir el archivo de procesos");
        free(processes);
        return NULL;
    }

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fprintf(stderr, "Error: no se pudo leer la cabecera del CSV.\n");
        fclose(file);
        free(processes);
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL && index < total)
    {
        char pid[PID_SIZE];
        int arrival;
        int burst;
        int fields;

        if (strlen(line) <= 1)
        {
            continue;
        }

        fields = sscanf(line, "%15[^,],%d,%d", pid, &arrival, &burst);
        if (fields != 3)
        {
            fprintf(stderr, "Error: línea inválida en CSV: %s", line);
            fclose(file);
            free(processes);
            return NULL;
        }

        if (arrival < 0 || burst <= 0)
        {
            fprintf(stderr, "Error: arrival y burst deben ser válidos en la línea: %s", line);
            fclose(file);
            free(processes);
            return NULL;
        }

        initialize_process(&processes[index]);
        snprintf(processes[index].pid, PID_SIZE, "%s", pid);
        processes[index].arrival_time = arrival;
        processes[index].burst_time = burst;
        processes[index].remaining_time = burst;
        index++;
    }

    fclose(file);

    *count = total;
    return processes;
}
