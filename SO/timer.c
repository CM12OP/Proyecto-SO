#include "timer.h"
#include <stdio.h>
#include <unistd.h>

pthread_cond_t timer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;
extern int working;

void *timer_thread(){
    while(working){
         pthread_mutex_lock(&timer_mutex);
        pthread_cond_wait(&timer_cond, &timer_mutex); //espera la señal del clock
        pthread_mutex_unlock(&timer_mutex);

        printf("Timer tick - sending interrupt signal\n");
    }
    return NULL;
}