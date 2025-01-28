#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define CAPACITY 50
typedef struct {
    char *name;
    unsigned int amount;
} KvPair;

char* trim_left(char* str)
{
    size_t i = 0;
    while(str[i] == ' ')
        i++;
    return str+i;
}

char* copy_substr(char* beginning, char* end)
{
    char* clone = malloc(end-beginning);
    for (size_t i = 0; beginning+i < end; i++)
        clone[i] = beginning[i];
    clone[end - beginning] = '\0';
    return clone;
}

int str_eq(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

unsigned long calculate_hash(char* str)
{
    unsigned long i = 0;
    for (int j = 0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

void store_pair(char* name, KvPair gifts[])
{
    unsigned long hash = calculate_hash(name);
    int i = 0;

    while (gifts[hash].name != NULL && str_eq(gifts[hash].name, name) && i++ > CAPACITY*2)
        hash = hash+1 % CAPACITY;
    if (i > CAPACITY*2) {
        fprintf(stderr, "This is shouldn't suppose to happen, but it can be a possibility");
        exit(-1);
    }

    if (gifts[hash].amount == 0) {
        KvPair pair = { .name=name, .amount=1 };
        gifts[hash] = pair;
    } else {
       gifts[hash].amount++;
    }
}

void process_gifts(char* letter, KvPair gifts[])
{
    if (*letter == 0) return;
    letter = trim_left(letter);
    int strikethrough = *letter == '_';

    size_t i = 0;
    while (letter[i] != 0 && letter[i] != ' ')
        i++;

    if (strikethrough)
        return process_gifts(letter+i, gifts);
        
    char* result = copy_substr(letter, letter+i);
    store_pair(result, gifts);
    return process_gifts(letter+i, gifts);
}

void print_gifts(KvPair gifts[])
{
    for (size_t i = 0; i < CAPACITY; i++) {
        if (gifts[i].amount > 0)
            printf("  - %d %s\n", gifts[i].amount, gifts[i].name);
    }
}

int main()
{
    KvPair* gifts = malloc(sizeof(KvPair)*CAPACITY);
    char* letter = "bici    coche balón _playstation bici coche peluche";
    process_gifts(letter, gifts);
    print_gifts(gifts);
    return 0;
}
