#ifndef CONCURRENT_COUNTER_H
#define CONCURRENT_COUNTER_H

#include <pthread.h>

#define NUM_THREADS 5

extern int counter;

extern pthread_mutex_t mutex;

void *worker(void *arg);

#endif
