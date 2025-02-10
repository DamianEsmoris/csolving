#include "str.h"
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
