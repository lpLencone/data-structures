#include "list.h"
#include "../../utils/set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *__retrieve_node_at(struct list *list, int at);
struct node *__retrieve_node_by_data(struct list *list, void *data, size_t size);
void __remove_node(struct list *list, struct node *node);
void __quicksort_recursive(struct node *start, struct node *end, int (*compare)(void *, void *, size_t), size_t size);

struct list list_init(int (*compare)(void *, void *, size_t), void (*free)(void *), void (*print)(void *))
{
    struct list list;
    list.head = NULL;
    list.tail = NULL;
    list.length = 0;
    list.compare = compare;
    list.print = print;
    list.free = free;
    return list;
}

int list_destroy(struct list *list)
{
    if (list == NULL) {
        return 1;
    }
    if (list->length != 0) {
        struct node *cursor = list->head;
        struct node *temp;
        while (cursor != NULL) {
            temp = cursor->next;
            node_destroy(cursor);
            cursor = temp;
        }
    }
    return 0;
}

int list_insert(struct list *list, int at, void *data, size_t size, void (*free)(void *), void (*print)(void *))
{
    if (list == NULL      || 
        at > list->length || 
        at < 0) {
        return 1;
    }
    
    // Set free function
    void (*free_function) = set_function(free, list->free);

    // Set print function
    void (*print_function) = set_function(print, list->print);
    
    // Initialize node
    struct node *node = node_init(data, size, free_function, print_function);
    
    // Insert first instance on the list
    if (list->length == 0) {
        list->head = list->tail = node;
        list->length++;
        return 0;
    }
    
    // Find cursor before position asked for
    struct node *cursor = __retrieve_node_at(list, at - 1);

    // Insert in the head of the list
    if (cursor == NULL) {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
        list->length++;
        return 0;
    }

    if (cursor->next == NULL) {
        list->tail = node;
    }
    else {
        cursor->next->prev = node;
    }
    node->prev = cursor;
    node->next = cursor->next;
    cursor->next = node;
    list->length++;

    return 0;
}

int list_append(struct list *list, void *data, size_t size, void (*free)(void *), void (*print)(void *))
{
    if (list == NULL) {
        return 1;
    }
    list_insert(list, list->length, data, size, free, print);
    return 0;
}

void *list_retrieve(struct list *list, int at)
{
    if (list == NULL      || 
        at > list->length || 
        at < 0 ) {
        return NULL;
    }
    struct node *node = __retrieve_node_at(list, at);
    return node->data;
}

int list_pop_head(struct list *list)
{
    if (list == NULL || list->length == 0) {
        return 1;
    }
    __remove_node(list, list->head);
    return 0;
}

int list_pop_tail(struct list *list)
{
    if (list == NULL ||list->length == 0) {
        return 1;
    }
    __remove_node(list, list->tail);
    return 0;
}

int list_remove_from(struct list *list, int at)
{
    if (list == NULL       ||
        list->length == 0  ||
        at >= list->length ||
        at < 0) {
        return 1;
    }
    struct node *node = __retrieve_node_at(list, at);
    __remove_node(list, node);
    return 0;
}

int list_remove_data(struct list *list, void *data, size_t size)
{
    if (list == NULL || list->length == 0) {
        return 1;
    }
    struct node *node = __retrieve_node_by_data(list, data, size);
    if (node == NULL) {
        return 1;
    }
    __remove_node(list, node);
    return 0;
}

int list_print(struct list *list)
{
    if (list == NULL || list->length == 0) {
        return 1;
    }
    struct node *cursor = list->head;
    while (cursor != NULL) {
        node_print(cursor, NULL);
        cursor = cursor->next;
    }
    return 0;
}

void list_quicksort(struct list *list, size_t size)
{
    __quicksort_recursive(list->head, list->tail, list->compare, size);
}

// Helper print function
void list_print_int(void *data)
{
    printf("%d ", *(int *)data);
}

// Helper compare function
int list_compare_int(void *data1, void *data2, size_t _){
    int int1 = *(int *)data1;
    int int2 = *(int *)data2;

    if (int1 < int2) {
        return -1;
    }
    else if (int1 == int2) {
        return 0;
    }
    else {
        return 1;
    }
}

// 
// PRIVATE FUNCTIONS
//

struct node *__retrieve_node_by_data(struct list *list, void *data, size_t size)
{
    struct node *cursor = list->head;
    if (list->compare != NULL) {
        while (cursor != NULL && list->compare(cursor->data, data, size) != 0) {
            cursor = cursor->next;
        }
    }
    else {
        while (cursor != NULL && memcmp(cursor->data, data, size) != 0) {
            cursor = cursor->next;
        }
    }
    return cursor;
}

struct node *__retrieve_node_at(struct list *list, int at)
{
    struct node *cursor;
    cursor = list->head;
    for (int i = 0; i < at; i++) {
        cursor = cursor->next;
    }
    return cursor;
}

void __remove_node(struct list *list, struct node *node)
{
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    else {
        list->head = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    else {
        list->tail = node->prev;
    }
    node_destroy(node);
    list->length--;
}

void __node_swap(struct node *node, struct node *node2)
{
    void *temp = node->data;
    node->data = node2->data;
    node2->data = temp;
}

struct node *__partition(struct node *start, struct node *end, int (*compare)(void *, void *, size_t), size_t size)
{
    struct node *pivot = start;
    struct node *cursor_i = start;
    struct node *cursor_j = start;

    while (cursor_j != NULL && cursor_j != end) {
        if (compare(cursor_j->data, end->data, size) < 0) {
            pivot = cursor_i;
            __node_swap(cursor_i, cursor_j);
            cursor_i = cursor_i->next;
        }
        cursor_j = cursor_j->next;
    }
    __node_swap(cursor_i, end);
    return pivot;
}

void __quicksort_recursive(struct node *start, struct node *end, int (*compare)(void *, void *, size_t), size_t size)
{
    if (start == end) {
        return;
    }
    struct node *pivot = __partition(start, end, compare, size);
    if (pivot != NULL && start != pivot) {
        __quicksort_recursive(start, pivot, compare, size);
    }
    if (pivot != NULL && pivot->next != NULL) {
        __quicksort_recursive(pivot->next, end, compare, size);
    }
}




