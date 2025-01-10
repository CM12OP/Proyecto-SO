#include "data_structures.h"


void init_process_queue(ProcessQueue *pq, int capacity){
    pq->queue = (PCB *)malloc(sizeof(PCB) * capacity);
    pq->front = pq->rear = pq->size = 0;
    pq->capacity = capacity;
    pthread_mutex_init(&pq->lock, NULL);
    pthread_cond_init(&pq->cond, NULL);
}

void destroy_process_queue(ProcessQueue *pq){
    free(pq->queue);
    pthread_mutex_destroy(&pq->lock);
    pthread_cond_destroy(&pq->cond);
}

void insert_process(ProcessQueue *pq, PCB process) {
    pthread_mutex_lock(&pq->lock);
    if (pq->size == pq->capacity) {
        pthread_mutex_unlock(&pq->lock);
        return;  // Cola llena
    }
    
    int i;
    for (i = pq->size - 1; i >= 0; i--) {
        if (pq->queue[(pq->front + i) % pq->capacity].priority <= process.priority) {
            break;
        }
        pq->queue[(pq->front + i + 1) % pq->capacity] = pq->queue[(pq->front + i) % pq->capacity];
    }
    pq->queue[(pq->front + i + 1) % pq->capacity] = process;
    pq->size++;
    pq->rear = (pq->rear + 1) % pq->capacity;
    
    pthread_cond_signal(&pq->cond);
    pthread_mutex_unlock(&pq->lock);
}
