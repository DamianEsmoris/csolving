#include "str.h"
#include <stdio.h>
int str_starts_with(const char* str, const char* beginning)
{
    while (*beginning != 0 && (*beginning == *str)) {
        beginning++;
        str++;
    }
    return *beginning == 0;
}

void str_set(char** target, const char* origin) {
    if (!*target)
        free(*target);
    if (!origin) {
        *target = NULL;
        return;
    }
    size_t origin_len = str_len(origin);
    char* new_str = malloc((origin_len + 1) * sizeof(char));
    for (size_t i = 0; i <= origin_len; i++) 
        new_str[i] = origin[i];
    *target = new_str;
}

int str_eq(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

size_t utf8_strlen(const char* s)
{
    size_t count = 0;
    while (*s)
        count += (*s++ & 0XC0) != 0x80;
    return  count;
}

size_t str_len(const char* s)
{
    size_t len = 0;
    while (*s++ != 0)
        len++;
    return len;
}

char char_to_upper(char chr)
{
    return (chr >= 'a' && chr <= 'z') ?
        chr + ('A' - 'a') :
        chr;
}
