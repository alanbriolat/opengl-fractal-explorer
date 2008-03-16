#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "complex.h"
//#include "queue.h"
#include "ui.h"
#include "fractal.h"
#include "worker.h"


int main(int argc, char **argv)
{
    Fractal *f = fractal_new(512, 512, complex_new(-2, -2), complex_new(2, 2));
    runfractal(f);
    texture = fractal_bitmap_RGB(f);
    ui_init(&argc, argv, 400, 400);
    ui_run();
    return 1;
}
