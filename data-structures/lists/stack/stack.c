#include "stack.h"

#include <stdio.h>

struct stack stack_init(void (*free)(void *data), void (*print)(void *data))
{
    struct stack stack;
    stack.top = NULL;
    stack.free = free;
    stack.print = print;
    stack.height = 0;
    return stack;
}

int stack_destroy(struct stack *stack)
{
    if (stack == NULL) {
        return 1;
    }
    // Unpile entire stack
    while (!stack_unpile(stack));

    return 0;
}

int stack_pile(struct stack *stack, void *data, size_t size, void (*free)(void *data), void (*print)(void *data))
{
    if (stack == NULL) {
        return 1;
    }

    struct node *node = node_init(data, size, free, print);

    node->next = stack->top;
    stack->top = node;
    stack->height++;
    return 0;
}

int stack_unpile(struct stack *stack)
{
    if (stack == NULL || stack->height == 0) {
        return 1;
    }

    struct node *top = stack->top->next;
    node_destroy(stack->top);
    stack->top = top;
    stack->height--;
    return 0;
}

void *stack_peek(struct stack *stack)
{
    if (stack == NULL || stack->top == NULL) {
        return NULL;
    }
    return stack->top->data;
}

//
// Public helper function
//

void stack_print_int(void *data)
{
    printf("%d ", *(int *)data);
}