#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include "../bstree/bstree.h"

struct avltree
{
    struct bstree bstree;
};

int avltree_insert(struct avltree *avltree, void *data, size_t size);

// Constructor and destructor
struct avltree avltree_init(int max_depth, int (*compare)(void *d1, void *d2, size_t size), void (*free_data)(void *data), void (*print)(void *data));
int avltree_destroy(struct avltree *avltree);

#endif /* _AVL_TREE_H */