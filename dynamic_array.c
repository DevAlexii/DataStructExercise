#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h> 

struct dynamic_array{
    size_t count;
    size_t capacity;
    size_t item_size;
    void* data;
};

struct dynamic_array* dynamic_array_init(const size_t intial_capacity,const size_t item_size){
    struct dynamic_array* dynamic_array = malloc(sizeof(struct dynamic_array));
    if (!dynamic_array)
    {
        return NULL;
    }
    
    dynamic_array->count = 0;
    dynamic_array->capacity = intial_capacity;
    dynamic_array->item_size = item_size;
    dynamic_array->data = malloc(item_size * intial_capacity);
    if (!dynamic_array->data)
    {
        free(dynamic_array);
        return NULL;
    }
    return dynamic_array;
}

long long dynamic_array_add(struct dynamic_array* dynamic_array, const void* value){

    if (dynamic_array->count + 1 > dynamic_array->capacity)
    {
        printf("Reached capacity!\n");
        //Realloc!
        dynamic_array->capacity *= 2;
        void* new_data = realloc(dynamic_array->data,dynamic_array->item_size*dynamic_array->capacity);
        if (!new_data)
        {
            printf("Failed to realloc!\n");
            return - 1;
        }
        dynamic_array->data = new_data;
    }
    else
    {
        memcpy(((char*)dynamic_array->data) + (dynamic_array->item_size * dynamic_array->count),value,dynamic_array->item_size);
    }

    dynamic_array->count++;

    return dynamic_array->count - 1;
}

int main(int argc,char** argv){

    struct dynamic_array* my_array = dynamic_array_init(2,sizeof(int));
    if (!my_array)
    {
        printf("Unable to create dynamic array\n");
        return -1;
    }
    
    int value = 100;
    for (int i = 0; i < 10; i++)
    {
        printf("added at %lld\n",dynamic_array_add(my_array,&value));
        printf("Capacity: %llu\n",my_array->capacity);
    }
    
    return 0;
}