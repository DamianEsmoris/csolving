#include <stdio.h>
#include <stdlib.h>

const unsigned int COINS[] = {1, 2, 5, 10, 20, 50};
#define COINS_LEN (sizeof(COINS)/sizeof(unsigned int))

typedef struct {
    unsigned int n;
    unsigned int expected_change[COINS_LEN];
} Test;

unsigned int* get_coins(int n)
{
    unsigned int* change = malloc(sizeof(unsigned int)*COINS_LEN);
    for (size_t i = COINS_LEN-1; i > 0; i--){
        change[i] = n/COINS[i];
        n = n%COINS[i];
    }
    change[0] = n;
    return change;
}

void print_change(unsigned int* change)
{
    printf("[");
    for (size_t i = 0; i < COINS_LEN-1; i++)
        printf("%d, ", change[i]);
    printf("%d]\n", change[COINS_LEN-1]);
}

int is_array_eq(unsigned int* a1, unsigned int* a2, size_t len)
{
    do
        len--;
    while (a1[len] == a2[len] && len > 0);
    return len == 0;
}
int main()
{
    Test tests[] = {
        { 51, {1, 0, 0, 0, 0, 1} },
        { 3, {1, 1, 0, 0, 0, 0} },
        { 5, {0, 0, 1, 0, 0, 0} },
        { 16, {1, 0, 1, 1, 0, 0} },
        { 100, {0, 0, 0, 0, 0, 2} },
    };
    size_t tests_len = sizeof(tests)/sizeof(Test);
    printf("[Transacciones]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        unsigned int* change = get_coins(tests[i].n);
        printf("  - %-3s %-3d -> ", is_array_eq(change, tests[i].expected_change, COINS_LEN) ? "ok" : "err", tests[i].n);
        print_change(change);
    }
    return 0;
}
