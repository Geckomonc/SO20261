#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/scheduler/scheduler.h"

typedef struct
{
    Process **items;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

static int init_queue(Queue *queue, int capacity)
{
    queue->items = malloc(sizeof(Process *) * (size_t) capacity);
    if (queue->items == NULL)
    {
        return 0;
    }

    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    return 1;
}

static void destroy_queue(Queue *queue)
{
    free(queue->items);
    queue->items = NULL;
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = 0;
}

static int is_empty(const Queue *queue)
{
    return queue->size == 0;
}

static int enqueue(Queue *queue, Process *process)
{
    if (queue->size == queue->capacity)
    {
        return 0;
    }

    queue->items[queue->rear] = process;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
    return 1;
}

static Process *dequeue(Queue *queue)
{
    Process *process = NULL;

    if (is_empty(queue))
    {
        return NULL;
    }

    process = queue->items[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return process;
}

static int get_quantum(int queue_level)
{
    if (queue_level == 0)
    {
        return 2;
    }

    if (queue_level == 1)
    {
        return 4;
    }

    return 8;
}

static Queue *get_queue_by_level(Queue *q0, Queue *q1, Queue *q2, int level)
{
    if (level == 0)
    {
        return q0;
    }

    if (level == 1)
    {
        return q1;
    }

    return q2;
}

static Process *get_next_process(Queue *q0, Queue *q1, Queue *q2)
{
    if (!is_empty(q0))
    {
        return dequeue(q0);
    }

    if (!is_empty(q1))
    {
        return dequeue(q1);
    }

    if (!is_empty(q2))
    {
        return dequeue(q2);
    }

    return NULL;
}

static int has_higher_priority_ready(int current_queue, Queue *q0, Queue *q1)
{
    if (current_queue == 0)
    {
        return 0;
    }

    if (current_queue == 1)
    {
        return !is_empty(q0);
    }

    return (!is_empty(q0) || !is_empty(q1));
}

static void print_arrival_event(const Process *process, int current_time)
{
    printf("[t=%d] Arrival: %s entered Q0 (burst=%d)\n",
           current_time,
           process->pid,
           process->burst_time);
}

static void print_dispatch_event(const Process *process, int current_time, int slice_left)
{
    printf("[t=%d] Dispatch: %s running in Q%d (remaining=%d, quantum=%d)\n",
           current_time,
           process->pid,
           process->current_queue,
           process->remaining_time,
           slice_left);
}

static void print_preemption_event(const Process *process, int current_time)
{
    printf("[t=%d] Preemption: %s returned to Q%d due to higher priority process\n",
           current_time,
           process->pid,
           process->current_queue);
}

static void print_demotion_event(const Process *process, int current_time)
{
    printf("[t=%d] Demotion: %s moved to Q%d (remaining=%d)\n",
           current_time,
           process->pid,
           process->current_queue,
           process->remaining_time);
}

static void print_finish_event(const Process *process, int current_time)
{
    printf("[t=%d] Finish: %s completed\n",
           current_time,
           process->pid);
}

static void print_boost_event(int current_time)
{
    printf("[t=%d] Priority boost applied: all ready processes moved to Q0\n",
           current_time);
}

static void print_cpu_idle_event(int current_time)
{
    printf("[t=%d] CPU Idle\n", current_time);
}

static void add_arrivals(Process *processes, int count, int current_time, Queue *q0)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (processes[i].arrival_time == current_time && !processes[i].enqueued && !processes[i].completed)
        {
            processes[i].current_queue = 0;

            if (!enqueue(q0, &processes[i]))
            {
                fprintf(stderr, "Error: no fue posible encolar el proceso %s.\n", processes[i].pid);
                exit(EXIT_FAILURE);
            }

            processes[i].enqueued = 1;
            print_arrival_event(&processes[i], current_time);
        }
    }
}

