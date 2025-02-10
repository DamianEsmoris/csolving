#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../common/str.h"
#define size(arr) sizeof(arr)/sizeof(arr[0])

typedef struct {
    char **rows;
    size_t rows_len;
} Present;

typedef struct {
    char **gifts;
    size_t gifts_len;
    char **expected_ouput;
} Test ;

Present* wrap_gifts(char **gifts, size_t gifts_len)
{
    const unsigned BORDER_WIDTH = 1;
    const unsigned BORDER_HEIGHT = 1;
    const unsigned int GIFT_WIDTH = 2;

    size_t gifts_per_row = utf8_strlen(gifts[0]);
    size_t width = BORDER_WIDTH*2 + gifts_per_row*GIFT_WIDTH;
    size_t acutal_width = BORDER_WIDTH*2 + str_len(gifts[0]); 
    size_t height = BORDER_HEIGHT*2 + gifts_len;
    size_t current_line = 0;

    char** present = malloc(height*sizeof(char*));
    for (size_t i = 0; i < height; i++)
        present[i] = malloc(sizeof(char)*acutal_width);
    for (size_t i = 0; i < BORDER_HEIGHT; i++) {
        for (size_t j = 0; j < width; j++)
            present[current_line][j] = '*';
        present[current_line++][width] = 0;
    }
    for (size_t i = 0; i < gifts_len; i++) {
        for (size_t j = 0; j < BORDER_WIDTH; j++)
            present[current_line][j] = '*';
        acutal_width = BORDER_WIDTH*2 + str_len(gifts[i]); 
        for (size_t j = 0; j < acutal_width-BORDER_WIDTH; j++)
            present[current_line][j+BORDER_WIDTH] = gifts[i][j];
        for (size_t j = acutal_width-BORDER_WIDTH; j < acutal_width; j++)
            present[current_line][j] = '*';

        present[current_line][acutal_width] = 0;
        current_line++;
    }
    for (size_t i = 0; i < BORDER_HEIGHT; i++) {
        for (size_t j = 0; j < width; j++)
            present[current_line][j] = '*';
        present[current_line++][width] = 0;
    }
    Present *p = malloc(sizeof(Present));
    p->rows = present;
    p->rows_len = height;
    return p;
}

int is_array_eq(char **s1, char** s2, size_t len)
{
    size_t i = 0;
    while (i < len && str_eq(s1[i], s2[i]))
        i++;
    return i == len;
}

void print_present(char **present, size_t present_len)
{
    for (size_t i = 0; i < present_len; i++)
        printf("    %s\n", present[i]);
    printf("\n");
}

int main()
{
    char* gif_0[] = {"📷", "⚽️"}; 
    char* out_0[] = {"****", "*📷*", "*⚽️*", "****"};
    char* gif_1[] = {"🏈🎸", "🎮🧸"};
    char* out_1[] = {"******", "*🏈🎸*", "*🎮🧸*", "******"};
    char* gif_2[] = {"📷"};
    char* out_2[] = {"****", "*📷*", "****"};
    Test tests[] = {
        { gif_0, size(gif_0), out_0 },
        { gif_1, size(gif_1), out_1 },
        { gif_2, size(gif_2), out_2 },
    };
    size_t tests_len = sizeof(tests)/sizeof(Test);
    printf("[Envolviendo]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        Present* out = wrap_gifts(tests[i].gifts, tests[i].gifts_len);
        printf("  - %-3s\n", is_array_eq(out->rows, tests[i].expected_ouput, out->rows_len) ? "ok" : "err");
        print_present(out->rows, out->rows_len);
    }
    return 0;
}
