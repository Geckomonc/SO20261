#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler/scheduler.h"
#include "../include/utils/utils.h"

int main(void)
{
    const int boost_interval = 20;
    const char *input_file = "assets/processes.csv";
    const char *output_file = "assets/results.csv";
    int process_count = 0;
    Process *processes = NULL;

    if (!ensure_directory("assets"))
    {
        return EXIT_FAILURE;
    }

    processes = load_processes_from_csv(input_file, &process_count);
    if (processes == NULL)
    {
        return EXIT_FAILURE;
    }

    run_mlfq(processes, process_count, boost_interval);
    print_results(processes, process_count);

    if (!export_results_csv(output_file, processes, process_count))
    {
        destroy_processes(processes);
        return EXIT_FAILURE;
    }

    printf("\nArchivo de entrada: %s\n", input_file);
    printf("Archivo de salida generado: %s\n", output_file);

    destroy_processes(processes);
    return EXIT_SUCCESS;
}
