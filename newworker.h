#ifndef _WORKER_H_
#define _WORKER_H_

#include "fractal.h"

void    runfractal(Fractal *f);
void    *worker(void *f);
int     iterate(complex_t c);

#endif
