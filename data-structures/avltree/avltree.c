#include "avltree.h"
#include "../utils/set.h"

#include <stdio.h>

int __max(int a, int b);
int __height(struct node *node);
int __getbalance(struct node *node);
struct node *__rotater(struct node *treenode);
struct node *__rotatel(struct node *treenode);
struct node *__avltree_insert_recursive(struct bstree *bstree, struct node *treenode, struct node *node, size_t size, int (*compare)(void *data1, void *data2, size_t size), int layer);

struct avltree avltree_init(int max_depth, int (*compare)(void *d1, void *d2, size_t size), void (*free_data)(void *data), void (*print)(void *data))
{
    struct avltree avltree;
    avltree.bstree = bstree_init(max_depth, compare, free_data, print);
    return avltree;
}

int avltree_destroy(struct avltree *avltree)
{
    return bstree_destroy(&avltree->bstree);
}

int avltree_insert(struct avltree *avltree, void *data, size_t size)
{
    struct bstree *bstree = (avltree == NULL) ? NULL : &avltree->bstree;
    if (bstree == NULL || size < 1) {
        return 1;
    }

    // Create node
    struct node *node = node_init(data, size, bstree->free, bstree->print);
    node->height = 1;

    // Insert node and rotate tree as many times as necessary to have it balanced (height-wise)
    bstree->root = __avltree_insert_recursive(bstree, avltree->bstree.root, node, size, bstree->compare, 1);

    return 0;
}

void *avltree_search(struct avltree *avltree, void *data, size_t size)
{
    return bstree_search(&avltree->bstree, data, size);
}

//
// PRIVATE FUNCTIONS
//

struct node *__avltree_insert_recursive(struct bstree *bstree, struct node *treenode, struct node *newnode, size_t size, int (*compare)(void *data1, void *data2, size_t size), int layer)
{
    if (bstree->max_depth > 0 && layer > bstree->max_depth) {
        node_destroy(newnode);
        return NULL;
    }
    
    if (layer > bstree->depth) {
        bstree->depth++;
    }

    if (treenode == NULL) {
        return newnode;
    }

    int comparison = compare(newnode->data, treenode->data, size);

    // If new newnode's data is less than the treenode's data
    if (comparison < 0) {
        treenode->prev = __avltree_insert_recursive(bstree, treenode->prev, newnode, size, compare, layer + 1);
    }
    // If treenode's data is less than the new node's data
    else if (comparison > 0) {
        treenode->next = __avltree_insert_recursive(bstree, treenode->next, newnode, size, compare, layer + 1);
    }
    // If equal
    else {
        node_destroy(newnode);
        return treenode;
    }

    treenode->height = 1 + __max(__height(treenode->prev), __height(treenode->next));

    int balance = __getbalance(treenode);

    // If left size is heavier
    if (balance > 1) {
        int left_comparison = compare(treenode->prev->data, newnode->data, size);

        // If treenode's left child's data is greater than the new newnode's data
        if (left_comparison > 0) {
            return __rotater(treenode);
        }

        // If treenode's left child's data is less than the new newnode's data
        if (left_comparison < 0) {
            treenode->prev = __rotatel(treenode->prev);
            return __rotater(treenode);
        }
    }

    // If right size is heavier
    else if (balance < -1) {
        int right_comparison = compare(treenode->next->data, newnode->data, size);

        // If treenode's right child's data is less than the new newnode's data
        if (right_comparison < 0) {
            return __rotatel(treenode);
        }

        // If treenode's right child's data is greater than the new newnode's data
        if (right_comparison > 0) {
            treenode->next = __rotater(treenode->next);
            return __rotatel(treenode);
        }
    }

    return treenode;
}

struct node *__rotater(struct node *treenode) 
{
    struct node *x = treenode->prev;
    struct node *T2 = x->next;

    x->next = treenode;
    treenode->prev = T2;

    treenode->height = __max(__height(treenode->prev), __height(treenode->next)) + 1;
    x->height = __max(__height(x->prev), __height(x->next)) + 1;

    return x;
}

struct node *__rotatel(struct node *treenode)
{
    struct node *y = treenode->next;
    struct node *T2 = y->prev;

    y->prev = treenode;
    treenode->next = T2;

    treenode->height = __max(__height(treenode->prev), __height(treenode->next)) + 1;
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




