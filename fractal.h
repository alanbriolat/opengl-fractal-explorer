#ifndef _FRACTAL_H_
#define _FRACTAL_H_

#include <pthread.h>
#include <stdint.h>

#include "complex.h"

#define FRACTAL_REF(f, x, y)    f->iterations[(y * f->width) + x]

typedef struct {
    // Dimensions of the image
    int width, height;
    // The range covered by the image
    complex_t min, max;
    // Dimensions of a single pixel
    complex_t pixelsize;
    // Array of iteration counts
    int16_t *iterations;
    // "Next pixel" lock for multi-threaded calculation
    pthread_mutex_t nextpixel_lock;
    // {X, Y} of next pixel, or NULL if the image is complete
    int *nextpixel;
} Fractal;

// Allocate and initialise a Fractal object
Fractal *fractal_new(int width, int height, complex_t min, complex_t max);

// Destroy the Fractal object
void fractal_destroy(Fractal *f);

// Prepare the fractal to be recalculated, resetting the "next pixel"
void fractal_restartcalc(Fractal *f);

// Return a pointer to a 2-item vector of {X, Y} for the next pixel that needs
// calculating, or return NULL if the image is finished
int *fractal_nextpixel(Fractal *f);

// Get the position on the complex plane for the specified 
inline complex_t fractal_value(Fractal *f, int x, int y);

// Write the fractal as a PNG image to fname
void fractal_writePNG(Fractal *f, char *fname);

#endif
