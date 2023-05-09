#include "tuple.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct tuple tuple_init(int n_items, ...)
{
    size_t item_size;
    struct tuple tuple;
    tuple.n_items = n_items;
    tuple.data = (void **)malloc(n_items * sizeof(void *));
    tuple.print = (void (**)(void *))malloc(n_items * sizeof(void (**)(void *)));
    va_list data;
    va_start(data, n_items);
    for (int i = 0; i < n_items; i++) {
        item_size = va_arg(data, size_t);
        tuple.data[i] = malloc(item_size);
        memcpy(tuple.data[i], va_arg(data, void *), item_size);
        tuple.print[i] = va_arg(data, void (*)(void *));
    }
    va_end(data);
    return tuple;
}

void tuple_destroy(struct tuple *tuple)
{
    for (int i = 0; i < tuple->n_items; i++) {
        free(tuple->data[i]);
    }
    free(tuple->data);
    free(tuple->print);
}

void *tuple_at(struct tuple *tuple, unsigned int at)
{
    return tuple->data[at];
}

void tuple_print(struct tuple *tuple)
{
    for (int i = 0; i < tuple->n_items; i++) {
        if (tuple->print != NULL) {
            tuple->print[i](tuple->data[i]);
        }
    }
}