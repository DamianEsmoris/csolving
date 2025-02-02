#include <stdio.h>

#define set(hm, ke, va) do { \
    KvPair kv___LINE__ = { (void*) va, ke };\
    store_pair(hm, kv___LINE__);\
} while(0)
#define get(hm, ke) get_pair(hm, ke)

typedef struct {
    void *ref;
    char *key;
} KvPair;

typedef struct {
    size_t size;
    size_t occupied;
    KvPair *slots;
} MapHeader;

typedef MapHeader * HashMap;
unsigned long calculate_hash(char* str, size_t size);
void initalize_hmap(HashMap* hmap);
void* store_pair(HashMap *hmap, KvPair kv);
void* get_pair(HashMap hmap, char* key);
void free_hmap(HashMap hmap);
