#ifndef queue_h
#define queue_h

#include "../../node/node.h"

#include <stddef.h>

struct queue
{
    struct node *first;
    struct node *last;
    void (*free)(void *data);
    void (*print)(void *data);
};

void queue_push(struct queue *queue, void *data, size_t size, void (*custom_free)(void *data), void (*print)(void *data));
void queue_pop(struct queue *queue);
void *queue_peek(struct queue *queue);
void queue_print(struct queue *queue);

// Public helper print function
void queue_print_int(void *data);

struct queue queue_init(void (*custom_free)(void *data), void (*print)(void *data));
void queue_destroy(struct queue *queue);

#endif /* queue_h */