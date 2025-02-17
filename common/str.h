#include <stdlib.h>
int str_eq(const char* s1, const char* s2);
int str_starts_with(const char* str, const char* beginning);
void str_set(char** target, const char* origin);
size_t utf8_strlen(const char* s);
size_t str_len(const char* s);
char char_to_upper(char chr);
