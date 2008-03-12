#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "complex.h"
#include "queue.h"
#include "image.h"
#include "worker.h"


int main(int argc, char **argv)
{
    // Make a pixel queue
    Queue *q = queue_new();

    // Generate a small image
    Image *i = image_new(5000, 5000, complex_new(-2.0, -2.0), complex_new(2.0, 2.0));
    image_generate(i, q);
    runqueue(q);
    image_writePNG(i, "foo-1.png");

    return 0;
}
