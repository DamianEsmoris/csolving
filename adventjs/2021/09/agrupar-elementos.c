#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../../../common/hashmap.h"

#define to_lst(lst, arr) do {\
    size_t arr_len___LINE__ = sizeof(arr)/sizeof(arr[0]); \
    for (size_t i = 0; i < arr_len___LINE__; i++)\
        push(lst, (void*)(arr+i));\
} while (0)\

#define set_fields(map, val, key) \
    for (size_t i = 0; i < sizeof(val)/sizeof(val[0]); i++)\
        set((map + i), key, (val + i))

typedef enum {
    INT,
    DOUBLE,
    STRING,
    HASH_MAP,
    DATE
} Type;


typedef struct {
    int day;
    int month;
    int year;
} Date;

char* int_to_string(int num) {
    int num_digits = 0;
    int temp = abs(num);
    if (num < 0) {
        num_digits++;
    }
    while (temp > 0) {
        temp /= 10;
        num_digits++;
    }
    char* str = (char*)malloc((num_digits + 1) * sizeof(char));
    int i = num_digits - 1;  
    if (num < 0) {
        str[0] = '-';  
        num = abs(num);
        i = num_digits -1;
    }
    str[i+1] = 0;
    while (num > 0) {
        str[i] = (num % 10) + '0';
        num /= 10;
        i--;
    }
    return str;
}

char* value_to_key(Type type, void* value)
{
    switch (type) {
        case INT:   
            return int_to_string(*(int*)value);
        default:   
            printf("\nNOT IMPLEMENTED %d\n", __LINE__);
            exit(1);
        break;
    }
    return NULL;
}

void print_type(Type type, void* value)
{
    switch (type) {
        case INT:   
            printf("%d", *(int*)value);
        break;
        case DOUBLE:
            printf("%.2f", *(double*)value);
        break;
        case STRING:
            printf("'%s'", *(char**)value);
        break;
        case DATE:
            Date *date = (Date*)value;
            printf("%d-%02d-%02d", date->day, date->month, date->year);
        break;
        case HASH_MAP:
            HashMap hmap = *(HashMap*)value;
            Linklst hmap_keys = (Linklst)keys(hmap);
            Node *node = hmap_keys->first;
            printf("{ ");
            while (node->next) {
                char *key = (char*)node->ref;
                printf("'%s': (...), ", key);
                node = node->next;
            }
            char* key = (char*)node->ref;
            printf("'%s': (...) }", key);
            break;
        default:   
            printf("\nNOT IMPLEMENTED %d\n", __LINE__);
            exit(1);
        break;
    }
}

void print_inner_list(char *key, Linklst list, Type type) {
    printf("    '%s': [", key);
    Node *node = list->first;
    while (node->next) {
        print_type(type, node->ref);
        printf(", ");
        node = node->next;
    }
    print_type(type, node->ref);
    printf("]\n");

}

void group(HashMap hmap, Type type)
{
    printf("{\n");
    Linklst hmap_keys = (Linklst)keys(hmap);
    Node *node = hmap_keys->first;
    while (node) {
        char* key = (char*)node->ref;
        Linklst inner_list = (Linklst)get(hmap, key);
        if (inner_list)
            print_inner_list(key, inner_list, type);
        node = node->next;
    }
    printf("}\n");
}

void group_by(Linklst l, void*(*fn)(void*), Type types[2])
{
    Node *node = l->first;
    size_t l_len = length(l);
    int results[l_len];
    HashMap hmap = NULL;
    while (node) {
        void *n = (int*)fn(node->ref);
        char *key = value_to_key(types[0], n);
        Linklst xs = get(hmap, key);
        if (xs) {
            push(&xs, node->ref);
        } else {
            push(&xs, node->ref);
            set(&hmap, key, xs);
        }
        node = node->next;
    }  
    group(hmap, types[1]);
}

void* _get_date_year(void* ref_date)
{
    Date *date = (void*)ref_date;
    int *year = malloc(sizeof(int));
    *year = date->year;
    return (void*)year;
}

void* _to_date(void* ref_miliseconds)
{
    long milliseconds = *(unsigned long long*)ref_miliseconds;
    time_t seconds = (time_t)(milliseconds/1000);
    struct tm *tm = localtime(&seconds);
    Date *d = malloc(sizeof(Date));
    d->year = tm->tm_year + 1900;
    d->month = tm->tm_mon + 1;
    d->day = tm->tm_mday;
    return d;
}


void group_by_field(Linklst l, char* field, Type types[2])
{
    HashMap hmap = NULL;
    Node *node = l->first;
    while (node) {
        HashMap map = *(HashMap*)node->ref;
        void* value_ref;
        if (map && (value_ref = get(map, field))) {
            char *key = value_to_key(types[0], value_ref);
            Linklst xs = get(hmap, key);
            int linklst_undefined = xs == NULL;
            push(&xs, node->ref);
            if (linklst_undefined)
                set(&hmap, key, xs);
        } 
        node = node->next;
    }
    group(hmap, types[1]);
}


void* _floor(void *n) {
    int *num = malloc(sizeof(int));
    *num = floor(*(double*)n);
    return (void*)num;
}

void* _length(void *c) {
    char* str = *(char**)c;
    int* count = malloc(sizeof(int));
    *count = 0;
    while (str[++(*count)] != 0);
    return (void*)count;
}

int main()
{

    void* (*p_floor)(void*);
    p_floor = &_floor;

    void* (*p_length)(void*);
    p_length = &_length;

    void* (*p_to_date)(void*);
    p_to_date = &_to_date;

    void* (*p_get_date_year)(void*);
    p_get_date_year = &_get_date_year;

    {
        Linklst values = NULL;
        double values_sub[] = {6.1, 4.2, 6.3};
        to_lst(&values, values_sub);
        Type types[2] = { INT, DOUBLE };
        group_by(values, p_floor, types);
        printf("\n");
    }
    {
        Linklst values = NULL;
        char* values_sub[] = {"one", "two", "three"};
        to_lst(&values, values_sub);
        Type types[2] = { INT, STRING };
        group_by(values, p_length, types);
        printf("\n");
    }
    {
        Linklst values = NULL;
        HashMap ages_map[] = {NULL, NULL, NULL};
        int ages[] = { 23, 24 };
        set_fields(ages_map, ages, "age");
        printf("\n");
        to_lst(&values, ages_map);
        Type types[2] = { INT, HASH_MAP };
        group_by_field(values, "age", types);
        printf("\n");
    }
    {
        Linklst values = NULL;
        unsigned long long values_sub[] = {1397639141184, 1363223700000};
        to_lst(&values, values_sub);
        Type types[2] = { INT, DATE };
        group_by(map(values, p_to_date), p_get_date_year, types);
        free(values);
        printf("\n");
    }
    {
        Linklst values = NULL;
        HashMap books_map[] = {NULL, NULL, NULL, NULL};
        int ratings[] = { 8, 10, 9, 9 };
        char* titles[] = {
            "JavaScript: The Good Parts",
            "Aprendiendo Git",
            "Clean Code",
            "Learning Rust"
        };
        set_fields(books_map, titles, "title");
        set_fields(books_map, ratings, "rating");
        to_lst(&values, books_map);
        Type types[2] = { INT, HASH_MAP };
        group_by_field(values, "rating", types);
        printf("\n");
    }
    return 0;
}
