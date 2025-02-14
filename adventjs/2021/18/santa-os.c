#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../common/hashmap.h"
#include "../../../common/str.h"

#define size(arr) sizeof(arr)/sizeof(arr[0])

typedef struct {
    char** files;
    char** expected_files;
    size_t files_len;
} Test;

char* fil_0[] = {"photo", "postcard", "photo", "photo", "video"};
char* exp_0[] = {"photo", "postcard", "photo(1)", "photo(2)", "video"};
char* fil_1[] = {"file", "file", "file", "game", "game"};
char* exp_1[] = {"file", "file(1)", "file(2)", "game", "game(1)"};
char* fil_2[] = {"file", "file(1)", "icon", "icon(1)", "icon(1)"};
char* exp_2[] = {"file", "file(1)", "icon", "icon(1)", "icon(1)(1)"};
Test tests[] = {
    {fil_0, exp_0, size(fil_0)},
    {fil_1, exp_1, size(fil_1)},
    {fil_2, exp_2, size(fil_2)},
};
size_t tests_len = size(tests);

char* make_unique_filename(char* filename, unsigned int n)
{
    const size_t MAX_INT_DIGITS = 1;
    size_t filename_len = str_len(filename) + 2 + MAX_INT_DIGITS;
    char *new_filename = malloc(filename_len*(sizeof(char)));
    sprintf(new_filename, "%s(%d)", filename, n);
    return new_filename;
}

char** fix_files(char** files, size_t files_len)
{
    HashMap files_seen = NULL;
    char** fixed_files = malloc(files_len*sizeof(char**));
    unsigned int* hasmap_entry = malloc(sizeof(unsigned int));
    *hasmap_entry = 1;
    char* filename = files[0];
    fixed_files[0] = filename;
    set(&files_seen, filename, hasmap_entry);
    for (size_t i = 1; i < files_len; i++) {
        filename = files[i];
        hasmap_entry = get(files_seen, filename);
        if (hasmap_entry) {
            filename = make_unique_filename(filename, (*hasmap_entry)++);
        } else {
            hasmap_entry = malloc(sizeof(unsigned int));
            *hasmap_entry = 1;
            set(&files_seen, filename, hasmap_entry);
        }
        fixed_files[i] = filename;
    }
    return fixed_files;
}

int is_array_eq(char **s1, char** s2, size_t len)
{
    size_t i = 0;
    while (i < len && str_eq(s1[i], s2[i]))
        i++;
    return i == len;
}

void print_array(char** arr, size_t arr_len)
{
    if (arr_len == 0) {
        printf("[]\n");
        return;
    }
    printf("[");
    for (size_t i = 0; i < arr_len-1; i++)
        printf("'%s', ", arr[i]);
    printf("'%s']\n", arr[arr_len-1]);
}

int main()
{
    printf("[Haciendo único y especial a cada archivo...]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        char** fixed_filenames = fix_files(tests[i].files, tests[i].files_len);
        if (is_array_eq(fixed_filenames, tests[i].expected_files, tests[i].files_len)) {
            printf("  - ok ");
            print_array(fixed_filenames, tests[i].files_len);
        } else {
            printf("  - err \n      expected: ");
            print_array(tests[i].expected_files, tests[i].files_len);
            printf("      result: ");
            print_array(fixed_filenames, tests[i].files_len);
        }
    }
    return 0;
}
