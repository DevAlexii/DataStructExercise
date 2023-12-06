#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h> 

#define get_int_value(x)(((struct int_list*)x)->value)
#define print_int_value(x)(printf("\nValue:%d", get_int_value(x)))

struct list
{
    struct list *next;
    unsigned int count;
};

struct list* get_tail(struct list** head);

int append(struct list** head, struct list* item);

int pop(struct list** head);

int length(struct list* head);

int remove_at(struct list** head,const int index);

int remove_item(struct list** head , struct list* item);

int reverse_list(struct list** head);