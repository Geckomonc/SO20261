#ifndef SCHEDULER_H
#define SCHEDULER_H

#define PID_SIZE 16
#define NUM_QUEUES 3

typedef struct
{
    char pid[PID_SIZE];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;
    int first_response_time;
    int current_queue;
    int response_time;
    int turnaround_time;
    int waiting_time;
    int started;
    int completed;
    int enqueued;
} Process;

Process *create_processes(int count);
void destroy_processes(Process *processes);

void initialize_process(Process *process);
void calculate_metrics(Process *processes, int count);
void print_results(const Process *processes, int count);
void run_mlfq(Process *processes, int count, int boost_interval);

#endif
