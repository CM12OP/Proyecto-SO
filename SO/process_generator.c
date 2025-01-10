#include "process_generator.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern pthread_cond_t timer_cond;
extern pthread_mutex_t timer_mutex;
extern int working;

void *process_generator_thread(void *arg){
    ProcessQueue *pq =(ProcessQueue *)arg;
    while(working){
        pthread_mutex_lock(&timer_mutex);
        pthread_cond_wait(&timer_cond, &timer_mutex); //espera al timer
        pthread_mutex_unlock(&timer_mutex);

        pthread_mutex_lock(&pq->lock);
        if((pq->size)<(pq->capacity)){
            PCB new_process;
            new_process.pid = rand() % 1000; //genera un PID aleatorio
            new_process.vida = rand() % 10; //genera un tiempo de vida aleatorio
            pq->queue[pq->rear] = new_process;
            pq->rear = (pq->rear + 1) % pq->capacity;
            pq->size++;
            printf("Generated process PID: %d\n", new_process.pid);
            pthread_cond_signal(&pq->cond);
        }
        pthread_mutex_unlock(&pq->lock);
        
    }
    return NULL;
}

void *priority_process_generator_thread(void *arg) {
    ProcessQueue *pq = (ProcessQueue *)arg;
    while(working){
        pthread_mutex_lock(&timer_mutex);
        pthread_cond_wait(&timer_cond, &timer_mutex); //espera al timer
        pthread_mutex_unlock(&timer_mutex);

        // Crear un nuevo proceso
        PCB new_process;
        new_process.pid = rand() % 1000; // genera un PID aleatorio
        new_process.vida = rand() % 10 + 1; // genera un tiempo de vida aleatorio entre 1 y 10
        new_process.priority = rand() % 5; // genera una prioridad aleatoria entre 0 y 4

        // Insertar el proceso en la cola con prioridad
        insert_process(pq, new_process);

        printf("Generated process PID: %d, Priority: %d, Lifetime: %d\n", 
               new_process.pid, new_process.priority, new_process.vida);
    }
    return NULL;
}
