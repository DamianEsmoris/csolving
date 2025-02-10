#include <stdio.h>
#include <stdlib.h>
#define size(arr) sizeof(arr)/sizeof(arr[0])

typedef struct {
    unsigned int *reindeers;
    size_t reindeers_len;
    unsigned int expected_ouput;
} Test ;

unsigned int missing_reindeer(unsigned int *reindeers, size_t reindeers_len)
{
    unsigned int expected = (reindeers_len*reindeers_len + reindeers_len)/2;
    unsigned int sum = 0;
    for (size_t i = 0; i < reindeers_len; i++)
        sum += reindeers[i];
    return expected-sum;
}


int main()
{
    unsigned int rei_0[] = {0, 2, 3};
    unsigned int rei_1[] = {5, 6, 1, 2, 3, 7, 0};
    unsigned int rei_2[] = {0, 1};
    unsigned int rei_3[] = {3, 0, 1};
    unsigned int rei_4[] = {9, 2, 3, 5, 6, 4, 7, 0, 1};
    unsigned int rei_5[] = {0};
    unsigned int rei_6[] = {1};
    Test tests[] = {
        { rei_0, size(rei_0), 1 },
        { rei_1, size(rei_1), 4 },
        { rei_2, size(rei_2), 2 },
        { rei_3, size(rei_3), 2 },
        { rei_4, size(rei_4), 8 },
        { rei_5, size(rei_5), 1 },
    };
    size_t tests_len = sizeof(tests)/sizeof(Test);
    printf("[Buscando renos]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        unsigned int missing = missing_reindeer(tests[i].reindeers, tests[i].reindeers_len);
        if (missing == tests[i].expected_ouput)
            printf("  - ok  (%d)\n", missing);
        else
            printf("  - err expected (%d) but got (%d)\n", tests[i].expected_ouput, missing);
    }
        return 0;
}
