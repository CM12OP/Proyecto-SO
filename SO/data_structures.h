#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <pthread.h>
#include <stdlib.h>

//Estructura PCB
typedef struct{
    int pid;
    int vida;
    int priority;
}PCB;

//estructura ProcessQueue
typedef struct ProcessQueue{
    PCB *queue;
    int front, rear, size;
    int capacity;
    pthread_mutex_t lock;
    pthread_cond_t cond;
}ProcessQueue;

//tercera fase
typedef struct {
    int *entries; // Array de entradas de la TLB (cada entrada almacena un par página-marco)
    int size;     // Tamaño de la TLB (número de entradas)
    int counter;  // Contador de accesos, para reemplazo en la TLB
} TranslationLookasideBuffer;

typedef struct {
    int *tlb;           // Puntero al TLB (Translation Lookaside Buffer)
} MemoryManagementUnit;

//funciones para inicializar las estructuras
void init_process_queue(ProcessQueue *queue,int capacity);
void destroy_process_queue(ProcessQueue *queue);
void insert_process(ProcessQueue *pq, PCB process);



#endif //DATA_STRUCTURES_H