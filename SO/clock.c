#include "clock.h"
#include <stdio.h>
#include <unistd.h>

extern pthread_cond_t timer_cond;
extern pthread_mutex_t timer_mutex;
extern int working;

void *clock_thread(){
    while(working){
        printf("Clock tick\n");
        usleep(10000); //Simula un ciclo de reloj
        pthread_mutex_lock(&timer_mutex);
        pthread_cond_signal(&timer_cond); //Notifica a todo
        pthread_mutex_unlock(&timer_mutex);

        printf("Clock tick\n");
    
    }
    return NULL;
} 