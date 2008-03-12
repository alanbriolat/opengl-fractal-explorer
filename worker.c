#include <stdio.h>
#include <pthread.h>

#include "config.h"
#include "complex.h"
#include "pixel.h"
#include "queue.h"

pthread_mutex_t pixelqueuelock;

int iterate(complex_t c)
{
    complex_t z = complex_new(0, 0);
    int i;
    for ( i = 0; i < ITERATIONS && mag(z) < THRESHOLD; ++i )
        z = (z * z) + c;
    return i;
}

void *worker(void *queue)
{
    Queue *q = (Queue *) queue;

    pthread_mutex_lock(&pixelqueuelock);
    while (!queue_empty(q))
    {
        Pixel *p = (Pixel *) queue_remove(q);
        pthread_mutex_unlock(&pixelqueuelock);
        p->i = iterate(complex_mean(p->min, p->max));
        pthread_mutex_lock(&pixelqueuelock);
    }
    pthread_mutex_unlock(&pixelqueuelock);
    pthread_exit(NULL);
}

void runqueue(Queue *q)
{
    pthread_t threads[THREADCOUNT];
    int rc;

    pthread_mutex_init(&pixelqueuelock, NULL);
    
    int j;
    for ( j = 0; j < THREADCOUNT; ++j )
    {
        pthread_create(&threads[j], NULL, worker, q);
        printf("Worker thread %d created\n", j);
    }

    void *status;
    for ( j = 0; j < THREADCOUNT; ++j )
    {
        pthread_join(threads[j], &status);
        printf("Worker thread %d terminated\n", j);
    }

    pthread_mutex_destroy(&pixelqueuelock);
}
