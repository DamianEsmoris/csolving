#include <stdio.h>
#define NO_RES {}

typedef struct {
    size_t numbers_len;
    int target;
    int result[2];
    int *numbers;
} Test;

int* sum_pairs(int arr[], size_t arr_len, int target, int *result)
{
    if (arr_len < 2)
        return NULL;
    size_t i = 0;
    size_t j;
    do {
        j = i+1;
        while (j < arr_len && (arr[i] + arr[j]) != target)
            j++;
    } while (i <= arr_len && (arr[i++] + arr[j]) != target);
    if (--i == arr_len)
        return NULL;
    result[0] = arr[i];
    result[1] = arr[j];
    return result;
}

int main()
{
    int tn0[] = {3, 5, 7, 2};
    int tn1[] = {-3, -2, 7, -5};
    int tn2[] = {2, 2, 3, 1};
    int tn3[] = {6, 7, 1, 2};
    int tn4[] = {0, 2, 2, 3, -1, 1, 5};
    int tn5[] = {0, 4, 8};
    int tn6[] = {0, 0, 0};

    Test tests[] = { 
        { sizeof(tn0)/sizeof(int), 10, {3, 7}, tn0 },
        { sizeof(tn1)/sizeof(int), 10, NO_RES, tn1 },
        { sizeof(tn2)/sizeof(int), 4, {2, 2}, tn2 },
        { sizeof(tn3)/sizeof(int), 8, {6, 2}, tn3 },
        { sizeof(tn4)/sizeof(int), 6, {1, 5}, tn4 },
        { sizeof(tn5)/sizeof(int), 12, {4, 8}, tn5 },
        { sizeof(tn6)/sizeof(int), 20, NO_RES, tn6 }
    };

    size_t test_len = sizeof(tests)/sizeof(Test);
    printf("[Pairs]:\n");
    for (size_t i = 0; i < test_len; i++) {
        printf("  %ld. ", i+1);
        Test *test = tests+i;
        int *res = sum_pairs(
            test->numbers,
            test->numbers_len,
            test->target,
            test->result
        );
        if (res) {
            printf(
                "%d + %d = %-2d === %d\n",
                res[0], res[1],
                res[0] + res[1],
                test->target
            );
        } else 
            printf("NULL\n");
    }
}

