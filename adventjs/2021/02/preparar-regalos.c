#include <stdio.h>
#include <stdlib.h>
#include "../../../common/hashmap.h"

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

void process_gifts(char* letter, HashMap *gifts_map)
{
    if (*letter == 0)
        return;
    letter = trim_left(letter);
    int strikethrough = *letter == '_';
    size_t i = 0;
    while (letter[i] != 0 && letter[i] != ' ')
        i++;
    if (strikethrough)
        return process_gifts(letter+i, gifts_map);
    char* item = copy_substr(letter, letter+i);
    int* amount = (int*)get(*gifts_map, item);
    if (amount) {
        (*amount)++;
    } else {
        amount = (int*)malloc(sizeof(int));
        *amount = 1;
        set(gifts_map, item, amount);
    } 
    process_gifts(letter+i, gifts_map);
}

void print_gifts(HashMap gifts_map)
{
    size_t i = 0;
    size_t found = 0;
    size_t gifts_len = 8;
    while (i < gifts_len) {
        KvPair slot = gifts_map->slots[i];
        if (slot.key) {
            int* amount = (int*)slot.ref;
            printf("  - %d %s\n", *amount, slot.key);
            found++;
        }
        i++;
    }
}

int main()
{
    HashMap gifts_map = NULL;
    char* letter = "bici    coche balón _playstation bici coche peluche";
    process_gifts(letter, &gifts_map);
    print_gifts(gifts_map);
    free_hmap(gifts_map);
    return 0;
}
