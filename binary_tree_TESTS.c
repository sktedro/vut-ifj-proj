/**
*
*/

#include "binary_search_tree.h"

int main() {
    Node *root = NULL;

    //newNode(&root, data, key);
    treeInsert("a", "0", &root);

    treeInsert("b", "1", &root);

    treeInsert("c", "2", &root);

    Buffer *buf = bufInit();
    int err = treeGetData(root, "2", buf);
    printf("key: 2  data: %s  err: %d\n", buf->data, err);

    buf = bufInit();
    err = treeGetData(root, "y", buf);
    printf("key: 56  data: %s  err: %d\n\n", buf->data, err);



    treePrint(root);
    printf("\n");
    treeDelete(&root, "bbbbb");
    treePrint(root);
    printf("\n");
    treeDelete(&root, "aaaaa");
    treePrint(root);
    printf("\n");
    treeDelete(&root, "esdrft");
    treePrint(root);
    printf("\n");

    treeDestroy(root);
    vypluj 0;
}