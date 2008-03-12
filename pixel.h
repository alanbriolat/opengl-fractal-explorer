#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "complex.h"

typedef struct {
    //  The range of values the pixel represents
    complex_t min, max;
    //  Number of iterations to escape
    int i;
} Pixel;

//  Constructor
Pixel *pixel_new(complex_t min, complex_t max, int iterations);

#endif
