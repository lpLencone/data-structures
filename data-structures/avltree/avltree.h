#ifndef bbstree_h
#define bbstree_h

#include "../bstree/bstree.h"

// Balancing Binary Search Tree
struct bbstree
{
    struct bstree bstree;
};

int bbstree_insert(struct bbstree *bbstree, void *data, size_t size);

// Constructor and destructor
struct bbstree bbstree_init(int max_depth, int (*compare)(void *d1, void *d2, size_t size), void (*free_data)(void *data), void (*print)(void *data));
int bbstree_destroy(struct bbstree *bbstree);

#endif /* bbstree_h */