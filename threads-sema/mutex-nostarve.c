#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct _ns_mutex_t {
    sem_t mutex;
    sem_t queue;
    int held;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    sem_init(&m->mutex, 0, 1);
    sem_init(&m->queue, 0, 0);
}

void ns_mutex_acquire(ns_mutex_t *m) {
    sem_wait(&m->mutex);
    if (m->held) {
        sem_post(&m->mutex);
        sem_wait(&m->queue);
        sem_wait(&m->mutex);
    }
    m->held = 1;
    sem_post(&m->mutex);
}

void ns_mutex_release(ns_mutex_t *m) {
    sem_wait(&m->mutex);
    m->held = 0;
    sem_post(&m->queue);
    sem_post(&m->mutex);
}


void *worker(void *arg) {
    ns_mutex_t *m = (ns_mutex_t *)arg;
    ns_mutex_acquire(m);
    printf("Thread %ld acquired the mutex\n", (long)pthread_self());
    sleep(1);
    ns_mutex_release(m);
    printf("Thread %ld released the mutex\n", (long)pthread_self());
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("parent: begin\n");
    ns_mutex_t m;
    ns_mutex_init(&m);
    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, &worker, &m);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("parent: end\n");
    return 0;
}