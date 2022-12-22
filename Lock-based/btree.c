#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "btree.h"

#define NUM_THREADS 15

void *insert_thread(void *arg) {
  BTree *tree = (BTree*) arg;
  int value = rand() % 100;
btree_insert(tree, value);
printf("Inserted value: %d\n", value);
return NULL;
}

int main() {
  struct timeval start, end;
  gettimeofday(&start, NULL);
  BTree tree;
  btree_init(&tree);

  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, insert_thread, &tree);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

    gettimeofday(&end, NULL);

  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

  printf("Elapsed time: %f seconds\n", elapsed);
  btree_destroy(&tree);
  return 0;
}