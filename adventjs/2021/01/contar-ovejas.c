#include <stdio.h>
#include "linklst.h"

#define linklst(l,...) do {                                        \
    Sheep sheep[] = { __VA_ARGS__ };                               \
    for (size_t i = 0; i < sizeof(sheep)/sizeof(sheep[0]); i++)    \
        push(&l,(void*)&sheep[i]);                                 \
} while(0)

typedef enum {
    RED,
    WHITE,
    PINK,
    YELLOW,
    ORANGE,
    VIOLET,
    GREEN,
    BLUE,
    PURPLE,
} Color;

typedef struct {
    char name[64];
    Color color;
} Sheep;

char to_lower(char chr)
{
    return (chr >='A' && chr<='Z') ? (chr + 32) : (chr);
}

int str_contains_char(char* str, char chr)
{
    while (*str != 0  && to_lower(*str) != to_lower(chr))
        str++;
    return *str != 0;
}

Linklst filter_sheep(Linklst sheep, Color color, char *chars)
{
    Linklst filtered_sheep = NULL;
    Node* node = sheep->first;
    while (node) {
        Sheep* sheep = node->ref;
        if (sheep->color == color) {
            size_t i = 0; 
            while (chars[i] != 0 && str_contains_char(sheep->name, chars[i]))
                i++;
            if (chars[i] == 0) 
              push(&filtered_sheep, sheep);
        } 
        node = node->next;
    }
    return filtered_sheep;
}

void print_sheep_names(Linklst l)
{
    if (!l) return;
    Node* node = l->first;
    while (node) {
        Sheep* sheep = node->ref;
        printf("- %s\n", sheep->name);
        node = node->next;
    }
}


int main()
{
    Linklst sheep_list = NULL;

    linklst(
        sheep_list,
        { .name="Noa", .color=BLUE },
        { .name="Euge", .color=RED },
        { .name="Navidad", .color=RED },
        { .name="Ki Na Ma", .color=RED },
        { .name="Sara", .color=RED },
        { .name="Nicolas", .color=RED },
        { .name="NEFARIUS", .color=RED }
    );

    Linklst chosen_sheep = filter_sheep(sheep_list, RED, "ns");
    printf("There are %d chosen sheep!\n", length(chosen_sheep));
    print_sheep_names(chosen_sheep);
    return 0;
}
