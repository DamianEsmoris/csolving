#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define load_step(stp, chr, len) do {\
    memset(stp, chr, len);\
    stp[len] = 0;\
} while (0)\

#define add_leafs_level(tre, lea, i, wid) do {\
    char step[lea];\
    load_step(step, '*', lea);\
    center_string(tre+i, step, lea, wid-1, '_');\
    tre[i-1] = '\n';\
    leafs+=2;\
} while(0)\

void center_string(char* dest, char* str, int str_size, int width, char padding_char)
{
    int padding = (width-str_size)/2;
    for (int i = 0; i < padding; i++)
        dest[i] = padding_char;
    for (size_t i = 0; i < str_size; i++)
        dest[i+padding] = str[i];
    for (int i = padding + str_size; i < width; i++)
        dest[i] = padding_char;
}

char* chrismas_tree(int height)
{
    int width = height*2;
    // the max amount of leafs is height*2-1 but
    // the breakline occupies one character!
    const int log_size = 1;
    const int logs = 2;
    int leafs = 1;

    int leafs_area = width*height;
    int tree_area = width*(height+logs);

    char* tree = malloc(tree_area*sizeof(char));
    tree[tree_area-1] = '\n';
    tree[tree_area] = 0;

    add_leafs_level(tree, leafs, 0, width);
    for (int i = width; i < leafs_area; i+=width) {
        add_leafs_level(tree, leafs, i, width);
        tree[i-1] = '\n';
    }


    {
        char step[log_size];
        load_step(step, '#', log_size);
        for (int i = leafs_area; i < tree_area; i+=width) {
            center_string(tree+i, step, log_size, width-1, '_');
            tree[i-1] = '\n';
        }
    }

    return tree;
}

int main()
{
    char* _10len_tree = chrismas_tree(10);
    printf("%s", _10len_tree);
    printf("\n");
    return 0;
}
