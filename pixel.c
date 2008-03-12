#include <stdlib.h>

#include "pixel.h"
#include "complex.h"

Pixel *pixel_new(complex_t min, complex_t max, int iterations)
{
    Pixel *p = (Pixel *) malloc(sizeof(Pixel));
    p->min = min;
    p->max = max;
    p->i = iterations;

    return p;
}
