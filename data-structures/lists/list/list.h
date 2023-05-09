#ifndef list_h
#define list_h

#include "../../node/node.h"

#include <stddef.h> // size_t

struct list
{
    struct node *head;
    struct node *tail;
    int (*compare)(void *data1, void *data2, size_t size);
    void (*free)(void *data);
    void (*print)(void *data);
    int length;
};

int list_append(struct list *list, void *data, size_t size, void (*free_data)(void *data), void (*print)(void *data));
int list_insert(struct list *list, int at, void *data, size_t size, void (*free)(void *data), void (*print)(void *data));

void *list_retrieve(struct list *list, int at);

int list_pop_head(struct list *list);
int list_pop_tail(struct list *list);
int list_remove_from(struct list *list, int from);
int list_remove_data(struct list *list, void *data, size_t size);
int list_print(struct list *list);
void list_quicksort(struct list *list, size_t size);

// Public helper functions
void list_print_int(void *data);
int list_compare_int(void *data1, void *data2, size_t _);

// Constructor
struct list list_init(int (*compare)(void *data1, void *data2, size_t size), void (*free)(void *data), void (*print)(void *data));
// Destructor
int list_destroy(struct list *list);

#endif /* list_h */