#include <stdio.h>
#define size(arr) sizeof(arr)/sizeof(arr[0])

typedef unsigned int Trip[3];

typedef enum {
    false,
    true
} boolean;

typedef struct {
    boolean expected_output;
    unsigned int capacity;
    const size_t trip_len;
    Trip *trip;
} Test;

Trip tri_01[] = {{2, 5, 8}, {3, 6, 10}};
Trip tri_02[] = {{1, 1, 5}, {2, 2, 10}};
Trip tri_03[] = {{2, 1, 5}, {3, 5, 7}};
Trip tri_04[] = {{2, 3, 8}, {2, 5, 7}};
Trip tri_05[] = {{2, 3, 8}};
Trip tri_06[] = {{1, 2, 4}, {2, 3, 8}};
Test tests[] = {
    { false, 4, size(tri_01), tri_01 },
    { true,  3, size(tri_02), tri_02 },
    { true,  3, size(tri_03), tri_03 },
    { true,  4, size(tri_04), tri_04 },
    { false, 1, size(tri_05), tri_05 },
    { false, 2, size(tri_06), tri_06 }
};
const size_t tests_len = size(tests);

boolean can_carry(unsigned int capacity, Trip *trip, size_t trip_len) {
    typedef enum{
        AMOUNT,
        ORIGIN,
        DESTINY
    } TripInfo;
    unsigned int current_amount = trip[0][AMOUNT];
    unsigned int *current_stop, *previous_stop;
    size_t i = 1;
    while (i < trip_len && current_amount <= capacity) {
        current_stop  = (unsigned int*)(trip+i);
        previous_stop = (unsigned int*)(trip+i-1);
        if (previous_stop[DESTINY] <= current_stop[ORIGIN]) 
            current_amount -= previous_stop[AMOUNT];
        current_amount += current_stop[AMOUNT];
        i++;
    }
    return current_amount <= capacity;
}

int main()
{
    printf("[Revisando viajes]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        boolean result = can_carry(tests[i].capacity, tests[i].trip, tests[i].trip_len);
        printf("  - %-2s\n", result == tests[i].expected_output ? "ok" : "err");
    }
    return 0;
}
