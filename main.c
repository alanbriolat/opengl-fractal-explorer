#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "complex.h"
#include "queue.h"
#include "ui.h"


int main(int argc, char **argv)
{
    ui_init(&argc, argv, 400, 400);
    ui_run();
    return 1;
}
