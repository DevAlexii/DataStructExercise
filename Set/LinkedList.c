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