#include <stdio.h>
#define NO_RES {}

typedef struct {
    size_t numbers_len;
    int result[2];
    int expected;
    int *numbers;
} Test;

int max_profit(int history[], size_t history_len, int* result)
{

    size_t min_index = 0;
    size_t i = 0;
    while (i < history_len - 1) {
        if (history[i] < history[min_index])
            min_index = i;
        i++;
    }
    i = min_index+1;
    int min = history[min_index];
    int max = history[i++];
    while (i < history_len) {
        if (history[i] > max)
            max = history[i];
        i++;
    }
    result[0] = min;
    result[1] = max;
    return max > min ? max - min : 1;
}

int main()
{
    int tn0[] = {39, 18, 29, 25, 34, 32, 5};
    int tn1[] = {10, 20, 30, 40, 50, 60, 70};
    int tn2[] = {18, 15, 12, 11, 9, 7};
    int tn3[] = {3, 3, 3, 3, 3};

    Test tests[] = { 
        { sizeof(tn0)/sizeof(int), {18, 34}, 16, tn0 },
        { sizeof(tn1)/sizeof(int), {10, 70}, 60, tn1 },
        { sizeof(tn2)/sizeof(int), NO_RES, -1, tn2 },
        { sizeof(tn3)/sizeof(int), NO_RES, -1, tn3 }
    };
    size_t test_len = sizeof(tests)/sizeof(Test);
    printf("[BTC]:\n");
    for (size_t i = 0; i < test_len; i++) {
        Test *test = tests+i;
        int profit = max_profit(
            test->numbers,
            test->numbers_len,
            test->result
        );
        printf("  %ld. ", i+1);
        if (profit == -1)
            printf("-> %-2d (no hay ganancia posible)", profit);
        else
            printf(
                "-> %-2d (compra a %-2d, vende a %-2d) === %d\n",
                profit,
                test->result[0],
                test->result[1],
                test->expected
            );
    }
}
