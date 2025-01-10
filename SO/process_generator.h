#ifndef PROCESS_GENERATOR_H
#define PROCESS_GENERATOR_H

#include "data_structures.h"
void *process_generator_thread(void *arg);
void *priority_process_generator_thread(void *arg);
#endif //PROCESS_GENERATOR_H