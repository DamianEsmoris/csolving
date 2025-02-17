#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "hashmap.h"
#include "str.h"

#define assert(assertion, msg, ...) if (!assertion) {\
    printf("[ASSERTION FAILED] (%s:%d): " #msg, __FILE_NAME__, __LINE__, __VA_ARGS__); \
    exit(1); \
}

size_t INITAL_CAP = 10;
double INC_MULT = 0.5;

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
    size_t old_occupied = hmap->occupied;
    hmap->size += hmap->size * INC_MULT;
    hmap->slots = malloc(hmap->size*sizeof(KvPair));
    old_occupied = hmap->occupied;
    hmap->occupied = 0;;
    for (size_t i = 0; i < hmap->size; i++)
            hmap->slots[i] = (KvPair){ NULL, NULL };
    size_t i, found;
    i = found = 0;
    while (found < old_occupied && i < old_size) {
        if (old_slots[i].key) {
            store_pair(&hmap, old_slots[i]);
            found++;
        }
        i++;
    }
    free(old_slots);
}

void hmap_free_keys(HashMap hmap) {
    size_t found = 0;
    size_t i = 0;
    while (found < hmap->occupied) {
        if (hmap->slots[i].key) {
            free(hmap->slots[i].key);
            found++;
        }
        i++;
    }
    free(hmap);
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
    print("      + %ld && %d\n", hash,map->slots[hash].key != NULL);
    if (map->slots[hash].key) {
        unsigned long bound = hash;
        do {
            hash = (hash+1) % map->size;
            print("      + %ld && %d\n", hash,map->slots[hash].key != NULL);
        } while (
            (map->slots[hash].key && !str_eq(map->slots[hash].key, kv.key))
            && hash != bound
        );
        print("    + (sto) %ld == %ld\n", hash, bound);
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
        unsigned long bound = hash;
        do 
            hash = (hash+1) % hmap->size;
        while (
            (!hmap->slots[hash].key || !str_eq(hmap->slots[hash].key, key))
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

void hmap_clone(HashMap source, HashMap *target)
{
    *target = (MapHeader*)malloc(sizeof(MapHeader));
    (*target)->occupied = source->occupied;
    (*target)->size = source->size;
    (*target)->slots = malloc(source->size*sizeof(KvPair));
    print("  # hmap: structure cloned (size: %ld/occupied: %ld)\n", (*target)->size, (*target)->occupied);
    for (size_t hash = 0; hash < source->size; hash++) {
        str_set(
            &(*target)->slots[hash].key,
            source->slots[hash].key
        );
        (*target)->slots[hash].ref = source->slots[hash].ref;
    }

    print("  # hmap: all the data was cloned");
}

void remove_by_key(HashMap hmap, char* key)
{
    unsigned long hash = calculate_hash(key, hmap->size);
    print("    + (rem) the key is '%s'\n", key);
    if (!hmap->slots[hash].key)
        return;
    if (!str_eq(hmap->slots[hash].key, key)) {
        unsigned long bound = hash;
        do
            hash = (hash+1) % hmap->size;
        while (
            (!hmap->slots[hash].key || !str_eq(hmap->slots[hash].key, key))
            && hash != bound
        );
        if (hash == bound || (!hmap->slots[hash].key || !str_eq(hmap->slots[hash].key, key))) {
            print("      - (rem) the key '%s' is not present\n", key);
            return;
        }
    }
    free(hmap->slots[hash].key);
    print("      + (rem) key '%s' removed\n", key);
    print("  # hmap: occupied: %ld\n", hmap->occupied);
    hmap->occupied--;
    hmap->slots[hash].ref = NULL;
    hmap->slots[hash].key = NULL;
}
