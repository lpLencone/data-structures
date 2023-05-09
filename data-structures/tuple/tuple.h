#ifndef tuple_h
# define tuple_h

struct tuple
{
    int n_items;
    void **data;
    void (**print)(void *data);
};

struct tuple tuple_init(int n_items, ...);
void tuple_destroy(struct tuple *tuple);
void * tuple_at(struct tuple *tuple, unsigned int i);
void tuple_print(struct tuple *tuple);


#endif /* tuple_h */