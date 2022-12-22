#ifndef hand_list_h
#define hand_list_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "mythreads.h"
#define MILLION 1000000
#define BILLION 1000000000L

long meas_hohlist (int iterations, int numthreads);
void *worker (void *args);

typedef struct __node_t {
    int             key;
    pthread_mutex_t lock;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t      *head;
} list_t;

typedef struct __wargs_t {
    list_t *L;
    int iterations;
} wargs_t;

void List_Init (list_t *L) {
    L->head = NULL;
}

// search
int List_Lookup(list_t *L, int key) {

    node_t *curr = L->head;
    Pthread_mutex_lock(&curr->lock);

    while (curr) {
        if (curr->key == key) {
            Pthread_mutex_unlock(&curr->lock);
            return 0; // success
        }

        if (curr->next != NULL) {
            Pthread_mutex_lock(&curr->next->lock);
        }
        
        Pthread_mutex_unlock(&curr->lock);
        curr = curr->next;
    }
    Pthread_mutex_unlock(&curr->lock);
    return -1; // failure
}

// insert
int List_Insert (list_t *L, int key) {
    // check if key is already in list
    //if (List_Lookup(L, key) == 0) return -1;
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL) {
        fprintf(stderr, "error allocating a new node\n");
        return -1;
    }
    new->key = key;
    Pthread_mutex_init (&new->lock, NULL);
    // locked section
    Pthread_mutex_lock(&new->lock);
    new->next = L->head;
    L->head = new;
    Pthread_mutex_unlock(&new->lock);
    return 0;
}


void print_result(list_t *L) {
    node_t *curr = L->head;
    while (curr->next != NULL)
    {
        printf("%d, ", curr->key);
        curr = curr->next;
    }
    printf("NULL\n");
}

#endif /* hoh_list_h */
