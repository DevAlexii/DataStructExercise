#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_collision_count 2
#define max_hashmap_size 1000

size_t djb33x_hash(const char* key) {
    size_t hash = 5381;
    size_t key_len = strlen(key);

    for (size_t i = 0; i < key_len; i++) {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

struct data {
    const char* key;
    size_t key_len;
    size_t item_size;
    int collision_count;
    void* value;
};

struct set {
    struct data data;
};

struct dictionary {
    struct set* sets;
    size_t size;
};

void print_dictionary(struct dictionary* dict) {
    printf("------------------------------------------\n");
    printf("Updated Dictionary:\n");
    for (size_t i = 0; i < dict->size; i++) {
        struct data data = dict->sets[i].data;
        if (data.key_len != 0) {
            if (data.item_size == sizeof(int)) {
                printf("Key: %s with value: %d at index: %zu (Collision count: %d)\n", data.key, *(int*)data.value, i, data.collision_count);
            } else if (data.item_size > 0) {
                printf("Key: %s with value: %s at index: %zu (Collision count: %d)\n", data.key, (char*)data.value, i, data.collision_count);
            } else {
                printf("Key: %s with unknown value at index: %zu (Collision count: %d)\n", data.key, i, data.collision_count);
            }
        }
    }
    printf("Dictionary size: %zu\n", dict->size);
}

struct dictionary* dictionary_init(size_t initial_size) {
    struct dictionary* dict = malloc(sizeof(struct dictionary));
    if (!dict) {
        printf("Dictionary Malloc Error\n");
        return NULL;
    }

    dict->sets = malloc(initial_size * sizeof(struct set));
    if (!dict->sets) {
        printf("Sets Malloc Error\n");
        free(dict);
        return NULL;
    }

    for (size_t i = 0; i < initial_size; i++) {
        dict->sets[i].data.collision_count = 0;
        dict->sets[i].data.item_size = 0;
        dict->sets[i].data.key = NULL;
        dict->sets[i].data.key_len = 0;
        dict->sets[i].data.value = NULL;
    }

    dict->size = initial_size;

    printf("Finish Initialize Dictionary!\n");
    return dict;
}

void dictionary_resize(struct dictionary** dict) {
    printf("------------------------------------------\n");
    printf("Need a resize 'cause collision count reached the max:\n");
    printf("Try resizing....\n");

    size_t new_size = (*dict)->size * 2;

    if (new_size > max_hashmap_size) {
        printf("Resize failing!! Dictionary is at its max size!\n");
        return;
    }

    struct dictionary* new_dict = malloc(sizeof(struct dictionary));
    if (!new_dict) {
        printf("Dictionary Resize Malloc Error\n");
        return;
    }

    new_dict->sets = malloc(new_size * sizeof(struct set));
    if (!new_dict->sets) {
        printf("Sets Malloc Error\n");
        free(new_dict);
        return;
    }

    new_dict->size = new_size;

    for (size_t i = 0; i < new_size; i++) {
        new_dict->sets[i].data.collision_count = 0;
        new_dict->sets[i].data.item_size = 0;
        new_dict->sets[i].data.key = NULL;
        new_dict->sets[i].data.key_len = 0;
        new_dict->sets[i].data.value = NULL;
    }

    for (size_t i = 0; i < (*dict)->size; i++) {
        if ((*dict)->sets[i].data.key_len != 0) {
            size_t new_index = djb33x_hash((*dict)->sets[i].data.key) % new_size;
            new_dict->sets[new_index] = (*dict)->sets[i];
            new_dict->sets[new_index].data.collision_count = 0;
        }
    }

    free((*dict)->sets);

    free(*dict);

    *dict = new_dict;

    print_dictionary(*dict);
}

int dictionary_add(struct dictionary** dict, const char* key, size_t item_size, void* value) {
    printf("------------------------------------------\n");
    size_t index = djb33x_hash(key) % (*dict)->size;

    if (item_size == sizeof(int)) {
        printf("Try To add key: %s with value: %d at index: %zu\n", key, *(int*)value, index);
    } else if (item_size > 0) {
        printf("Try To add key: %s with value: %s at index: %zu\n", key, (char*)value, index);
    } else {
        printf("Try To add key: %s with unknown value at index: %zu\n", key, index);
    }

    if ((*dict)->sets[index].data.key_len == 0) {

        (*dict)->sets[index].data.key = key;
        (*dict)->sets[index].data.key_len = strlen(key);
        (*dict)->sets[index].data.item_size = item_size;
        (*dict)->sets[index].data.value = value;

        printf("Successfully added key: %s at index: %zu\n", key, index);
        print_dictionary(*dict);
        return 1;
    }

    printf("Collision at index %zu, there's already an item with Key: %s\n", index, (*dict)->sets[index].data.key);
    (*dict)->sets[index].data.collision_count++;
    print_dictionary(*dict);

    if ((*dict)->sets[index].data.collision_count >= max_collision_count) {
        dictionary_resize(dict);
    }
    return 0;
}

int main() {
    size_t initial_size = 10;
    struct dictionary* my_dictionary;
    printf("Dictionary Initialize..\n");
    my_dictionary = dictionary_init(initial_size);

    int start_life = 100;
    int start_stamina = 50;

    dictionary_add(&my_dictionary, "Health", sizeof(int), &start_life);
    dictionary_add(&my_dictionary, "Stamina", sizeof(int), &start_stamina);

    dictionary_add(&my_dictionary, "World", sizeof(char), "Earth");
    dictionary_add(&my_dictionary, "Hello", sizeof(char), "Hola");
    dictionary_add(&my_dictionary, "Hello", sizeof(char), "Hola");
    dictionary_add(&my_dictionary, "Hello", sizeof(char), "Hola");


    return 0;
}
