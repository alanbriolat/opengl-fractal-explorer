#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gd.h>

#include "fractal.h"
#include "config.h"

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
    f->pixelsize = complex_new(
            (real(max) - real(min)) / width,
            (imag(max) - imag(min)) / height);
    // Iteration counts
    f->iterations = (int16_t *) calloc(height * width, sizeof(int16_t));
    // Mutex
    pthread_mutex_init(&f->nextpixel_lock, NULL);
    // Next pixel
    f->nextpixel = (int *) calloc(2, sizeof(int));
    f->nextpixel[0] = 0;
    f->nextpixel[1] = 0;
    return f;
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
    pthread_mutex_lock(&f->nextpixel_lock);
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
    pthread_mutex_unlock(&f->nextpixel_lock);
    return pixel;
}

void fractal_destroy(Fractal *f)
{
    pthread_mutex_destroy(&f->nextpixel_lock);
    free(f->iterations);
    free(f->nextpixel);
    free(f);
}

void fractal_writePNG(Fractal *f, char *fname)
{
    // Open the file
    FILE *fp = fopen(fname, "wb");

    printf("Creating image (%d x %d)\n", f->width, f->height);
    gdImagePtr im = gdImageCreateTrueColor(10, 10);

    // Create the GD image
    gdImagePtr gdi = gdImageCreateTrueColor(f->width, f->height);
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

    // Convert the image
    int x, y;
    for ( y = 0; y < f->height; ++y )
    {
        for ( x = 0; x < f->width; ++x )
        {
            if ( FRACTAL_REF(f, x, y) != ITERATIONS )
                gdImageSetPixel(gdi, x, f->height - 1 - y, colors[FRACTAL_REF(f, x, y) % 85]);
        }
    }

    gdImagePng(gdi, fp);
    gdImageDestroy(gdi);
    fclose(fp);
}

complex_t fractal_value(Fractal *f, int x, int y)
{
    return complex_new(
            real(f->min) + ((x + 0.5) * real(f->pixelsize)),
            imag(f->min) + ((y + 0.5) * imag(f->pixelsize)));
}

GLubyte *fractal_bitmap_RGB(Fractal *f)
{
	int color_step = 30;
	int base_val = 15;

    GLubyte colors[85][3];
    GLubyte black[3] = {0, 0, 0};
    
    int j = 0;
	
	//	Blue
	for (int k = 0; k <= 7; ++k)
    {
        j++;
        colors[j][0] = 0;
        colors[j][1] = 0;
        colors[j][2] = base_val + (k * color_step);
    }
	for (int k = 0; k <= 6; ++k)
    {
        j++;
        colors[j][0] = 0;
        colors[j][1] = 0;
        colors[j][2] = 255 - (k * color_step);
    }
	//	Aqua
	for (int k = 1; k <= 7; ++k)
    {
        j++;
        colors[j][0] = 0;
        colors[j][1] = base_val + (k * color_step);
        colors[j][2] = base_val + (k * color_step);
    }
	for (int k = 0; k <= 6; ++k)
    {
        j++;
        colors[j][0] = 0;
        colors[j][1] = 255 - (k * color_step);
        colors[j][2] = 255 - (k * color_step);
    }
	//	Green
	for (int k = 1; k <= 7; ++k)
    {
        j++;
        colors[j][0] = 0;
        colors[j][1] = base_val + (k * color_step);
        colors[j][2] = 0;
    }
	for (int k = 0; k <= 6; ++k)
    {
        j++;
        colors[j][0] = 0;
        colors[j][1] = 255 - (k * color_step);
        colors[j][2] = 0;
    }
	//	Yellow
	for (int k = 1; k <= 7; ++k)
    {
        j++;
        colors[j][0] = base_val + (k * color_step);
        colors[j][1] = base_val + (k * color_step);
        colors[j][2] = 0;
    }
	for (int k = 0; k <= 6; ++k)
    {
        j++;
        colors[j][0] = 255 - (k * color_step);
        colors[j][1] = 255 - (k * color_step);
        colors[j][2] = 0;
    }
	//	Red
	for (int k = 1; k <= 7; ++k)
    {
        j++;
        colors[j][0] = base_val + (k * color_step);
        colors[j][1] = 0;
        colors[j][2] = 0;
    }
	for (int k = 0; k <= 6; ++k)
    {
        j++;
        colors[j][0] = 255 - (k * color_step);
        colors[j][1] = 0;
        colors[j][2] = 0;
    }
	//	Pink
	for (int k = 1; k <= 7; ++k)
    {
        j++;
        colors[j][0] = base_val + (k * color_step);
        colors[j][1] = 0;
        colors[j][2] = base_val + (k * color_step);
    }
	for (int k = 0; k <= 6; ++k)
    {
        j++;
        colors[j][0] = 255 - (k * color_step);
        colors[j][1] = 0;
        colors[j][2] = 255 - (k * color_step);
    }

    // Allocate the memory
    GLubyte *bitmap = (GLubyte *) malloc(f->width * f->height * 3 * sizeof(GLubyte));
    GLubyte *ptr = bitmap;

    for ( int y = 0; y < f->height; ++y )
    {
        for ( int x = 0; x < f->width; ++x )
        {
            if ( FRACTAL_REF(f, x, y) == ITERATIONS )
                memcpy(ptr, black, 3 * sizeof(GLubyte));
            else
                memcpy(ptr, colors[FRACTAL_REF(f, x, y) % 85], 3 * sizeof(GLubyte));
            ptr += 3 * sizeof(GLubyte);
        }
    }
    
    return bitmap;
}

Fractal *fractal_zoom(Fractal *source, int xoffset, int yoffset, float scale)
{
    complex_t center = complex_new(
            ((real(source->min) + real(source->max)) / 2) + (xoffset * real(source->pixelsize)),
            ((imag(source->min) + imag(source->max)) / 2) + (yoffset * imag(source->pixelsize)));
}
