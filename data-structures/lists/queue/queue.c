#include "queue.h"
#include "../../utils/set.h"

#include <stdio.h>
#include <stdlib.h>

struct queue queue_init(void (*custom_free)(void *data), void (*print)(void *data))
{
    struct queue queue;
    queue.first = NULL;
    queue.last = NULL;
    queue.free = custom_free;
    queue.print = print;
    return queue;
}

void queue_destroy(struct queue *queue)
{
    while (queue->first != NULL) {
        queue_pop(queue);
    }
}

void queue_pop(struct queue *queue)
{
    if (queue->first->next != NULL) {
        queue->first->next->prev = NULL;
    }
    struct node *second = queue->first->next;
    node_destroy(queue->first);
    queue->first = second;
}

void queue_push(struct queue *queue, void *data, size_t size, void (*custom_free)(void *data), void (*print)(void *data))
{
    void (*free_function) = set_function(custom_free, queue->free);
    void (*print_function) = set_function(print, queue->print);
    struct node *node = node_init(data, size, free_function, print_function);
    if (queue->last == NULL) {
        queue->last = queue->first = node;
        return;
    }
    node->prev = queue->last;
    queue->last->next = node;
    queue->last = node;
}

void *queue_peek(struct queue *queue)
{
    return queue->first->data;
}

void queue_print(struct queue *queue)
{
    struct node *cursor = queue->first;
    while (cursor != NULL) {
        node_print(cursor, NULL);
        cursor = cursor->next;
    }
}

void queue_print_int(void *data)
{
    printf("%d ", *(int *)data);
}
