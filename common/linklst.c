#include <stdlib.h>
#include "linklst.h"

void push(Linklst *l, void *ref)
{
    if (!*l) {
        Head* lh = *l = (Head*)malloc(sizeof(Linklst));
        lh->last = lh->first = (Node*)malloc(sizeof(Node));
        lh->first->ref = ref;
        lh->first->next = NULL;
        return;
    } 

    Head* lh = *l; 
    if (lh->last)
        lh->last = lh->last->next = (Node*)malloc(sizeof(Node));
    else 
        lh->last = (Node*)malloc(sizeof(Node));
    lh->last->ref = ref;
    lh->last->next = NULL;
}

void free_lst(Linklst *l)
{
    if (!*l) return;
    Head* lh = *l;
    if (lh) {
        Node *node = lh->first;
        Node *next_node;
        while (node) {
            next_node = node->next;
            if (node->ref)
                free(node->ref);
            free(node);
            node = next_node;
        }
    }
    free(*l);
    *l = NULL;
}

void nonfree_pop(Linklst *l)
{
    if (!*l || !(*l)->first){
        free_lst(l);
        return;
    }
    Head* lh = *l;
    Node* to_remove = lh->last; 
    if (lh->first == lh->last) {
        lh->first = lh->last = NULL;
        free_lst(l);
        return;
    } else { 
        Node* node = lh->first;
        while (node->next != lh->last)
            node = node->next;
        lh->last = node;
        lh->last->next = NULL;
    }
    if (to_remove->ref) 
        free(to_remove->ref);
    free(to_remove); 
}


int length(Linklst l)
{
    if (!l || !l->first) 
        return 0;
    Node* node = l->first;
    int count = 0;
    while (node) {
        count++;
        node = node->next;
    }
    return count;
}

void* last(Linklst l)
{
    return (!l || !l->first) ? NULL : l->last->ref;
}

void* prev(Linklst l, void* ref)
{
    if (!l || !l->first)
        return NULL;
    Node* node = l->first;
    while (node && node->next->ref != ref)
        node = node->next;
    return node ? node->ref : NULL;
}

Linklst map(Linklst l, void*(fn)(void*))
{
    Node *node = l->first;
    Linklst result = NULL;
    while (node) {
        push(&result, fn(node->ref));
        node = node->next;
    }
    return result;
}
