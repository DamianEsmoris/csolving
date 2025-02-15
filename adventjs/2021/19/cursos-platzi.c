#include <stdio.h>
#include <stdlib.h>

#define size(arr) sizeof(arr)/sizeof(arr[0])
typedef size_t CoursesPair[2];

typedef struct {
    unsigned int time;
    unsigned int* courses;
    size_t courses_len;
    unsigned int* expected_output;
} Test;

unsigned int cou_01[] = {2, 3, 8, 1, 4};
unsigned int cou_02[] = {2, 10, 4, 1};
unsigned int cou_03[] = {10, 15, 20, 5};
unsigned int cou_04[] = {8, 2, 1};
unsigned int cou_05[] = {8, 2, 1, 4, 3};
unsigned int cou_06[] = {10, 14, 20};
unsigned int cou_07[] = {5, 5, 5};
unsigned int out_01[] = {0, 2};
unsigned int out_02[] = {1, 2};
unsigned int out_03[] = {0, 1};
unsigned int out_04[] = {1, 2};
unsigned int out_05[] = {3, 4};
Test tests[] = {
    {10, cou_01, size(cou_01), out_01},
    {15, cou_02, size(cou_02), out_02},
    {25, cou_03, size(cou_03), out_03},
    { 8, cou_04, size(cou_04), out_04},
    { 8, cou_05, size(cou_05), out_05},
    { 4, cou_06, size(cou_06), NULL},
    { 5, cou_07, size(cou_07), NULL}
};
size_t tests_len = size(tests);

int is_pair_unset(CoursesPair p)
{
    return p[0] == 0 && p[1] == 0;
}

unsigned int* learn(unsigned int time, unsigned int *courses, size_t courses_len)
{
    unsigned int* courses_pair = malloc(sizeof(unsigned int) * 2);
    CoursesPair times_sum[time];
    for (unsigned int i = 0; i < time; i++) {
        times_sum[i][0] = 0;
        times_sum[i][1] = 0;
    }
    unsigned int total_time;
    size_t i, j;
    i = -1;
    do {
        i++;
        j = i+1;
        total_time = courses[i]+courses[j];
        while (j < courses_len && (i == j || courses[i]+courses[j] != time))  {
            total_time = courses[i]+courses[j];
            if (i != j && total_time < time && is_pair_unset(times_sum[total_time])) {
                times_sum[total_time][0] = (size_t)i;
                times_sum[total_time][1] = (size_t)j;
            }
            j++;
        }
    } while (
        i < courses_len && 
        ((i == j || courses[i]+courses[j] != time)) || j == courses_len
    );
    if (i < courses_len && j < courses_len) {
        courses_pair[0] = i;
        courses_pair[1] = j;
        return courses_pair;
    };
    i = time-1;
    while (i > 0 && is_pair_unset(times_sum[i]))
        i--;
    if (i == 0) {
        free(courses_pair);
        return NULL;
    }
    courses_pair[0] = times_sum[i][0];
    courses_pair[1] = times_sum[i][1];
    return courses_pair;
    return NULL;

}

int is_pair_eq(unsigned int* p1, unsigned int* p2)
{
    if ((p1 == NULL) != (p2 == NULL))
        return 0;
    else if ((p1 == NULL) == (p2 == NULL))
        return 1;
    size_t i = 0;
    while (i < 2 && p1[i] == p2[i])
        i++;
    return i == 2;
}

void print_array(unsigned int* arr, size_t arr_len)
{
    if (!arr || arr_len == 0) {
        printf("[]\n");
        return;
    }
    printf("[");
    for (size_t i = 0; i < arr_len-1; i++)
        printf("%d, ", arr[i]);
    printf("%d]\n", arr[arr_len-1]);
}

int main()
{

    printf("[Buscando el mejor par de cursos según las horas disponibles]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        unsigned int *courses_pair = learn(tests[i].time, tests[i].courses, tests[i].courses_len);
        if (is_pair_eq(courses_pair, tests[i].expected_output)) {
            printf("  - ok ");
            print_array(courses_pair, 2);
        } else {
            printf("  - err \n      expected: ");
            print_array(tests[i].expected_output, 2);
            printf("      result: ");
            print_array(courses_pair, 2);
        }
    }
    return 0;
}
