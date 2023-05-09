#ifndef node_h
#define node_h

#include <stddef.h> // size_t

struct node
{
    struct node *prev;
    struct node *next;
    void *data;
    size_t size;
    
    void (*free)(void *data);
    void (*print)(void *data);

    // used exclusively (for now) by the Balancing Binary Search Tree (bbstree)
    int height;
};

void node_print(struct node *node, void (*print)(void *data));

// Constructor / Destructor
struct node *node_init(void *data, size_t size, void (*free_data)(void *data), void (*print)(void *data));
void node_destroy(struct node *node);

#endif /* node_h */