#include "data-structures/data-structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void list_example();
void bstree_example();
void avltree_example();
void dict_example();
void queue_example();
void tuple_example();

int main(void)
{
    srand(time(NULL));

    // puts("List example");
    // list_example();

    // puts("\n\nQueue example");
    // queue_example();

    // puts("\n\nTuple example");
    // tuple_example();

    // puts("\n\nDictionary example");
    // dict_example();

    // puts("Binary tree example");
    // bstree_example();

    // puts("AVL Binary tree example");
    // avltree_example();

    return 0;
}

void print_string(void *s) {printf("%s\n", (char *)s);}
void print_int(void *i) {printf("%d\n", *(int *)i);}
void print_float(void *f) {printf("%f\n", *(float *)f);}
void print_double(void *d) {printf("%lf\n", *(double *)d);}

void tuple_example()
{
    char name[] = "Lucas";
    int age = 18;
    int weight = 110;
    float salary = 0;
    double credit_rate = 1.111234;
    struct tuple tuple = tuple_init(5, sizeof(name), name, print_string, sizeof(int), &age, print_int, sizeof(int), &weight, print_int, sizeof(float), &salary, print_float, sizeof(double), &credit_rate, print_double);

    printf("tuple printing:\n");
    tuple_print(&tuple);
    puts("");
    puts("");
    printf("Name: %s\nAge: %d years old\nWeigth: %d pounds\nSalary: US$%.2f\nCredit Rate: %lf\n", 
        (char *)tuple_at(&tuple, 0), 
        *(int *)tuple_at(&tuple, 1), 
        *(int *)tuple_at(&tuple, 2), 
        *(float *)tuple.data[3], 
        *(double *)tuple.data[4]);

    tuple_destroy(&tuple);
}

void dict_example()
{
    struct dict dict = dict_init(dict_compare_int, dict_free_simple_entry, dict_print_int);
    int key;
    int val;
    for (int i = 0; i < 10; i++) {
        key = i;
        val = i * 10;
        dict_insert(&dict, &key, sizeof(int), &val, sizeof(int));
    }
    key = 4;
    void *data = dict_search(&dict, (void *)&key, sizeof(int));
    if (data != NULL) {
        dict_print_int(data);
    }
    puts("");
    bstree_print(&dict.bstree, DEPTH_FIRST, NULL);
    dict_destroy(&dict);
}

void bstree_example()
{
    srand(time(NULL));
    struct bstree bstree = bstree_init(5, bstree_compare_int, NULL, bstree_print_int);
    int data;
    data = 18;
    bstree_insert(&bstree, &data, sizeof(int), NULL, NULL);
    for (int i = 0; i < 100; i++) {
        data = rand() % 100;
        bstree_insert(&bstree, &data, sizeof(int), NULL, NULL);
    }


    // puts("");
    // bstree_print(&bstree, DEPTH_FIRST, NULL);
    // puts("");

    // puts("");
    // bstree_print(&bstree, BREADTH_FIRST, bstree_print_int_unspaced);
    // puts("");

    puts("");
    bstree_print(&bstree, BREADTH_FIRST, bstree_print_int_unspaced);
    puts("");

    bstree_destroy(&bstree);
}

void list_example()
{
    struct list list = list_init(list_compare_int, NULL, list_print_int);
    for (int i = 0; i < 12; i++) {
        list_append(&list, &i, sizeof(int), NULL, NULL);
    }
    printf("nodes appended\n");

    list_print(&list);
    puts("");

    list_pop_head(&list);
    list_pop_tail(&list);

    printf("nodes popped\n");

    list_print(&list);
    puts("");

    int data = 5;
    list_remove_data(&list, &data, sizeof(int));
    data = 6;
    list_remove_data(&list, &data, sizeof(int));
    list_remove_from(&list, 3); // takes out 4, at position 3
    list_remove_from(&list, 5); // takes out 9, at position 5

    printf("nodes removed\n");

    list_print(&list);
    puts("");

    list_insert(&list, 4, &data, sizeof(int), NULL, NULL);
    data = 10;
    list_insert(&list, 2, &data, sizeof(int), NULL, NULL);
    data = -123;
    list_insert(&list, 7, &data, sizeof(int), NULL, NULL);

    printf("nodes inserted\n");

    list_print(&list);
    puts("");

    list_quicksort(&list, sizeof(int));

    printf("list sorted\n");
    list_print(&list);
    puts("");

    list_destroy(&list);
}

void queue_example()
{
    struct queue queue = queue_init(NULL, queue_print_int);

    int data;
    for (int i = 0; i < 10; i++) {
        data = i;
        queue_push(&queue, &data, sizeof(int), NULL, NULL);
    }

    printf("nodes pushed\n");
    queue_print(&queue);
    puts("");
    puts("");

    data = *(int *)queue_peek(&queue);
    printf("data peeked: %d\n\n", data);

    // pop nodes
    for (int i = 0; i < 4; i++) {
        queue_pop(&queue);
    }
    printf("nodes popped\n");

    queue_print(&queue);
    puts("");
    puts("");
    
    data = *(int *)queue_peek(&queue);
    printf("data peeked: %d\n", data);

    queue_destroy(&queue);
}

void avltree_example()
{
    struct bbstree bbstree = bbstree_init(5, bstree_compare_int, NULL, bstree_print_int);

    int data;
    data = 20;
    bbstree_insert(&bbstree, &data, sizeof(int));
    for (int i = 0; i < 100; i++) {
        data = rand() % 100;
        bbstree_insert(&bbstree, &data, sizeof(int));
    }

    puts("");
    // bstree_print(&bbstree.bstree, DEPTH_FIRST, NULL);
    bstree_print(&bbstree.bstree, BREADTH_FIRST, bstree_print_int_unspaced);
    puts("");
    bstree_print(&bbstree.bstree, VERTICAL_BREADTH_FIRST, bstree_print_int_unspaced);
    puts("");

    bbstree_destroy(&bbstree);
}







