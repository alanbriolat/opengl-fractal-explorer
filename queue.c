#include <assert.h>
#include <stdlib.h>

#include "queue.h"

Queue *queue_new()
{
    Queue *q = (Queue *) malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

int queue_empty(Queue *q)
{
    return q->head == NULL;
}

void queue_add(Queue *q, void *item)
{
    QueueEl *e = (QueueEl *) malloc(sizeof(QueueEl));
    e->next = NULL;
    e->item = item;
    
    if (q->head == NULL)
        q->head = e;
    else
        q->tail->next = e;

    q->tail = e;
}

void *queue_remove(Queue *q)
{
    assert(!queue_empty(q));

    //  Get the queued item
    void *item = q->head->item;

    //  Remove and free the queue slot
    QueueEl *e = q->head;
    q->head = q->head->next;
    free(e);

    return item;
}
