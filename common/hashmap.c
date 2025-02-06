#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "str.h"

#define assert(assertion, msg, ...) if (!assertion) {\
    printf("[ASSERTION FAILED] (%s:%d): " #msg, __FILE_NAME__, __LINE__, __VA_ARGS__); \
    exit(1); \
}

size_t INITAL_CAP = 10;
size_t INC_MULT = 1;

#define dbug 0
#define print(msg, ...) if (dbug) \
    printf(msg __VA_OPT__(,) __VA_ARGS__)


unsigned long calculate_hash(char* str, size_t size)
{
    unsigned long i = 0;
    size_t j = 0;
    while (str[j] != 0)
        i += str[j++];
    print("    + the hash is: %ld\n", (i % size));
    return i % size;
}

void initalize_hmap(HashMap* hmap)
{
    *hmap = (MapHeader*)malloc(sizeof(MapHeader));
    (*hmap)->occupied = 0;
    (*hmap)->size = INITAL_CAP;
    (*hmap)->slots = malloc(INITAL_CAP*sizeof(KvPair));
    for (size_t i = 0; i < (*hmap)->size; i++)
        (*hmap)->slots[i] = (KvPair){ NULL, NULL };
    print("  # hmap: initalized\n", NULL);
}

void increase_hmap(HashMap hmap)
{
    KvPair *old_slots = hmap->slots;
    size_t old_size = hmap->size;
    hmap->size += hmap->size * INC_MULT;
    hmap->slots = malloc(hmap->size*sizeof(KvPair));
    for (size_t i = 0; i < old_size; i++) {
        if (old_slots[i].key) {
            size_t hash = calculate_hash(old_slots[i].key, hmap->size);
            hmap->slots[hash] = old_slots[i];
        }
        if (!hmap->slots[i].key)
            hmap->slots[i] = (KvPair){ NULL, NULL };
    }
    for (size_t i = old_size; i < hmap->size; i++) {
        if (!hmap->slots[i].key)
            hmap->slots[i] = (KvPair){ NULL, NULL };
    }
    free(old_slots);
}

void free_hmap(HashMap hmap)
{
    size_t found = 0;
    size_t i = 0;
    while (found < hmap->occupied) {
        if (hmap->slots[i].key != NULL) {
            free(hmap->slots[i].ref);
            found++;
        }
        i++;
    }
    free(hmap);
}

void* store_pair(HashMap *hmap, KvPair kv)
{
    if (!*hmap) {
        initalize_hmap(hmap);
    } else if ((*hmap)->occupied == (*hmap)->size)
    {
        print("  # hmap: full, pre increasing hmap...\n", NULL);
        increase_hmap(*hmap);
    }
    MapHeader *map = *hmap;
    print("  # hmap: %ld/%ld \n", map->occupied, map->size);
    unsigned long hash = calculate_hash(kv.key, map->size);
    print("    + (sto) the key is '%s'\n", kv.key);
    if (map->slots[hash].key) {
        int bound = hash;
        do 
            hash = (hash+1) % map->size;
        while (
            map->slots[hash].key != NULL
            && !str_eq(map->slots[hash].key, kv.key)
            && hash != bound
        );
        assert((hash != bound), "Full iteration... the hasmap doesn't have any free space and the pre reservation didn't catch it!", NULL);
        print("    + (sto) the hash after collision chk is %ld\n", hash);
    }
    print("    + (sto) the hash is %ld\n", hash);
    map->occupied++;
    map->slots[hash] = kv;
    print("    + (sto) value stored\n");
    return map->slots+hash;
}

void* get_pair(HashMap hmap, char* key)
{
    if (!hmap) return NULL;
    print("    # hmap: size = %ld (ret)\n", hmap->size);
    unsigned long hash = calculate_hash(key, hmap->size);
    print("    + (ret) the key is '%s'\n", key);
    if (!hmap->slots[hash].key)
        return NULL;
    if (!str_eq(hmap->slots[hash].key, key)) {
        int bound = hash;
        do 
            hash = (hash+1) % hmap->size;
        while (
            hmap->slots[hash].key 
            && !str_eq(hmap->slots[hash].key, key)
            && hash != bound
        );
        if (hash == bound || (!hmap->slots[hash].key || !str_eq(hmap->slots[hash].key, key)))
            return NULL;
    }
    return hmap->slots[hash].ref;

}

Linklst keys(HashMap hmap)
{
    Linklst keys = NULL;
    size_t i = 0;
    size_t found = 0;
    while (i < hmap->size && found < hmap->occupied) {
        KvPair slot = hmap->slots[i];
        if (slot.ref) {
            print("    + (key) found key: '%s'\n", slot.key);
            push(&keys, slot.key);
            found++;
        }
        i++;
    }
    return keys;
}
