#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t hash_function(const char* key) {
    size_t hash = 5381;
    size_t key_len = strlen(key);
    
    for (size_t i = 0; i < key_len; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

struct data {
    const char* key;
    size_t key_len;
    size_t item_size;
    void* value;
};

struct set {
    struct data data;
    struct set* next;
};

struct dictionary {
    struct set** sets;
    size_t size;
};

struct dictionary* dictionary_init(size_t size) {
    struct dictionary* dict = malloc(sizeof(struct dictionary));
    if (!dict) {
        printf("Dictionary Malloc Error\n");
        exit(EXIT_FAILURE);
    }

    dict->sets = calloc(size, sizeof(struct set*));
    if (!dict->sets) {
        printf("Dictionary Calloc Error\n");
        exit(EXIT_FAILURE);
    }

    dict->size = size;
    return dict;
}

void set_add(struct set** set, const char* key, size_t item_size, void* value) {
    struct set* new_node = malloc(sizeof(struct set));
    if (!new_node) {
        printf("Set Malloc Error\n");
        exit(EXIT_FAILURE);
    }

    new_node->data.key = key;
    new_node->data.key_len = strlen(key);
    new_node->data.item_size = item_size;
    new_node->data.value = malloc(item_size);
    if (!new_node->data.value) {
        printf("Set Calloc Error\n");
        exit(EXIT_FAILURE);
    }
    memcpy(new_node->data.value, value, item_size);
    new_node->next = *set;
    *set = new_node;
}

void dictionary_add(struct dictionary* dict, const char* key, size_t item_size, void* value) {
    
    size_t index = hash_function(key) % dict->size;

    if (dict->sets[index] != NULL)
    {
        printf("Collision for key %s!\n",key);
        return;
    }
    

    set_add(&(dict->sets[index]), key, item_size, value);
}

void* set_find(struct set* set, const char* key) {
    while (set != NULL) {
        if (strcmp(set->data.key, key) == 0) {
            return set->data.value;
        }
        set = set->next;
    }
    return NULL;
}

void* dictionary_find(struct dictionary* dict, const char* key) {
    size_t index = hash_function(key) % dict->size;

    return set_find(dict->sets[index], key);
}

void print_dictionary(struct dictionary* dictionary)
{
    printf("Dictionary:\n");
     for (size_t i = 0; i < dictionary->size; i++) {
        struct set* current_set = dictionary->sets[i];
        while (current_set != NULL) {
            if (current_set->data.item_size == sizeof(int)) {
                printf("Key: %s, Value: %d\n", current_set->data.key, *(int*)current_set->data.value);
            } else if (current_set->data.item_size > 0) {
                printf("Key: %s, Value: %s\n", current_set->data.key, (char*)current_set->data.value);
            } else {
                printf("Key: %s, Value: [Unknown type]\n", current_set->data.key);
            }
            current_set = current_set->next;
        }
    }
    printf("-------------------------------\n");
}

int main() {
    size_t dict_size = 10;
    struct dictionary* my_dict = dictionary_init(dict_size);

    int value1 = 100;
    int value2 = 50;
    char name[] = "Player"; 

    dictionary_add(my_dict, "Health", sizeof(int), &value1);
    print_dictionary(my_dict);

    dictionary_add(my_dict, "Stamina", sizeof(int), &value2);
    print_dictionary(my_dict);

    dictionary_add(my_dict, "Name", sizeof(char) * (strlen(name) + 1), name);
    print_dictionary(my_dict);

    dictionary_add(my_dict, "Health", sizeof(int), &value2);

    return 0;
}
