#include <stdlib.h>

#include "newworker.h"

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
        f->iterations[pixel[1] * f->width + pixel[0]] = 
            iterate(complex_new());
