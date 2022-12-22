#ifndef CONCURRENT_COUNTER_H
#define CONCURRENT_COUNTER_H

#include <pthread.h>

// Number of threads
#define NUM_THREADS 5

// Global counter variable
extern int counter;

// Mutex to synchronize access to the counter
extern pthread_mutex_t mutex;

// Thread function
void *worker(void *arg);

#endif
