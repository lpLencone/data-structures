#ifndef bstree_h
#define bstree_h

#include "../node/node.h"

enum print_mode
{
    BREADTH_FIRST, DEPTH_FIRST, VERTICAL_BREADTH_FIRST
};

struct bstree
{
    struct node *root;
    int depth;
    int max_depth;
    int (*compare)(void *d1, void *d2, size_t size);
    void (*free)(void *data);
    void (*print)(void *data);
};

int bstree_insert(struct bstree *bstree, void *data, size_t size, void (*free_data)(void *data), void (*print)(void *data));
void *bstree_search(struct bstree *tree, void *data, size_t size);
/// @param mode one of the two modes available (BREADTH_FIRST, DEPTH_FIRST); BREADTH_FIRST can only print tree up to 5 layers of depth (counting from 0 to 5, inclusive)
/// @param print (optional) if not specified, use print function defined in the bstree's initialization instead
int bstree_print(struct bstree *bstree, enum print_mode mode, void (*print)(void *data));

/**
 * PUBLIC HELPER FUNCTIONS
*/

int bstree_compare_int(void *data1, void *data2, size_t _);
void bstree_print_int(void *data);
void bstree_print_int_unspaced(void *data);


// Constructor / Destructor
struct bstree bstree_init(int max_depth, int (*compare)(void *d1, void *d2, size_t size), void (*free_data)(void *data), void (*print)(void *data));
int bstree_destroy(struct bstree *bstree);

#endif /* bstree_h */