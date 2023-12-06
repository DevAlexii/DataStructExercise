#include "LinkedList.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define hashmap_size 64

struct set_item {
    struct list next;
    const char* key;
    size_t key_len;
};

struct set {
    struct list* hashmap[hashmap_size];
};

size_t djb33x_hash(const char* key, const size_t key_len) {
    size_t hash = 5381;
    for (size_t i = 0; i < key_len; i++) {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

void set_insert(struct set* set, struct set_item* item) {
    const size_t key_len = strlen(item->key);
    const size_t hash = djb33x_hash(item->key, key_len);
    const size_t index = hash % hashmap_size;

    printf("-------------------------\n");
    printf("Try to add %s at index: %llu\n", item->key, index);

    if (set->hashmap[index] == NULL)
    {   
        printf("List empty\n");
        set->hashmap[index] = (struct list*)item;
        printf("Make new item at index: %llu (slot: 0)\n", index);
        return;
    }
    
    struct list* current_item_list = set->hashmap[index];
    printf("Collision at index: %llu\n", index);
    get_tail(&current_item_list)->next = &(item->next);
    current_item_list->count++;
    printf("add new item at index: %llu (slot: %d)\n", index,length(current_item_list));
}

void print_set(const struct set* set) {
    printf("-------------------------\n");
    printf("Set contents:\n");

    for (size_t i = 0; i < hashmap_size; ++i) {
        struct list* current_node = set->hashmap[i];
        int slot = 0;
        while (current_node != NULL) {
            struct set_item* current_item = (struct set_item*)current_node;
            printf("Index %zu: %s (slot: %d)\n", i, current_item->key,slot);
            slot++;
            current_node = current_node->next;
        }
    }
}

int search(struct set* set, const char* key){
    const size_t key_len = strlen(key);
    const size_t hash = djb33x_hash(key, key_len);
    const size_t index = hash % hashmap_size;

    printf("-------------------------\n");
    printf("Try to find %s at index: %llu\n", key, index);
    if (set->hashmap[index] == NULL)
    { 
        printf("There's nothing at index %llu\n",index);
        return 0;
    }
    else
    {
        struct list* current_node = set->hashmap[index];
        int slot = 0;
        while (current_node != NULL) {
            struct set_item* current_item = (struct set_item*)current_node;
            if (current_item->key == key)
            {
                printf("Found %s at index %llu (slot: %d)\n", key,index,slot);
                return 1;
            }
            slot++;
            current_node = current_node->next;
        }
        
        printf("%s not found at index: %llu\n", key, index);
        return 0;
    }
}

void set_remove(struct set* set,const char* key){
    const size_t key_len = strlen(key);
    const size_t hash = djb33x_hash(key, key_len);
    const size_t index = hash % hashmap_size;

    printf("-------------------------\n");
    printf("Try to remove %s at index: %llu\n", key, index);
    if (set->hashmap[index] == NULL)
    { 
        printf("There's nothing at index %llu\n",index);
        printf("Cannot remove %s at index %llu\n",key,index);
    }
    else
    {
        struct list* current_node = set->hashmap[index];
        struct set_item* prev_item = NULL;

        int slot = 0;
        while (current_node != NULL) {

            struct set_item* current_item = (struct set_item*)current_node;

            if (current_item->key == key)
            {
                if (slot == 0)
                {
                    pop(&(set->hashmap[index]));
                }
                else
                {
                    prev_item->next = current_item->next;
                    current_item = NULL;
                }
                printf("Successfully remove %s at index: %llu (slot: %d)\n",key,index,slot);
                return;
            }
            slot++;
            prev_item = current_item;
            current_node = current_node->next;
        }
    }
}

void set_insert_unique_key(struct set* set, struct set_item* item){
    const size_t key_len = strlen(item->key);
    const size_t hash = djb33x_hash(item->key, key_len);
    const size_t index = hash % hashmap_size;

    printf("-------------------------\n");
    printf("Try to add unique key %s at index: %llu\n", item->key, index);

    if (set->hashmap[index] == NULL)
    {   
        printf("List empty\n");
        set->hashmap[index] = (struct list*)item;
        printf("Make new item at index: %llu (slot: 0)\n", index);
        return;
    }
    
    struct list* current_node = set->hashmap[index];
    struct set_item* prev_item = NULL;

    int slot = 0;
    while (current_node != NULL) {

        struct set_item* current_item = (struct set_item*)current_node;

        if (current_item->key == item->key)
        {
            printf("There's already key %s at index: %llu (slot: %d)\n",item->key,index,slot);
            return;
        }
        slot++;
        prev_item = current_item;
        current_node = current_node->next;
    }

    struct list* current_item_list = set->hashmap[index];
    printf("Collision at index: %llu\n", index);
    get_tail(&current_item_list)->next = &(item->next);
    current_item_list->count++;
    printf("add new item at index: %llu (slot: %d)\n", index,length(current_item_list));

}

int main(int argc, char** argv) {
    struct set my_set;
    memset(&my_set, 0, sizeof(struct set));

    struct set_item item1 = {0};
    item1.key = "Hello";

    set_insert(&my_set, &item1);

    struct set_item item2 = {0};
    item2.key = "World";

    set_insert(&my_set, &item2);

    print_set(&my_set);

    search(&my_set,"Pluto");
    search(&my_set,"World");

    set_remove(&my_set,"Pippo");
    set_remove(&my_set,"Hello");

    search(&my_set,"Hello");
    search(&my_set,"World");

    set_remove(&my_set,"World");

    print_set(&my_set);

    set_insert(&my_set,&item2);

    print_set(&my_set);

    set_insert_unique_key(&my_set,&item2);
    set_insert_unique_key(&my_set,&item1);

    print_set(&my_set);

    return 0;
}