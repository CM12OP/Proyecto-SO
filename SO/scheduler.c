#include "scheduler.h"
#include "data_structures.h"
#include "machine.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

extern Machine machine;
extern int working;
extern pthread_mutex_t machine_mutex;
extern pthread_mutex_t timer_mutex;
extern pthread_cond_t timer_cond;


void *simulate_process(void *arg){
    ProcessArgs *args =(ProcessArgs *)arg;
    PCB *process = args->process;
    HardwareThread *available_thread = args->available_thread;
    int remaining_time = process->vida;

    while (remaining_time >0 && working){
        // Espera la señal del timer
        pthread_mutex_lock(&timer_mutex);
        pthread_cond_wait(&timer_cond, &timer_mutex); // Espera un tick del reloj
        pthread_mutex_unlock(&timer_mutex);

        // Resta 1 al tiempo de vida del proceso
        remaining_time--;
    }

    if (remaining_time <=0){
        printf("Finished process PID: %d********************************************\n", process->pid);
    }
    // Marca el hilo como libre después de la simulación
    pthread_mutex_lock(&machine_mutex);
    available_thread->process = NULL;
    pthread_mutex_unlock(&machine_mutex);

    free(args); // Liberar la memoria asignada para los argumentos
    return NULL;
}


void *scheduler_thread(void *arg){
    ProcessQueue *pq =(ProcessQueue *)arg;
    while(working){
        int clocking = 1;
        int cpu, core, thread;
        pthread_mutex_lock(&pq->lock);
        while (pq->size == 0 && working) {
            pthread_cond_wait(&pq->cond, &pq->lock);
        }
        //desencola un proceso y simula el scheduling.
        PCB process = pq->queue[pq->front];
        pq->front = (pq->front + 1) % pq->capacity;
        pq->size--;
        pthread_mutex_unlock(&pq->lock);

        //busca el primr hilo disponible
        pthread_mutex_lock(&machine_mutex);
        HardwareThread *available_thread = NULL;
        for(int i= 0; i < machine.num_cpus && !available_thread; i++){
            for(int j =0; j < machine.cpus[i].num_cores && !available_thread; j++){
                for(int k =0; k < machine.cpus[i].cores[j].num_threads; k++){
                    if(machine.cpus[i].cores[j].threads[k].process == NULL){
                        available_thread = &machine.cpus[i].cores[j].threads[k];
                        cpu = i; core = j; thread = k;
                        break;
                    }
                }
            }
        }
        if (available_thread){
            //asigna el proceso al hilo disponible
            available_thread->process = &process;  // Asignación del proceso
            printf("Scheduled process PID: %d on CPU %d, Core %d, Thread %d\n", 
            process.pid, cpu, core, thread);

            pthread_mutex_unlock(&machine_mutex);
            
            //cear un nuevo hilo para simular el procesamiento del proceso
            pthread_t process_tid;
            ProcessArgs *args =malloc(sizeof(ProcessArgs));
            args->process =&process;
            args->available_thread =available_thread;
            pthread_create(&process_tid, NULL, simulate_process, args);
            pthread_detach(process_tid); //desvincular el hilo 


        }else {
                printf("No hay hilos disponibles para el proceso PID: %d\n", process.pid);
                pthread_mutex_unlock(&machine_mutex);
                //volver a poner el proceso en la cola
                pthread_mutex_lock(&pq->lock);
                pq->queue[(pq->front + pq->size) % pq->capacity] = process;
                pq->size++;
                pthread_mutex_unlock(&pq->lock);
            }

        while (clocking && working){
            //espera la señal del timer para continuar
            pthread_mutex_lock(&timer_mutex);
            pthread_cond_wait(&timer_cond, &timer_mutex);
            pthread_mutex_unlock(&timer_mutex); 
            clocking = 0;
        }
    }
    return NULL;
}
