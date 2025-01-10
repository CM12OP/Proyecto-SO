#include "machine.h"
#include <stdlib.h>


pthread_mutex_t machine_mutex = PTHREAD_MUTEX_INITIALIZER;


void init_machine(Machine *machine, int num_cpus, int num_cores_per_cpu, int num_threads_per_core) {
    machine->num_cpus = num_cpus;
    machine->cpus = malloc(num_cpus * sizeof(CPU));
    
    for(int i= 0; i <num_cpus;i++){
        machine->cpus[i].id = i;
        machine->cpus[i].num_cores = num_cores_per_cpu;
        machine->cpus[i].cores = malloc(num_cores_per_cpu * sizeof(Core));
        
        for(int j =0; j <num_cores_per_cpu;j++){
            machine->cpus[i].cores[j].id = j;
            machine->cpus[i].cores[j].num_threads = num_threads_per_core;
            machine->cpus[i].cores[j].threads = malloc(num_threads_per_core *sizeof(HardwareThread));
            
            for(int k=0;k <num_threads_per_core;k++){
                machine->cpus[i].cores[j].threads[k].id =k;
            }
        }
    }
}

void destroy_machine(Machine *machine) {
    for (int i = 0; i <machine->num_cpus;i++){
        for (int j = 0; j <machine->cpus[i].num_cores;j++){
            free(machine->cpus[i].cores[j].threads);
        }
        free(machine->cpus[i].cores);
    }
    free(machine->cpus);
}
