#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define macro_random_in_range(min,max)(rand()%(max - min + 1) + min)

struct doubly_list{

    struct doubly_list *prev;
    struct doubly_list *next;
    unsigned int count;
};

int length(struct doubly_list* head) {
    return head ? head->count : 0;
}

struct doubly_list* get_tail(struct doubly_list** head){

    struct doubly_list *current_node = *head;
    struct doubly_list *last_node = NULL;
    while (current_node)
    {
        last_node = current_node;
        current_node = current_node->next;
    }
    return last_node;
};

int alreay_contained(struct doubly_list** head, struct doubly_list* item){
    int current_index = 0;
    struct doubly_list* current_head = *head;
    while (current_index < length(*head))
    {
        current_index++;
        current_head = current_head->next;
        if (current_head)
        {
            if (current_head == item)
            {
                return -1;
            }
        }
    }
    return 0;
}

int append (struct doubly_list** head, struct doubly_list* item){

    struct doubly_list* tail = get_tail (head);
    if (!tail)
    {
        *head = item;
        (*head)->count = 1;
    }
    else
    {
        if (alreay_contained(head,item)<0)
        {   
            printf("\nItem already contained");
            return -1;
        }
    
        tail->next = item;
        (*head)->count++;
    }
    item->prev = tail;
    item->next = NULL;

    return 0;
}

int pop(struct doubly_list** head){
    
    if (!head)
    {
        printf("\nNull list");
        return 0; //empty list
    }
    struct doubly_list *current_head = *head;
    const int current_count = current_head->count;
    *head = (*head)->next;
   
    if (*head)
    {
       (*head)->count = current_count - 1;
       (*head)->prev = NULL;
    }

    current_head->next = NULL;
    return -1;
};

int remove_at(struct doubly_list** head, const int index){
    if (!head) {
        printf("\nNull list");
        return 0; //empty list
    }
    
    if (index == 0){
        pop(head);
        return -1; //Remove first index;
    }

    if(index >= (*head)->count){
         printf("\nTry Remove out of range");
        return 0; //out of index
    }
   
    struct doubly_list* current_head = *head;
    
    for (int i = 0; i < index; i++)
    {
        current_head = current_head->next;
    }
    current_head->prev->next = current_head->next;
    if (current_head->next)
    {
        current_head->next->prev = current_head->prev;
    }
    current_head = NULL;
    
    (*head)->count--;
    return -1;
}

int append_after_item(struct doubly_list** head, struct doubly_list* given_item, struct doubly_list* item_to_add) {
    
    if (!*head) {
        printf("\nNull list:");
        return -1; 
    }

    if (alreay_contained(head,item_to_add)<0)
    {   
        printf("\nItem already contained");
        return -1;
    }

    struct doubly_list* current_head = *head;

    if (!given_item) {
        printf("\nNo Given Item:");
        return -1; 
    }  
    else {
        while (current_head != given_item) {
        current_head = current_head->next;
        if (!current_head) {
            printf("\nGiven item not found!");
            return -1; 
        }
        }

        struct doubly_list* saved_next_head = current_head->next;
        current_head->next = item_to_add;
        item_to_add->prev = current_head;
        item_to_add->next = saved_next_head;

        if (saved_next_head) {
            saved_next_head->prev = item_to_add;
        }
    }
    (*head)->count++;
    return -1;
}
   
int append_before_item(struct doubly_list** head, struct doubly_list* given_item, struct doubly_list* item_to_add) {
    
    if (!*head) {
        printf("\nNull list:");
        return -1; // empty list
    }

    if (alreay_contained(head,item_to_add)<0)
    {   
        printf("\nItem already contained");
        return -1;
    }

    struct doubly_list* current_head = *head;
    int current_index = 0;

    while (current_head != given_item) {
        current_index++;
        current_head = current_head->next;
        if (!current_head) {
            printf("\nGiven item not found!");
            return -1; // given item not found
        }
    }

    item_to_add->next = current_head;
    item_to_add->prev = current_head->prev;
    struct doubly_list* prev_current = current_head->prev;
    if (prev_current)
    {
        prev_current->next = item_to_add;
    }
    
    current_head->prev = item_to_add;
    
    if(current_index==0){
        int count = (*head)->count;
        (*head) =  item_to_add;
        (*head)->count = count;
    }

    (*head)->count++;

    return 0;
}

struct int_doubly_list{
    struct doubly_list self;
    int value;
};

void swap_values(struct doubly_list* a, struct doubly_list* b) {
    int temp = ((struct int_doubly_list*)a)->value;
    ((struct int_doubly_list*)a)->value = ((struct int_doubly_list*)b)->value;
    ((struct int_doubly_list*)b)->value = temp;
}

int shuffle(struct doubly_list** head) {
    if (*head == NULL || (*head)->next == NULL) {
        printf("List has zero or one item, no need to shuffle\n");
        return -1;
    }

    srand(time(0));
    int count = (*head)->count;
    struct doubly_list* current = *head;

    for (int i = count - 1; i > 0; i--) 
    {
        int j = macro_random_in_range(0, i);

        struct doubly_list* node_i = current;
        struct doubly_list* node_j = current;

        //move node_i to last index - i
        for (int k = 0; k < i; k++)
        {
            node_i = node_i->next;
        }
        //move node_j to randomize index
        for (int k = 0; k < j; k++) 
        {
            node_j = node_j->next;
        }

        swap_values(node_i, node_j);
    }

    return 0;
}

int print_list_value(struct doubly_list* head){
    struct doubly_list* my_list_copy = head;
    int count = length(head);
    for (int i = 0; i < count; i++) {
        printf("\nValue: %d", ((struct int_doubly_list*)my_list_copy)->value);
        my_list_copy = my_list_copy->next;
    }
    return 0;
}

int main(int argc, char** argv){

    struct doubly_list* my_list = NULL;

    struct int_doubly_list item;
    item.value = 100;

    struct int_doubly_list item2;
    item2.value = 200;

    struct int_doubly_list item3;
    item3.value = 300;

    struct int_doubly_list item4;
    item4.value = 400;

    //append Items
    append(&my_list,&item.self);
    append(&my_list,&item2.self);
    append(&my_list,&item3.self);

    printf("StartList:");
    print_list_value(my_list);

    //Remove At index
    int index_to_remove = 1;
    printf("\nTry Remove index:%d",index_to_remove);
    remove_at(&my_list,index_to_remove);
    print_list_value(my_list);

    //append after Item
    printf("\nTry add item :%d",item4.value);
    printf(" after given item :%d",item.value);
    append_after_item(&my_list,&item.self,&item4.self);
    print_list_value(my_list);

    //append after Item
    printf("\nTry add item :%d",item4.value);
    printf(" after given item :%d",item.value);
    append_after_item(&my_list,&item.self,&item4.self);
    print_list_value(my_list);

    //append before Item
    printf("\nTry add item :%d",item2.value);
    printf(" before given item :%d",item.value);
    append_before_item(&my_list,&item.self,&item2.self);
    print_list_value(my_list);

    //Shuffle
    printf("\nTry shuffle:");
    shuffle(&my_list);
    print_list_value(my_list);

    return 0;
}