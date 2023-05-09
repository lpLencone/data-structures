#include "avltree.h"
#include "../utils/set.h"

#include <stdio.h>

int __max(int a, int b);
int __height(struct node *node);
int __getbalance(struct node *node);
struct node *__rotater(struct node *leaf);
struct node *__rotatel(struct node *leaf);
struct node *__bbstree_insert_recursive(struct bstree *bstree, struct node *leaf, struct node *node, size_t size, int (*compare)(void *data1, void *data2, size_t size), int layer);

struct bbstree bbstree_init(int max_depth, int (*compare)(void *d1, void *d2, size_t size), void (*free_data)(void *data), void (*print)(void *data))
{
    struct bbstree bbstree;
    bbstree.bstree = bstree_init(max_depth, compare, free_data, print);
    return bbstree;
}

int bbstree_destroy(struct bbstree *bbstree)
{
    return bstree_destroy(&bbstree->bstree);
}

int bbstree_insert(struct bbstree *bbstree, void *data, size_t size)
{
    struct bstree *bstree = (bbstree == NULL) ? NULL : &bbstree->bstree;
    if (bstree == NULL || size < 1) {
        return 1;
    }

    // Create node
    struct node *node = node_init(data, size, bstree->free, bstree->print);
    node->height = 1;

    // Insert node and rotate tree as many times as necessary to have it balanced (height-wise)
    bstree->root = __bbstree_insert_recursive(bstree, bbstree->bstree.root, node, size, bstree->compare, 1);

    return 0;
}

//
// PUBLIC HELPER FUNCTIONS
//

// void bbstree_print_height(void *nodo)
// {
//     struct node *node = (struct node *)nodo;
//     int height = node->height;
//     printf("%d\n", height);
// }

// void bbstree_print_height_unspaced(void *nodo)
// {
//     struct node *node = (struct node *)nodo;
//     printf("%2d", node->height);
// }

// void bbstree_print_int_unspaced(void *nodo)
// {
//     struct node *node = (struct node *)nodo;
//     printf("%2d", *(int *)node->data);
// }

//
// PRIVATE FUNCTIONS
//

struct node *__bbstree_insert_recursive(struct bstree *bstree, struct node *leaf, struct node *node, size_t size, int (*compare)(void *data1, void *data2, size_t size), int layer)
{
    if (bstree->max_depth > 0 && layer > bstree->max_depth) {
        node_destroy(node);
        return NULL;
    }
    
    if (layer > bstree->depth) {
        bstree->depth++;
    }

    if (leaf == NULL) {
        return node;
    }

    int comparison = compare(node->data, leaf->data, size);

    // If new node's data is less than the leafs's data
    if (comparison < 0) {
        leaf->prev = __bbstree_insert_recursive(bstree, leaf->prev, node, size, compare, layer + 1);
    }
    // If leaf's data is less than the new node's data
    else if (comparison > 0) {
        leaf->next = __bbstree_insert_recursive(bstree, leaf->next, node, size, compare, layer + 1);
    }
    // If equal
    else {
        node_destroy(node);
        return leaf;
    }

    leaf->height = 1 + __max(__height(leaf->prev), __height(leaf->next));

    int balance = __getbalance(leaf);

    // If left size is heavier
    if (balance > 1) {
        int left_comparison = compare(leaf->prev->data, node->data, size);

        // If leaf's left child's data is greater than the new node's data
        if (left_comparison > 0) {
            return __rotater(leaf);
        }

        // If leaf's left child's data is less than the new node's data
        if (left_comparison < 0) {
            leaf->prev = __rotatel(leaf->prev);
            return __rotater(leaf);
        }
    }

    // If right size is heavier
    else if (balance < -1) {
        int right_comparison = compare(leaf->next->data, node->data, size);

        // If leaf's right child's data is less than the new node's data
        if (right_comparison < 0) {
            return __rotatel(leaf);
        }

        // If leaf's right child's data is greater than the new node's data
        if (right_comparison > 0) {
            leaf->next = __rotater(leaf->next);
            return __rotatel(leaf);
        }
    }

    return leaf;
}

struct node *__rotater(struct node *leaf) 
{
    struct node *x = leaf->prev;
    struct node *T2 = x->next;

    x->next = leaf;
    leaf->prev = T2;

    leaf->height = __max(__height(leaf->prev), __height(leaf->next)) + 1;
    x->height = __max(__height(x->prev), __height(x->next)) + 1;

    return x;
}

struct node *__rotatel(struct node *leaf)
{
    struct node *y = leaf->next;
    struct node *T2 = y->prev;

    y->prev = leaf;
    leaf->next = T2;

    leaf->height = __max(__height(leaf->prev), __height(leaf->next)) + 1;
    y->height = __max(__height(y->prev), __height(y->next)) + 1;

    return y;
}

int __max(int a, int b)
{
    return (a > b) ? a : b;
}

int __height(struct node *node)
{
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int __getbalance(struct node *node)
{
    if (node == NULL) {
        return 0;
    }
    return __height(node->prev) - __height(node->next);
}




