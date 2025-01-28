typedef struct Node {
    void *ref;
    struct Node *next ;
} Node;

typedef struct {
    Node *last;
    Node *first;
} Head;

typedef Head * Linklst;

void push(Linklst *l, void *ref);
int length(Linklst l);
