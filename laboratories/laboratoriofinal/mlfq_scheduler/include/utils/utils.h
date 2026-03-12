#ifndef UTILS_H
#define UTILS_H

#include "../scheduler/scheduler.h"

int ensure_directory(const char *path);
int export_results_csv(const char *filename, const Process *processes, int count);

int count_processes_in_csv(const char *filename);
Process *load_processes_from_csv(const char *filename, int *count);

#endif
