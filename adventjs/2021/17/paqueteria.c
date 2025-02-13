#include <stdio.h>
#include "../../../common/linklst.h"
#include "../../../common/str.h"

#define lst(l, arr) do {\
    size_t arr_len___LINE__ = sizeof(arr)/sizeof(arr[0]); \
    for (size_t i = 0; i < arr_len___LINE__; i++)\
        push(l, arr+i);\
} while (0)\

#define to_lst(l, ...) do {\
    void*  arr___LINE__[] = { __VA_ARGS__ }; \
    lst(l, arr___LINE__); \
} while (0)\


typedef struct {
    char name[64];
    unsigned int daily_load;
    Linklst subordinates;
} Carrier;


Carrier* search_carrier(Linklst lst, char* name)
{
    Node *node = lst->first;
    while (node && node->ref && !str_eq(((Carrier*)node->ref)->name, name))
        node = node->next;
    return node ? node->ref : node;
}

void print(Linklst l)
{
    if (!l) {
        printf("[]\n");
        return;
    }
    Node *node = l->first;
    printf("[");
    while (node->next) {
        printf("'%s', ", *(char**)node->ref);
        node = node->next;
    }
    printf("'%s'", *(char**)node->ref);
    printf("]\n");
}
unsigned int count_packages(Linklst lst, char* name)
{
    Carrier* inital_carrier = search_carrier(lst, name);
    if (!inital_carrier)
        return 0;
    unsigned int packages = inital_carrier->daily_load;
    if (inital_carrier->subordinates) {
        Node *node  = inital_carrier->subordinates->first;
        char* subordinate_name;
        while (node) {    
            subordinate_name = *(char**)node->ref;
            packages += count_packages(lst, subordinate_name);
            node = node->next;
        }
    }
    return packages;
}

int main()
{
    {
        Linklst carriers = NULL;
        Carrier carriers_alloc[] = {
            {"dapelu", 5, NULL},
            {"midu", 2, NULL},
            {"jelowing", 2, NULL}
        };
        to_lst(&carriers_alloc[0].subordinates, "midu", "jelowing");
        lst(&carriers, carriers_alloc);
        unsigned int result = count_packages(carriers, "dapelu");
        unsigned int expected = 9;
        if (result != expected)
            printf("  - err expected %d but got %d\n", expected, result); 
        else
            printf("  - ok  (%d)\n", result); 
    }
    {
        Linklst carriers = NULL;
        Carrier carriers_alloc[] = {
            {"lolivier", 8, NULL},
            {"camila", 5, NULL},
            {"jesuspoleo", 4, NULL},
            {"sergiomartinez", 4, NULL},
            {"conchaasensio", 3, NULL },
            {"facundocapua", 2, NULL},
            {"faviola", 1, NULL}
        };
        to_lst(&carriers_alloc[0].subordinates, "camila", "jesuspoleo");
        to_lst(&carriers_alloc[1].subordinates, "sergiomartinez", "conchaasensio");
        to_lst(&carriers_alloc[4].subordinates, "facundocapua", "faviola");
        lst(&carriers, carriers_alloc);
        unsigned int result = count_packages(carriers, "camila");
        unsigned int expected = 15;
        if (result != expected)
            printf("  - err expected %d but got %d\n", expected, result); 
        else
            printf("  - ok  (%d)\n", result); 
    }
    return 0;
}
