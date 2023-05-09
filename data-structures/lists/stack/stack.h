#ifndef stack_h
#define stack_h

#include "../../node/node.h"

#include <stddef.h> // size_t

struct stack
{
    struct node *top;
    void (*free)(void *data);
    void (*print)(void *data);
    int height;
};

int stack_pile(struct stack *stack, void *data, size_t size, void (*free_data)(void *data), void (*print)(void *data));
int stack_unpile(struct stack *stack);
void *stack_peek(struct stack *stack);

// Public helper functions
void stack_print_int(void *data);

// Constructor
struct stack stack_init(void (*free)(void *data), void (*print)(void *data));
// Destructor
int stack_destroy(struct stack *stack);

#endif /* stack_h */