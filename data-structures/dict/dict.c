#include "dict.h"
#include "../utils/set.h"

#include <stdio.h>
// #include <stdlib.h>

struct dict dict_init(int (*compare)(void *key, void *entry, size_t key_size), void (*free_data)(void *data), void (*print)(void *data))
{
    struct dict dict;
    dict.bstree = bstree_init(0, compare, free_data, print);
    dict.n_entries = 0;
    return dict;
}

int dict_destroy(struct dict *dict)
{
    if (dict == NULL) {
        return 1;
    }
    bstree_destroy(&dict->bstree);
    return 0;
}

int dict_insert(struct dict *dict, void *key, size_t key_size, void *val, size_t val_size)
{
    if (dict == NULL || key_size == 0 || val_size == 0) {
        return 1;
    }
    struct entry entry = entry_init(key, key_size, val, val_size);
    bstree_insert(&dict->bstree, &entry, sizeof(entry), NULL, NULL);
    dict->n_entries++;
    return 0;
}

void *dict_search(struct dict *dict, void *key, size_t key_size)
{
    void *val = bstree_search(&dict->bstree, key, key_size);
    return val;
}

// PUBLIC HELPER FUNCTIONS
int dict_compare_int(void *key, void *entry, size_t _)
{
    int key1 = *(int *)key;
    int key2 = *(int *)((struct entry *)entry)->key;

    if (key1 < key2) {
        return -1;
    }
    else if (key1 == key2) {
        return 0;
    }
    else {
        return 1;
    }
}

void dict_print_int(void *data)
{
    struct entry *entry = (struct entry *)data;
    int key = *(int *)entry->key;
    int val = *(int *)entry->val;
    printf("%3d : %3d\n", key, val);
}

void dict_free_simple_entry(void *entry)
{
    entry_destroy((struct entry *)entry);
}









