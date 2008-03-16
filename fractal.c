#include <stdlib.h>

#include "fractal.h"

Fractal *fractal_new(int width, int height, complex_t min, complex_t max)
{
    Fractal *f = (Fractal *) malloc(sizeof(Fractal));
    // Dimensions
    f->width = width;
    f->height = height;
    // Range
    f->min = min;
    f->max = max;
    // Pixel dimension
    f->pixelsize = complex_new(real(max) - real(min), imag(max) - imag(min));
    // Iteration counts
    f->iterations = (int16_t *) calloc(height * width, sizeof(int16_t));
    // Mutex
    pthread_mutex_init(f->nextpixel_lock, NULL);
    // Next pixel
    f->nextpixel = (int *) calloc(2, sizeof(int));
    f->nextpixel[0] = 0;
    f->nextpixel[1] = 0;
}

void fractal_restartcalc(Fractal *f)
{
    if ( f->nextpixel == NULL )
        f->nextpixel = (int *) calloc(2, sizeof(int));
    f->nextpixel[0] = 0;
    f->nextpixel[1] = 0;
}

int *fractal_nextpixel(Fractal *f)
{
    pthread_mutex_lock(f->nextpixel_lock);
    int *pixel = f->nextpixel;
    if ( pixel != NULL )
    {
        int x = pixel[0];
        int y = pixel[1];
        x++;
        if ( x >= f->width )
        {
            x = 0;
            y++;
        }
        if ( y >= f->height )
        {
            f->nextpixel = NULL;
        }
        else
        {
            f->nextpixel = (int *) malloc(2 * sizeof(int));
            f->nextpixel[0] = x;
            f->nextpixel[1] = y;
        }
    }
    pthread_mutex_unlock(f->nextpixel_lock);
    return pixel;
}

void fractal_destroy(Fractal *f)
{
    pthread_mutex_destroy(f->nextpixel_lock);
    free(f->iterations);
    free(f->nextpixel);
    free(f);
}
