#ifndef MACHINE_H
#define MACHINE_H

#include <pthread.h>
#include "data_structures.h"


typedef struct {
    int id; // ID del hilo de hardware
    PCB *process; // Puntero al proceso en ejecución
} HardwareThread;

typedef struct {
    int id; // ID del núcleo
    int num_threads;
    HardwareThread *threads; // Array de hilos de hardware
} Core;

typedef struct {
    int id; // ID de la CPU
    int num_cores;
    Core *cores; // Array de núcleos
} CPU;

typedef struct {
    int num_cpus;
    CPU *cpus; // Array de CPUs
} Machine;

// Funciones para inicializar y liberar la estructura Machine
void init_machine(Machine *machine, int num_cpus, int num_cores_per_cpu, int num_threads_per_core);
void destroy_machine(Machine *machine);

#endif // MACHINE_H
