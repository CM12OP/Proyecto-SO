#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "data_structures.h"
#include "machine.h"
void *scheduler_thread(void *arg);

//estructura para crear hilos de hardware
typedef struct {
    PCB *process;
    HardwareThread *available_thread;
} ProcessArgs;

#endif //SCHEDULER_H