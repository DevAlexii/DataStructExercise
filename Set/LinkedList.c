#include "LinkedList.h"

struct list* get_tail(struct list** head)
{
    struct list *current_node = *head;
    struct list *last_node = NULL;
    while (current_node)
    {
        last_node = current_node;
        current_node = current_node->next;
    }
    return last_node;
};

int append(struct list** head, struct list* item)
{
    struct list *tail = get_tail(head);
    if (!tail)
    {
        *head = item;
        (*head)->count = 1;
    }
    else
    {
        tail->next = item;
        (*head)->count++;
    }
    item->next = NULL;
    return 0;
};

int pop(struct list** head)
{
    struct list *current_head = *head;
    if (!current_head)
    {
        printf("\nList is Null");
        return -1;
    }
    const int current_count = current_head->count;
    *head = (*head)->next;
    if (*head)
    {
       (*head)->count = current_count - 1;
    }
    
    current_head->next = NULL;
    return 0;
}

int length(struct list* head)
{
    return head?head->count:0;
};

int remove_at(struct list** head,const int index)
{
    if(!*head)
    {
        printf("\nList is Null");
        return 0; //empty list
    }
    if (index == 0)
    {
       pop(head);
       return -1; //remove first index
    }

    int head_length = (*head)->count;

    if (index > head_length)
    {
        return -1;
    }
    

    int current_index = 0;
    struct list* current_head = *head;
    struct list* prev_head = current_head;

    while(current_index < index)
    {
        current_index++;
        prev_head = current_head;
        current_head = current_head->next;
    }
    prev_head->next = current_head->next;
    (*head)->count--;
    return 0;
};

int remove_item(struct list** head , struct list* item)
{
    if(!*head)
    {
        printf("\nList is Null");
        return 0; 
    }
    
    struct list* current_head  = *head;

    if (current_head == item)
    {
        pop(head);
        return -1;
    }
    
    struct list* prev_head = current_head;

    while (current_head != item)
    {
        prev_head = current_head;
        current_head = current_head->next;
        if (current_head == NULL)
        {
            printf("\nItem not found!");
            return 0;
        }
    }
    prev_head->next = current_head->next;
    (*head)->count--;
    return -1; // remove item
};

int reverse_list(struct list** head)
{
    if (!head || !(*head) || !(*head)->next)
    {
        printf("\nList is null or has 1 item so no need to be reversed");
        return 0;
    }
    
    struct list* current_head = (*head);
    struct list* prev_head = NULL;
    struct list* next_head = NULL;
    
    int count = (*head)->count;

    while (current_head != NULL) 
    {
        next_head = current_head->next;
        current_head->next = prev_head;
        prev_head = current_head;
        current_head = next_head;
    }
    (*head) = prev_head;
    (*head)->count = count;
    return -1;
};

struct int_list
{
    struct list self;
    int value;
};

struct int_list* new_int_list(const int value)
{
    struct int_list *item = malloc(sizeof(struct int_list));
    if (!item)
    {
        return NULL;
    }
    item->value = value;

     return item; 
};

void print(struct list** head)
    {
        struct list* my_list_copy = *head;
        while (my_list_copy)
        {
            print_int_value(my_list_copy);
            my_list_copy = my_list_copy->next;
        }
    }

void linked_list_test()
{
    struct list* my_list = NULL;
    struct int_list test_item;
    test_item.value = 1;

    append(&my_list,&test_item.self);
    append(&my_list,&new_int_list(100)->self);
    append(&my_list,&new_int_list(200)->self);
    append(&my_list,&new_int_list(300)->self);
    printf("Start List:");
    print(&my_list);
     
    printf("\nTry remove item:%d",test_item.value);
    remove_item(&my_list,&test_item.self);
    print(&my_list);

    printf("\nTry remove item at index:%d",2);
    remove_at(&my_list,2);
    print(&my_list);

    printf("\nTry to reverse list");
    reverse_list(&my_list);
    print(&my_list);
}

// int main(int argc, char** argv)
// {
//     linked_list_test();
//     return 0;
// }