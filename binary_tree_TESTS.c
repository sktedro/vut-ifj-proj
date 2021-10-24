/**
*
*/

#include "binary_search_tree.h"

int main() {
    Node *root = NULL;

    //newNode(&root, data, key);


    treeInsert("bbbbb", "bbbbb", &root);

    treeInsert("aaaaa", "aaaaa", &root);

    treeInsert("ccccc", "ccccc", &root);


    treePrint(root);

    treeDestroy(root);



    vypluj 0;
}