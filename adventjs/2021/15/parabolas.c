#include <stdio.h>
#include <stdlib.h>
#define size(arr) sizeof(arr)/sizeof(arr[0])
#define assert(assertion, msg, ...) if (!assertion) {\
    printf("[ASSERTION FAILED] (%s:%d): " #msg, __FILE_NAME__, __LINE__, __VA_ARGS__); \
    exit(1); \
}

typedef enum {
    false,
    true
} boolean;

typedef struct {
    unsigned int *jumps;
    size_t jumps_len;
    boolean expected_ouput;
} Test ;

boolean check_sled_jump(unsigned int *jumps, size_t jumps_len)
{
    assert((jumps_len >= 3), "constraint, the minimun amount of positions on the array is 3", NULL); 
    size_t max_peak_index = 0;
    size_t i = 1;
    while (i < jumps_len && jumps[i] > jumps[max_peak_index])
        max_peak_index = i++;
    while (i < jumps_len && jumps[i] < jumps[max_peak_index])
        i++;
    return i == jumps_len && max_peak_index != jumps_len-1;
}

int main()
{
    unsigned int jum_0[] = {1, 2, 3, 2, 1};
    unsigned int jum_1[] = {0, 1, 0};
    unsigned int jum_2[] = {0, 3, 2, 1};
    unsigned int jum_3[] = {0, 1000, 1};
    unsigned int jum_4[] = {2, 4, 4, 6, 2};
    unsigned int jum_5[] = {1, 2, 3};
    unsigned int jum_6[] = {1, 2, 3, 2, 1, 2, 3};
    Test tests[] = {
        { jum_0, size(jum_0), true },
        { jum_1, size(jum_1), true },
        { jum_2, size(jum_2), true },
        { jum_3, size(jum_3), true },
        { jum_4, size(jum_4), false },
        { jum_5, size(jum_5), false },
        { jum_6, size(jum_6), false },
    };
    size_t tests_len = sizeof(tests)/sizeof(Test);
    printf("[Analizando saltos]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        unsigned int result = check_sled_jump(tests[i].jumps, tests[i].jumps_len);
        printf("  - %-3s (%d)\n", result == tests[i].expected_ouput ? "ok" : "err", result);
    }
    return 0;
}
