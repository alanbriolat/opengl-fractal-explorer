#include <stdlib.h>
#include <stdio.h>
#include <gd.h>

#include "config.h"
#include "image.h"
#include "pixel.h"

Image *image_new(int width, int height, complex_t min, complex_t max)
{
    Image *i = (Image *) malloc(sizeof(Image));
    i->width = width;
    i->height = height;
    i->min = min;
    i->max = max;
    i->pixelsize = complex_new(real(max) - real(min), imag(max) - imag(min));
    i->pixels = (Pixel **) calloc(height * width, sizeof(Pixel *));

    return i;
}

void image_generate(Image *i, Queue *pixelqueue)
{
    Pixel *p;
    _complex_t p_rsize = (real(i->max) - real(i->min)) / i->width;
    _complex_t p_isize = (imag(i->max) - imag(i->min)) / i->height;
    int x, y;
    for (y = 0 ; y < i->height ; y++)
        for (x = 0 ; x < i->width ; x++)
        {
            p = pixel_new(
                    complex_new(
                        real(i->min) + (x * p_rsize),
                        imag(i->min) + (y * p_isize)),
                    complex_new(
                        real(i->min) + ((x+1) * p_rsize),
                        imag(i->min) + ((y+1) * p_isize)),
                    0);
            image_setpixel(i, x, y, p);
            queue_add(pixelqueue, image_getpixel(i, x, y));
        }
}


void image_setpixel(Image *i, int x, int y, Pixel *p)
{
    i->pixels[y * i->width + x] = p;
}

Pixel *image_getpixel(Image *i, int x, int y)
{
    return i->pixels[y * i->width + x];
}

Image *image_interpolate(Image *i, Queue *pixelqueue)
{
    Image *new = image_new(i->width * 2, i->height * 2, i->min, i->max);
    
    int x, y;
    for (y = 0 ; y < i->height ; y++)
        for (x = 0 ; x < i->width ; x++)
        {
            Pixel *p = image_getpixel(i, x, y);
            Pixel *p2;

            complex_t mid = complex_mean(p->min, p->max);

            //printf("Min (%f,%f), Max (%f,%f), Mid (%f, %f)\n");

            /*
             *  +----+----+
             *  | HL | HH |
             *  +----+----+
             *  | LL | LH |
             *  +----+----+
             */

            // LL
            p2 = pixel_new(p->min, mid, p->i);
            image_setpixel(new, x*2, y*2, p2);
            queue_add(pixelqueue, p2);

            // LH
            p2 = pixel_new(complex_new(real(mid), imag(p->min)),
                           complex_new(real(p->max), imag(mid)),
                           p->i);
            image_setpixel(new, x*2 + 1, y*2, p2);
            queue_add(pixelqueue, p2);

            // HL
            p2 = pixel_new(complex_new(real(p->min), imag(mid)),
                           complex_new(real(mid), imag(p->max)),
                           p->i);
            image_setpixel(new, x*2, y*2 + 1, p2);
            queue_add(pixelqueue, p2);

            // HH
            p2 = pixel_new(mid, p->max, p->i);
            image_setpixel(new, x*2 + 1, y*2 + 1, p2);
            queue_add(pixelqueue, p2);
            
            free(p);
        }

    return new;
}

void image_writePNG(Image *i, char *fname)
{
    // Open the file
    FILE *fp = fopen(fname, "wb");

    printf("Creating image (%d x %d)\n", i->width, i->height);
    gdImagePtr im = gdImageCreateTrueColor(10, 10);

    // Create the GD image
    gdImagePtr gdi = gdImageCreateTrueColor(i->width, i->height);
    // Add black and white
    int black = gdImageColorAllocate(gdi, 0, 0, 0);
    int white = gdImageColorAllocate(gdi, 255, 255, 255);

	int color_step = 30;
	int base_val = 15;

    int colors[85];
    int j = 0;
	
	//	Blue
	for (int k = 0; k <= 7; ++k) colors[j++] = gdImageColorAllocate(gdi, 0, 0, base_val + (k*color_step));
	for (int k = 0; k <= 6; ++k) colors[j++] = gdImageColorAllocate(gdi, 0, 0, 225 - (k*color_step));
	//	Aqua
	for (int k = 1; k <= 7; ++k) colors[j++] = gdImageColorAllocate(gdi, 0, base_val + (k*color_step), base_val + (k*color_step));
	for (int k = 0; k <= 6; ++k) colors[j++] = gdImageColorAllocate(gdi, 0, 225 - (k*color_step), 225 - (k*color_step));
	//	Green
	for (int k = 1; k <= 7; ++k) colors[j++] = gdImageColorAllocate(gdi, 0, base_val + (k*color_step), 0);
	for (int k = 0; k <= 6; ++k) colors[j++] = gdImageColorAllocate(gdi, 0, 225 - (k*color_step), 0);
	//	Yellow
	for (int k = 1; k <= 7; ++k) colors[j++] = gdImageColorAllocate(gdi, base_val + (k*color_step), base_val + (k*color_step), 0);
	for (int k = 0; k <= 6; ++k) colors[j++] = gdImageColorAllocate(gdi, 225 - (k*color_step), 225 - (k*color_step), 0);
	//	Red
	for (int k = 1; k <= 7; ++k) colors[j++] = gdImageColorAllocate(gdi, base_val + (k*color_step), 0, 0);
	for (int k = 0; k <= 6; ++k) colors[j++] = gdImageColorAllocate(gdi, 225 - (k*color_step), 0, 0);
	//	Pink
	for (int k = 1; k <= 7; ++k) colors[j++] = gdImageColorAllocate(gdi, base_val + (k*color_step), 0, base_val + (k*color_step));
	for (int k = 0; k <= 6; ++k) colors[j++] = gdImageColorAllocate(gdi, 225 - (k*color_step), 0, 225 - (k*color_step));

    printf("%d\n", j);

    // Convert the image
    int x, y;
    for ( y = 0; y < i->height; ++y )
    {
        for ( x = 0; x < i->width; ++x )
        {
            Pixel *p = image_getpixel(i, x, y);
            if ( p->i != ITERATIONS )
                gdImageSetPixel(gdi, x, i->height - 1 - y, colors[p->i % 85]);
        }
    }

    gdImagePng(gdi, fp);
    gdImageDestroy(gdi);
    fclose(fp);
}
