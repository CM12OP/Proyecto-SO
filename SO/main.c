#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "clock.h"
#include "timer.h"
#include "process_generator.h"
#include "scheduler.h"
#include "data_structures.h"
#include "machine.h"

Machine machine;
int working = 0;

int main(){
    
    pthread_t clock_tid, timer_tid, process_gen_tid, scheduler_tid;
    //Inicializar estructuras de datos
    ProcessQueue pq;
    int numcpus, numcores, numthreads, simT, plan;
    init_process_queue(&pq,10);
    printf("\nIndica el numero de CPUs: ");
    if(scanf("%d", &numcpus) !=1){
        fprintf(stderr, "Error: Entrada invalida. Se esperaba un numero entero.\n");
        exit(EXIT_FAILURE);
    }
    printf("\nIndica el número de Cores por CPU: ");
    if (scanf("%d", &numcores)!=1){
        fprintf(stderr, "Error: Entrada invalida. Se esperaba un numero entero.\n");
        exit(EXIT_FAILURE);
    }

    printf("\nIndica el número de Threads por Core: ");
    if (scanf("%d", &numthreads)!=1){
        fprintf(stderr, "Error: Entrada invalida. Se esperaba un numero entero.\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");
    init_machine(&machine, numcpus, numcores, numthreads);

    printf("\n\nESPECIFICACIONES DE LA MAQUINA");
    printf("\n==============================\n\n");

    printf("Numero de CPUs: %d\n", machine.num_cpus);
    printf("Numero de Cores por CPU: %d\n", machine.cpus[0].num_cores);
    printf("Numero de Threads por Core: %d\n", machine.cpus[0].cores[0].num_threads);

    printf("\n\nIndica tiempo de simulacion (seg): ");
    if(scanf("%d", &simT) !=1){
    fprintf(stderr, "Error: Entrada invalida. Se esperaba un numero entero.\n");
    exit(EXIT_FAILURE);
    }

    printf("\n\nIndica tipo de planificación (FiFo 0//Priority 1): ");
    if(scanf("%d", &plan) !=1){
    fprintf(stderr, "Error: Entrada invalida. Se esperaba un numero entero.\n");
    exit(EXIT_FAILURE);
    }

    printf("\n\nINICIO DE LA SIMULACION DEL KERNEL");
    printf("\n==================================\n\n");    
    working = 1;

    //crear hilos
    pthread_create(&clock_tid,NULL, clock_thread,NULL);
    pthread_create(&timer_tid,NULL, timer_thread,NULL);
    if (plan== 0) {
        pthread_create(&process_gen_tid, NULL, process_generator_thread, &pq);
    } else {
        pthread_create(&process_gen_tid, NULL, priority_process_generator_thread, &pq);
    }
    pthread_create(&scheduler_tid,NULL, scheduler_thread,&pq);

    sleep(simT);
    working = 0;

    //Esperar a que los hilos terminen (nunca lo harán en este ejemplo)
    pthread_detach(clock_tid);
    pthread_detach(timer_tid);
    pthread_detach(process_gen_tid);
    pthread_detach(scheduler_tid);

    //destruir estructuras de datos
    destroy_process_queue(&pq);
    destroy_machine(&machine);

    printf("\n\n=================================="); 
    printf("\nFIN DE LA SIMULACION DEL KERNEL\n\n");


    return 0;
}