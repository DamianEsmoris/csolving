#include <stdio.h>
#include "../../../common/linklst.h"
#include "../../../common/str.h"

#define new_product(na) { PRODUCT, na }
#define print_result(st, pr) printf("  - (%d) '%s' %s en la almacen\n", contains(st, pr), pr, contains(st, pr) ? "esta" : "no esta");

typedef enum {
    CONTAINER,
    PRODUCT
} StructType;

typedef struct {
    StructType type;
    Linklst subContainers;
    char* name;
} Container;

typedef struct {
    StructType type;
    char * name; 
} Product;

int contains(Linklst l, char *product_name)
{
    if (!l)
        return 0;
    Node *node = l->first;
    while (node) {
        Container *cont = node->ref;
        if (cont->type == CONTAINER && contains(cont->subContainers, product_name)) {
            return 1;
        } else if (cont->type == PRODUCT) {
            Product *prod = (Product*)cont;
            if (str_eq(prod->name, product_name))
                return 1;
        }
        node = node->next;
    }
    return 0;
}

int main()
{
    {
    Linklst store = NULL;

    Container bookshelf_1 = { CONTAINER, NULL, "estanteria1" };
    Container drawer_1 = { CONTAINER, NULL, "cajon1" };
    push(&bookshelf_1.subContainers, (void*)&drawer_1);
    Product drawer_1_products[] = { new_product("coca-cola"), new_product("fanta"), new_product("sprite") };
    size_t drawer_1_products_len = sizeof(drawer_1_products)/sizeof(Product);
    for (size_t i = 0; i < drawer_1_products_len; i++)
        push(&drawer_1.subContainers, drawer_1_products+i);

    Container bookshelf_2 = { CONTAINER, NULL, "estanteria2" };
    Container drawer_2 = { CONTAINER, NULL, "cajon1" };
    Container drawer_3 = { CONTAINER, NULL, "cajon2" };
    Product drawer_3_products[] = { new_product("pantalones"), new_product("camiseta") };
    size_t drawer_3_products_len = sizeof(drawer_3_products)/sizeof(Product);
    for (size_t i = 0; i < drawer_3_products_len; i++)
        push(&drawer_3.subContainers, drawer_3_products+i);
    push(&bookshelf_2.subContainers, (void*)&drawer_2);
    push(&bookshelf_2.subContainers, (void*)&drawer_3);
    push(&store, (void*)&bookshelf_1);
    push(&store, (void*)&bookshelf_2);

    printf("[almacen]:\n");
    print_result(store, "fanta");
    print_result(store, "camiseta");
    }

    {
    Linklst store = NULL;

    Container chest_1 = { CONTAINER, NULL, "baul" };
    Container bottom_1 = { CONTAINER, NULL, "fondo" };
    Product bottom_1_products[] = { new_product("cd-rom"), new_product("disquette"), new_product("mando") };
    size_t bottom_1_products_len = sizeof(bottom_1_products)/sizeof(Product);
    for (size_t i = 0; i < bottom_1_products_len; i++)
        push(&bottom_1.subContainers, bottom_1_products+i);
    push(&chest_1.subContainers, (void*)&bottom_1);
    push(&store, (void*)&chest_1);

    printf("[otro almacen]:\n");
    print_result(store, "cd-rom");
    print_result(store, "disquette");
    print_result(store, "mando");
    print_result(store, "gameboy");
    }
    return 0;
}
