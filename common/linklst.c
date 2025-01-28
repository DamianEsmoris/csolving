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
    lh->last = lh->last->next = (Node*)malloc(sizeof(Node));
    lh->last->ref = ref;
    lh->last->next = NULL;
}

int length(Linklst l)
{
    Node* node = l->first;
    int count = 0;
    while (node) {
        count++;
        node = node->next;
    }
    return count;
}
