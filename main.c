#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "complex.h"
//#include "queue.h"
//#include "ui.h"
#include "fractal.h"
#include "newworker.h"


int main(int argc, char **argv)
{
    /*ui_init(&argc, argv, 400, 400);
    ui_run();
    return 1;*/
    Fractal *f = fractal_new(1000, 1000, complex_new(-2, -2), complex_new(2, 2));
    runfractal(f);
    fractal_writePNG(f, "foobar2.png");
    return 0;
}
