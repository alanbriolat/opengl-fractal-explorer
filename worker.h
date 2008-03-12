#ifndef _WORKER_H_
#define _WORKER_H_

#include "complex.h"
#include "queue.h"

void    runqueue(Queue *q);
void    *worker(void *q);
int     iterate(complex_t c);

#endif
