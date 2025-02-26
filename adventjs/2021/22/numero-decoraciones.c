#include <stdio.h>
#include <stdlib.h>

struct BinTree {
    unsigned int value;
    struct BinTree *left;
    struct BinTree *right;
};

typedef struct BinTree * BinTree ;

#define B_BRANCH(val, lef, rig) ({ \
    BinTree btree = (BinTree)malloc(sizeof(struct BinTree)); \
    btree->value = val; \
    btree->left = lef; \
    btree->right = rig; \
    btree; \
})


unsigned int count_decorations(BinTree btree) {
    return btree
        ? btree->value + count_decorations(btree->left) + count_decorations(btree->right)
        : 0;
}

int main()
{
    {
        BinTree chrismas_tree = B_BRANCH(
            1,
            B_BRANCH(2, NULL, NULL),
            B_BRANCH(3, NULL, NULL)
        );
        printf("mini tree decorations: %d\n", count_decorations(chrismas_tree));
    }
    {
        BinTree chrismas_tree = B_BRANCH(
            1,
            B_BRANCH(5, B_BRANCH(7, B_BRANCH(3, NULL, NULL), NULL), NULL),
            B_BRANCH(6, B_BRANCH(5, NULL, NULL), B_BRANCH(1, NULL, NULL))
        );
        printf("big tree decorations: %d\n", count_decorations(chrismas_tree));
    }
    return 0;
}
