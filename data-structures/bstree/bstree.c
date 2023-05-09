#include "bstree.h"
#include "../utils/set.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *__bstree_search_recursive(struct bstree *bstree, struct node *cursor, void *data, size_t size, int *direction, int *layer);
void __destroy_recursive(struct node *node);
void __depth_print_recursive(struct node *cursor, void (*print)(void *data));
void __breadth_print_recursive(struct node **cursors, int depth, int layer, void (*print)(void *data));
void __vertical_breadth_print_recursive(struct node *leaf, int depth, int layer, void (*print)(void *data));

struct bstree bstree_init(int max_depth, int (*compare)(void *d1, void *d2, size_t size), void (*free_data)(void *data), void (*print)(void *data))
{
    struct bstree bstree;
    bstree.root = NULL;
    bstree.depth = 0;
    bstree.max_depth = max_depth;
    bstree.compare = compare;
    bstree.print = print;
    bstree.free = free_data;
    return bstree;
}

int bstree_destroy(struct bstree *bstree)
{
    if (bstree == NULL) {
        return 1;
    }
    __destroy_recursive(bstree->root);
    return 0;
}

int bstree_insert(struct bstree *bstree, void *data, size_t size, void (*free_data)(void *data), void (*print)(void *data))
{
    if (bstree == NULL || size < 1) {
        return 1;
    }
    // Set free function
    void (*free_function) = set_function(free_data, bstree->free);
    // Set print function
    void (*print_function) = set_function(print, bstree->print);
    struct node *node = node_init(data, size, free_function, print_function);
    if (bstree->root == NULL) {
        bstree->root = node;
        bstree->depth++;
        return 0;
    }
    int direction, layer = 0;
    struct node *cursor = __bstree_search_recursive(bstree, bstree->root, data, size, &direction, &layer);

    // 2 for layer exceeding bstree's maximum depth
    if (bstree->max_depth > 0 && layer > bstree->max_depth) {
        return 2;
    }
    // 3 for data already in bstree
    if (direction == 0) {
        return 3;
    }
    // Increase bstree's depth if layer is greater than it
    if (layer > bstree->depth) {
        bstree->depth++;
    }
    // Insert node accordingly
    if (direction == -1) {
        cursor->prev = node;
    }
    else if (direction == 1) {
        cursor->next = node;
    }
    return 0;
}

void *bstree_search(struct bstree *bstree, void *data, size_t size)
{
    int direction, dummy_layer;
    struct node *cursor = __bstree_search_recursive(bstree, bstree->root, data, size, &direction, &dummy_layer);
    if (direction == 0) {
        return cursor->data;
    }
    return NULL;
}

int bstree_print(struct bstree *bstree, enum print_mode mode, void (*print)(void *data))
{
    if (bstree == NULL || bstree->root == NULL) {
        return 1;
    }

    void (*print_function) = set_function(print, bstree->print);

    if (mode == BREADTH_FIRST) {
        __breadth_print_recursive(&bstree->root, bstree->depth, 0, print_function);
    }
    else if (mode == DEPTH_FIRST) {
        __depth_print_recursive(bstree->root, print_function);
    }
    else if (mode == VERTICAL_BREADTH_FIRST) {
        __vertical_breadth_print_recursive(bstree->root, bstree->depth, 0, print_function);
    }

    return 0;
}

// PUBLIC HELPER FUNCTIONS
int bstree_compare_int(void *d1, void *d2, size_t _)
{
    int i1 = *(int *)d1;
    int i2 = *(int *)d2;
    return i1 - i2;
}

void bstree_print_int(void *data)
{
    printf("%d ", *(int *)data);
}

void bstree_print_int_unspaced(void *data)
{
    printf("%2d", *(int *)data);
}

