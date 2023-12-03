#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define hashmap_size 64
#define hashmap_size_list 2


struct set_item{
    const char* key;
    size_t key_len;
};

struct set{
    struct set_item hasmap[hashmap_size][hashmap_size_list];
};

size_t djb33x_hash(const char* key,const size_t key_len){
    
    size_t hash = 5381;

    for (size_t i = 0; i < key_len; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

void set_insert(struct set* set, const char* key){

    const size_t key_len = strlen(key);

    const size_t hash = djb33x_hash(key,key_len);
    
    const size_t index = hash % hashmap_size;

    printf("Hash of %s = %llu (index: %llu)\n",key,hash,index);

    for (size_t i = 0; i < hashmap_size_list; i++)
    {
        if (set->hasmap[index][i].key_len == 0)
        {
            set->hasmap[index][i].key = key;
            set->hasmap[index][i].key_len = key_len;
            printf("Added %s at index %llu slot %llu\n",key,index,i);
            return;
        }
    }
    printf("Collision! for %s (index: %llu)\n",key,index);
}

int set_search(struct set* set, const char* key){

    const size_t key_len = strlen(key);

    const size_t hash = djb33x_hash(key,key_len);
    
    const size_t index = hash % hashmap_size;

    printf("Hash of %s = %llu (index: %llu)\n",key,hash,index);

    for (size_t i = 0; i < hashmap_size_list; i++)
    {
        if(set->hasmap[index][i].key_len > 0)
        {
            if (set->hasmap[index][i].key_len == key_len && !memcmp(set->hasmap[index][i].key, key,key_len))
            {
                printf("Found %s at index %llu slot %llu\n",key,index,i);
                return 0;
            }
        }
    }
    printf("Item %s not found!\n",key);
    return -1;
}

int set_remove(struct set* set, const char* key){

    const size_t key_len = strlen(key);

    const size_t hash = djb33x_hash(key,key_len);
    
    const size_t index = hash % hashmap_size;

    for (size_t i = 0; i < hashmap_size_list; i++)
    {
        if(set->hasmap[index][i].key_len > 0)
        {
            if (set->hasmap[index][i].key_len == key_len && !memcmp(set->hasmap[index][i].key, key,key_len))
            {
                set->hasmap[index][i].key = 0;
                set->hasmap[index][i].key_len = 0;
                printf("%s successfully removed at index %llu slot %llu\n",key,index,i);
                return 0;
            }
        }
    }
    printf("%s is not found!\n",key);
    return -1;
}

int set_insert_unique_key(struct set* set, const char* key){
    const size_t key_len = strlen(key);

    const size_t hash = djb33x_hash(key,key_len);
    
    const size_t index = hash % hashmap_size;

    printf("Hash of %s = %llu (index: %llu)\n",key,hash,index);

    for (size_t i = 0; i < hashmap_size_list; i++)
    {
        if (set->hasmap[index][i].key_len == 0)
        {
            set->hasmap[index][i].key = key;
            set->hasmap[index][i].key_len = key_len;
            printf("Added %s at index %llu slot %llu\n",key,index,i);
            return 0;
        }
        else if (set->hasmap[index][i].key_len == key_len && strcmp(set->hasmap[index][i].key, key) == 0)
        {
            printf("%s already exist at index: %llu slot %llu, cannot be added\n",key,index,i);
            return -1;
        }
    }
    return 0;
}

int main(int argc, char** argv){
    
    struct set my_set;
    memset(&my_set,0,sizeof(struct set));

    set_insert(&my_set,"Hello");
    set_insert_unique_key(&my_set,"Hello");
    set_insert(&my_set,"XYZ");

    set_search(&my_set,"XYZ");

    printf("-------------------------\n");
    printf("Try to remove Hello\n");
    set_remove(&my_set,"Hello");
    set_remove(&my_set,"Hello");

    return 0;
}