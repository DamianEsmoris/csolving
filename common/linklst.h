typedef struct Node {
    void *ref;
    struct Node *next ;
} Node;

typedef struct {
    Node *last;
    Node *first;
} Head;

typedef Head * Linklst;

void free_lst(Linklst *l);
void push(Linklst *l, void *ref);
void nonfree_pop(Linklst *l);
int length(Linklst l);
void* last(Linklst l);
void* prev(Linklst l, void* ref);
