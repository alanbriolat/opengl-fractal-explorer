#ifndef _QUEUE_H_
#define _QUEUE_H_

struct QueueEl_ {
    struct QueueEl_ *next;
    void *item;
};
typedef struct QueueEl_ QueueEl;

typedef struct {
    QueueEl *head, *tail;
} Queue;

Queue   *queue_new();
int     queue_empty(Queue *q);
void    queue_add(Queue *q, void *item);
void    *queue_remove(Queue *q);

#endif
