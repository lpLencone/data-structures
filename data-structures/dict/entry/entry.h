#ifndef entry_h
#define entry_h

#include <stddef.h>

struct entry
{
    void *key;
    void *val;
};

struct entry entry_init(void *key, size_t key_size, void *val, size_t val_size);
void entry_destroy(struct entry *entry);

#endif /* entry_h */