// PRIVATE FUNCTIONS
struct node *__bstree_search_recursive(struct bstree *bstree, struct node *cursor, void *data, size_t size, int *direction, int *layer)
{
    // Compare searched data and this iteration's cursor data
    int result = bstree->compare(data, cursor->data, size);
    // Data is less than cursor data
    if (result < 0) {
        // If cursor has a child with a smaller value (is pointed to by `prev`)...
        if (cursor->prev != NULL) {
            // Make it the cursor of the next iteration and return the result
            *layer += 1;
            return __bstree_search_recursive(bstree, cursor->prev, data, size, direction, layer);
        }
        else {
            // Return cursor and set direction to -1 (where node with the searched data should be inserted in, in this case, `prev`)
            *direction = -1;
            *layer += 1;
            return cursor;
        }
    }
    // Data is greater than cursor data
    if (result > 0) {
        // If cursor has a child with a greater value...
        if (cursor->next != NULL) {
            // Make it the cursor of the next iteration
            *layer += 1;
            return __bstree_search_recursive(bstree, cursor->next, data, size, direction, layer);
        }
        else {
            // Return cursor and set direction to 1 (where a new node with the searched data should be inserted in, in this case, `next`)
            *direction = 1;
            *layer += 1;
            return cursor;
        }
    }
    // Searched data is equal to cursor data, meaning the data is already in the tree; a new node will not be inserted
    *direction = 0;
    *layer = 0;
    return cursor;
}

void __destroy_recursive(struct node *node)
{
    if (node == NULL) {
        return;
    }
    __destroy_recursive(node->prev);
    __destroy_recursive(node->next);
    node_destroy(node);    
}

void __breadth_print_recursive(struct node **nodes, int depth, int layer, void (*print)(void *data))
{
    if (depth > 5) {
        printf("Can only print up to 6 layers of depth\n");
        return;
    }
    int n_nodes = pow(2, layer);
    int n_children = 2 * n_nodes; // naturally, each node has 2 children (even if they're null)
    struct node **children = (struct node **)malloc(sizeof(struct node *) * n_children); // love how verbose this is (no sarcasm)
    int n = 0, there_are_children = 0;
    
    // print leading spaces conform current layer
    for (int i = 0; i < pow(2, depth - layer); i++) {
        printf("  ");
    }

    // gonna curse through the nodes, print their values, and add their kids to the children vector
    struct node *cursor;
    for (int i = 0; i < n_nodes; i++) {
        cursor = nodes[i];
        if (cursor != NULL) {
            node_print(cursor, print);
            children[n++] = cursor->prev;
            children[n++] = cursor->next;
            there_are_children = 1;
        }
        else {
            // if there's no node, next two seats are naturally vacant, too
            printf("  ");
            for (int m = n + 2; n < m; children[n++] = NULL);
        }
        // print spaces between this layer's nodes' values, yes?
        for (int i = 0; i < pow(2, depth - layer + 1) - 1; i++) {
            printf("  ");
        }
    }
    // if there ain't no children, return right back
    if (!there_are_children) {
        return;
    }
    // a new layer comes
    printf("\n");
    __breadth_print_recursive(children, depth, layer + 1, print);
    // thank you for your time, kiddos
    free(children);
}

void __depth_print_recursive(struct node *cursor, void (*print)(void *data))
{
    if (cursor == NULL) {
        return;
    }
    __depth_print_recursive(cursor->prev, print);
    node_print(cursor, print);
    __depth_print_recursive(cursor->next, print);
}

int __ln_recursive(int x) {
    if (x < 0) {
        return 0;
    }
    if (x == 0) {
        return 1;
    }
    return 2 * __ln_recursive(x - 1) + 1;
}

void __vertical_breadth_print_recursive(struct node *leaf, int depth, int layer, void (*print)(void *data))
{
    if (leaf == NULL) {
        for (int i = 0; i < __ln_recursive(depth - layer); i++) {
            printf("\n");
        }
        return;
    }

    __vertical_breadth_print_recursive(leaf->prev, depth, layer + 1, print);

    for (int i = 0; i < layer; i++) {
        printf("    ");
    }
    node_print(leaf, print);
    printf("\n");

    __vertical_breadth_print_recursive(leaf->next, depth, layer + 1, print);
}