static void priority_boost(Queue *q0, Queue *q1, Queue *q2)
{
    Process *process = NULL;

    while (!is_empty(q1))
    {
        process = dequeue(q1);
        process->current_queue = 0;

        if (!enqueue(q0, process))
        {
            fprintf(stderr, "Error: no fue posible aplicar priority boost.\n");
            exit(EXIT_FAILURE);
        }
    }

    while (!is_empty(q2))
    {
        process = dequeue(q2);
        process->current_queue = 0;

        if (!enqueue(q0, process))
        {
            fprintf(stderr, "Error: no fue posible aplicar priority boost.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void initialize_process(Process *process)
{
    memset(process, 0, sizeof(Process));
    process->start_time = -1;
    process->finish_time = -1;
    process->first_response_time = -1;
    process->current_queue = 0;
}

Process *create_processes(int count)
{
    Process *processes = NULL;
    int i;

    processes = malloc(sizeof(Process) * (size_t) count);
    if (processes == NULL)
    {
        fprintf(stderr, "Error: no se pudo reservar memoria para los procesos.\n");
        return NULL;
    }

    for (i = 0; i < count; i++)
    {
        initialize_process(&processes[i]);
    }

    return processes;
}

void destroy_processes(Process *processes)
{
    free(processes);
}

void run_mlfq(Process *processes, int count, int boost_interval)
{
    Queue q0;
    Queue q1;
    Queue q2;
    Process *running = NULL;
    int current_time = 0;
    int completed_count = 0;
    int slice_left = 0;
    Queue *target_queue = NULL;

    if (!init_queue(&q0, count) || !init_queue(&q1, count) || !init_queue(&q2, count))
    {
        fprintf(stderr, "Error: no se pudo inicializar las colas.\n");
        destroy_queue(&q0);
        destroy_queue(&q1);
        destroy_queue(&q2);
        exit(EXIT_FAILURE);
    }

    printf("\n=== MLFQ Execution Trace ===\n");

    while (completed_count < count)
    {
        add_arrivals(processes, count, current_time, &q0);

        if (boost_interval > 0 && current_time > 0 && current_time % boost_interval == 0)
        {
            if (running != NULL)
            {
                running->current_queue = 0;

                if (!enqueue(&q0, running))
                {
                    fprintf(stderr, "Error: no se pudo reinsertar el proceso en boost.\n");
                    destroy_queue(&q0);
                    destroy_queue(&q1);
                    destroy_queue(&q2);
                    exit(EXIT_FAILURE);
                }

                running = NULL;
                slice_left = 0;
            }

            priority_boost(&q0, &q1, &q2);
            print_boost_event(current_time);
        }

        if (running != NULL && has_higher_priority_ready(running->current_queue, &q0, &q1))
        {
            target_queue = get_queue_by_level(&q0, &q1, &q2, running->current_queue);

            if (!enqueue(target_queue, running))
            {
                fprintf(stderr, "Error: no se pudo reinsertar proceso preemptado.\n");
                destroy_queue(&q0);
                destroy_queue(&q1);
                destroy_queue(&q2);
                exit(EXIT_FAILURE);
            }

            print_preemption_event(running, current_time);
            running = NULL;
            slice_left = 0;
        }

        if (running == NULL)
        {
            running = get_next_process(&q0, &q1, &q2);

            if (running != NULL)
            {
                slice_left = get_quantum(running->current_queue);
                print_dispatch_event(running, current_time, slice_left);
            }
        }

        if (running == NULL)
        {
            print_cpu_idle_event(current_time);
            current_time++;
            continue;
        }

        if (!running->started)
        {
            running->started = 1;
            running->start_time = current_time;
            running->first_response_time = current_time;
        }

        running->remaining_time--;
        slice_left--;
        current_time++;

        if (running->remaining_time == 0)
        {
            running->completed = 1;
            running->finish_time = current_time;
            completed_count++;
            print_finish_event(running, current_time);
            running = NULL;
            slice_left = 0;
            continue;
        }

        if (slice_left == 0)
        {
            if (running->current_queue < 2)
            {
                running->current_queue++;
            }

            target_queue = get_queue_by_level(&q0, &q1, &q2, running->current_queue);

            if (!enqueue(target_queue, running))
            {
                fprintf(stderr, "Error: no se pudo reinsertar proceso tras agotar quantum.\n");
                destroy_queue(&q0);
                destroy_queue(&q1);
                destroy_queue(&q2);
                exit(EXIT_FAILURE);
            }

            print_demotion_event(running, current_time);
            running = NULL;
        }
    }

    printf("=== End of Trace ===\n");

    calculate_metrics(processes, count);

    destroy_queue(&q0);
    destroy_queue(&q1);
    destroy_queue(&q2);
}

void calculate_metrics(Process *processes, int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        processes[i].response_time = processes[i].first_response_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }
}

void print_results(const Process *processes, int count)
{
    int i;

    printf("\n%-6s %-8s %-6s %-6s %-7s %-9s %-11s %-8s\n",
           "PID", "Arrival", "Burst", "Start", "Finish", "Response", "Turnaround", "Waiting");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
    {
        printf("%-6s %-8d %-6d %-6d %-7d %-9d %-11d %-8d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].start_time,
               processes[i].finish_time,
               processes[i].response_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);
    }
}
