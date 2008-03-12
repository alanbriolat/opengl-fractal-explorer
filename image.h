#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "complex.h"
#include "pixel.h"
#include "queue.h"

typedef struct {
    //  Dimensions of the image
    int height, width;
    //  Dimensions of a pixel
    complex_t pixelsize;
    //  The range of the display
    complex_t min, max;
    //  The pixel array
    Pixel **pixels;
} Image;

/**
 * Create a new (but uncalculated) image for the area between min and max
 */
Image   *image_new(int height, int width, complex_t min, complex_t max);

/**
 * (Re)generate an image - recalculate all pixels using the supplied pixel
 * queue
 */
void    image_generate(Image *i, Queue *pixelqueue);

/**
 * Set the pixel at a particular location in the pixel array
 */
void    image_setpixel(Image *i, int x, int y, Pixel *p);

/**
 * Get the pixel from a particular location in the pixel array
 */
Pixel   *image_getpixel(Image *i, int x, int y);

/**
 * Double an image's size by interpolation, adding all new pixels to the 
 * supplied queue
 */
Image   *image_interpolate(Image *i, Queue *pixelqueue);

/**
 * Write the image as a PNG
 */
void    image_writePNG(Image *i, char *fname);

#endif
