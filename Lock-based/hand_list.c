#include "hand_list.h"

long meas_hohlist (int iterations, int numthreads) {

    // list and list initialization
    list_t mylist;
    List_Init(&mylist);

    // arguments for the threads in worker method
    wargs_t myargs;
    myargs.iterations = iterations;
    myargs.L = &mylist;

    // timer initialization
    struct timespec begin, end;

    // generating numberthread threads
    pthread_t *threads = malloc(sizeof(pthread_t) * numthreads);

     // start timer and test
    clock_gettime(CLOCK_MONOTONIC, &begin);

    for (int i = 0; i < numthreads; i++)
    {
        Pthread_create(&threads[i], NULL, &worker, &myargs);
    }
    for (int i = 0; i < numthreads; i++)
    {
        Pthread_join(threads[i], NULL);
    }

    // stop timer
    clock_gettime(CLOCK_MONOTONIC, &end);

    //print_result(&mylist);

    // elapsed time in nanoseconds
    return (BILLION * (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec));
}


void *worker (void *args) {
    wargs_t *thisargs = args;
    for (int i = 0; i < thisargs->iterations; i++)
    {
        List_Insert(thisargs->L, i);
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    int opt;
    // default values
    int iterations = 10;
    int numthreads = 4;

    // Eigene Eingaben
    while ((opt = getopt(argc, argv, "hi:n:")) != -1)
        switch (opt) {
            case 'h':
                printf("Options:\n-i ITERATIONS\n   iterations per thread\n");
                printf("-n NUMBER_THREADS\n     number of threads\n");
                break;
            case 'i':
                iterations = atoi(optarg);
                break;
            case 'n':
                numthreads = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-i <iterations>] [-n <number of threads>]\n", argv[0]);
                exit(1);
        }
    // Standartisiertes Verfahren
    long elapsed;
    for (int i = 1; i < 5; i++)
    {
        for (int j = 0; j <= 100000; j += 20000)
        {
            elapsed = meas_hohlist(j, i);
            printf("elapsed with %d threads, each %d iterations: %ld ns (%.3f ms)\n", i, j, elapsed, (float) elapsed / MILLION);
        }
        
    }
    
    elapsed = meas_hohlist(iterations, numthreads);
    printf("elapsed with %d threads, each %d iterations: %ld ns (%.3f ms)\n", numthreads, iterations, elapsed, (float) (elapsed / MILLION));
    return 0;
}
