#include "node.h"
#include "../utils/set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy

struct node *node_init(void *data, size_t size, void (*custom_free)(void *data), void (*print)(void *data))
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->prev = NULL;
    node->next = NULL;
    node->data = malloc(size);
    memcpy(node->data, data, size);
    node->size = size;
    node->free = custom_free;
    node->print = print;
    return node;
}

void node_destroy(struct node *node)
{
    if (node->free != NULL) {
        node->free(node->data);
    }
    free(node->data);
    free(node);
}

void node_print(struct node *node, void (*print)(void *data))
{
    void (*print_function)(void *data) = set_function(print, node->print);
    print_function(node->data);
}
