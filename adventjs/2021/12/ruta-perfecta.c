#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUTE_LEN 11
#define size(arr) sizeof(arr)/sizeof(arr[0])

typedef struct {
    unsigned int expected_jump;
    size_t obstacles_len;
    unsigned int *obstacles;
} Test;

void print_array(unsigned int *arr, size_t arr_len)
{
    printf("[ ");
    for (unsigned int i = 0; i < arr_len-1; i++)
        printf("%d, ", arr[i]);
    printf("%d]\n", arr[arr_len-1]);
}

int get_min_jump(unsigned int *obstacles, size_t obstacles_len)
{
    unsigned int route[ROUTE_LEN];
    memset(&route, 0, ROUTE_LEN*sizeof(unsigned int));
    for (unsigned int i = 0; i < obstacles_len; i++)
        route[obstacles[i]] = 1;
    size_t j = 0;
    size_t i = 0;
    do {
        j++;
        i = 0;
        while (i+j <= ROUTE_LEN && route[i] == 0)
           i+=j;
    } while (route[i] == 1);
    return j;

}

    int compare(const void* n1, const void* n2)
{
    return (*(unsigned int*)n1 - *(unsigned int*)n2);
}

void print_route(unsigned int *obstacles, size_t obstacles_len)
{
    qsort(obstacles, obstacles_len, sizeof(unsigned int), compare);

    for (unsigned int j = 0; j < ROUTE_LEN; j++)
        printf("%d, ", j);
    printf("%d", ROUTE_LEN);
    printf("\n            ");
    for (unsigned int j = 0; j < obstacles[0]; j++)
        printf(".  ");
    printf("X  ");

    for (size_t i = 1; i < obstacles_len; i++) {
        for (unsigned int j = obstacles[i-1]+1; j < obstacles[i]; j++)
            printf(".  ");
        printf("X  ");
    }
    for (unsigned int i = obstacles[obstacles_len-1]; i < ROUTE_LEN; i++)
        printf(".  ");
    printf("\n\n");
}

int main() {
    unsigned int ob1[] = {5, 3, 6, 7, 9};
    unsigned int ob2[] = {2, 4, 6, 8, 10};
    unsigned int ob3[] = {1, 2, 3, 5};
    unsigned int ob4[] = {3, 7, 5};
    unsigned int ob5[] = {9, 5, 1};
    Test tests[] = {
        {4, size(ob1), ob1},
        {7, size(ob2), ob2},
        {4, size(ob3), ob3},
        {2, size(ob4), ob4},
        {2, size(ob5), ob5},
    };
    size_t tests_len = size(tests);
    for (size_t i = 0; i < tests_len; i++) {
        unsigned int jump = get_min_jump(tests[i].obstacles, tests[i].obstacles_len);
        printf(
            "  - %-3s (%d) ",
            jump == tests[i].expected_jump ? "ok" : "err",
            jump
        );
        print_route(tests[i].obstacles, tests[i].obstacles_len);
    }
    return 0;
}
