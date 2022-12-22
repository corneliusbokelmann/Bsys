#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "counter.h"


#define NUM_THREADS 5
#define LIMIT 10000

int counter = 0;

pthread_mutex_t mutex;

void *worker(void *arg) {
  pthread_mutex_lock(&mutex);

  counter++;

  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

int main(void) {
  struct timeval start, end;
  gettimeofday(&start, NULL);
  pthread_t threads[NUM_THREADS];

  pthread_mutex_init(&mutex, NULL);
  for (int c = 0; c < LIMIT; c++){ 
    for (int i = 0; i < NUM_THREADS; i++) {
      int ret = pthread_create(&threads[i], NULL, worker, NULL);
      if (ret != 0) {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
      }
    }
   

    for (int i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
    }
  } 

  gettimeofday(&end, NULL);

  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

  printf("Counter: %d\n", counter);
  printf("Elapsed time: %f seconds\n", elapsed);

  pthread_mutex_destroy(&mutex);

  return 0;
}