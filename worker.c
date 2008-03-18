#include <stdlib.h>
#include <stdio.h>

#include "worker.h"
#include "config.h"
#include "fractal.h"

int iterate(complex_t c)
{
    complex_t z = complex_new(0, 0);
    int i;
    for ( i = 0; i < ITERATIONS && mag(z) < THRESHOLD; ++i )
        z = (z * z) + c;
    return i;
}

void *worker(void *fractal)
{
    Fractal *f = (Fractal *) fractal;

    int *pixel;
    while ( (pixel = fractal_nextpixel(f)) != NULL )
    {
        FRACTAL_REF(f, pixel[0], pixel[1]) = 
            iterate(fractal_value(f, pixel[0], pixel[1]));
        /*fprintf(stderr, "Calculating (%f, %f) (%d, %d) = %d\n", 
                real(FRACTAL_VALUE(f, pixel[0], pixel[1])),
                imag(FRACTAL_VALUE(f, pixel[0], pixel[1])),
                pixel[0], pixel[1], FRACTAL_REF(f, pixel[0], pixel[1]));*/
        free(pixel);
    }
    pthread_exit(NULL);
}

void runfractal(Fractal *f)
{
    pthread_t threads[THREADCOUNT];
    int rc;

    fractal_restartcalc(f);

    for ( int i = 0; i < THREADCOUNT; ++i )
    {
        pthread_create(&threads[i], NULL, worker, f);
        printf("Worker thread %d created\n", i);
    }

    void *status;
    for ( int i = 0; i < THREADCOUNT; ++i )
    {
        pthread_join(threads[i], &status);
        printf("Worker thread %d terminated\n", i);
    }
}
