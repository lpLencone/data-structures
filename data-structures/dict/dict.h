#ifndef dict_h
#define dict_h

#include "../bstree/bstree.h"
#include "entry/entry.h"

struct dict
{
    struct bstree bstree;
    int n_entries;
};

int dict_insert(struct dict *dict, void *key, size_t key_size, void *val, size_t val_size);
void *dict_search(struct dict *dict, void *key, size_t key_size);

// PUBLIC HELPER FUNCTIONS

int dict_compare_int(void *entry1, void *entry2, size_t _unused);
void dict_print_int(void *data);
void dict_free_simple_entry(void *entry);

struct dict dict_init(int (*compare)(void *key, void *entry, size_t key_size), void (*free_data)(void *data), void (*print)(void *data));
int dict_destroy(struct dict *dict);

#endif /* dict_h */