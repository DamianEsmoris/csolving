#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define size(arr) sizeof(arr)/sizeof(arr[0])
#define NaN -1 

typedef struct {
    char *letter;
    int expected_ouput;
} Test ;


int parse_ch_to_value(char chr)
{
    switch (chr) {
        case 0:
            return 0;
        case '.':
            return 1;
        case ',':
            return 5;
        case ':':
            return 10;
        case ';':
            return 50;
        case '!':
            return 100;
    }
    return NaN;
}

int get_next_token(char **letter)
{
    return parse_ch_to_value(*(*letter)++);
}

int peek_next_token(char *letter)
{
    return parse_ch_to_value(*letter);
}

int decode_numbers(char *letter)
{
    int result = 0;
    int tk;
    while ((tk = get_next_token(&letter)) > 0) {
        if (peek_next_token(letter) > tk){
            tk *= -1;
        }
        result += tk;
    }
    return tk == NaN ? NaN : result;
}


int main()
{
    Test tests[] = {
        { "...", 3 },
        { ".,", 4 },
        { ",.", 6  },
        { ",...", 8  },
        { ".........!", 107 },
        { ".;", 49 },
        { "..,", 5 },
        { "..,!", 95 },
        { ".;!", 49  },
        { "!!!", 300 },
        { ";!", 50 },
        { ";.W", NaN },
    };
    size_t tests_len = sizeof(tests)/sizeof(Test);
    printf("[Decodificando cartas...]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        int result = decode_numbers(tests[i].letter);
        if (result == tests[i].expected_ouput)
            printf("  - ok  (%d)\n", result);
        else
            printf("  - err expected (%d) but got (%d)\n", tests[i].expected_ouput, result);
    }

    return 0;
}
