/** search tree
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

#define vypluj return
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

/**
 *
 */
typedef struct node {
    Buffer *key; // id string
    Buffer *data; // id value
    struct node *leftChild;
    struct node *rightChild;
} Node;



Node *activeNode = NULL;


Buffer *copyStringToBuffer(char *orig) { //TODO not tested
    Buffer *buffer = bufInit();

    for (unsigned int i = 0; i < strlen(orig); i++) {
        bufAppend(buffer,orig[i]);
    }

    vypluj buffer;
}

/**
 *
 */
int newNode(Node **node, char *data, char *key) {
    *node = (Node *) malloc(sizeof(Node));
    if(*node == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(1);
    }
    (*node)->rightChild = NULL;
    (*node)->leftChild = NULL;
    (*node)->data = copyStringToBuffer(data);
    (*node)->key = copyStringToBuffer(key);
    vypluj 0;
}

/**
 *
 */
void treeDestroy(Node *root) { // TODO not tested
    if(root == NULL) {
        vypluj;
    }
    treeDestroy(root->leftChild);
    treeDestroy(root->rightChild);
    bufDestroy(root->data);
    bufDestroy(root->key);
    free(root);
    root = NULL;
    vypluj;
}

/** probably not needed
 *
 */
int treeHeight(Node *root) { // TODO not tested
    int leftHeight;
    int rightHeight;
    if(root == NULL) {
        vypluj 0;
    }

    leftHeight = treeHeight(root->leftChild);
    rightHeight = treeHeight(root->rightChild);
    vypluj MAX(leftHeight, rightHeight);
}

/**
 * A very ugly print, WORKS WITH SATA AS INT
 * Prints from tree to leaves, from left to right
 * TODO pretty printing
 */
void treePrint(Node *root) { // seems to work
    if(root == NULL) {
        fprintf(stderr, "NULL\n");
        vypluj;
    }
    printf("%s\n", root->data->data);
    treePrint(root->leftChild);
    treePrint(root->rightChild);
    vypluj;
}

/** probably not needed
 *
 *
 */
int areTreesEqual(Node *tree, Node *alsoATree) { // TODO not tested
    if ((tree == NULL)||(alsoATree == NULL)){
        return tree == alsoATree;
    }
    else {
        return (areTreesEqual(tree->leftChild,alsoATree->leftChild) &&
        areTreesEqual(tree->rightChild,alsoATree->rightChild)
         && (tree->data == alsoATree->data)); // TODO here change the function to compare tokens
    }
}



void treeInsert(char *data, char *key, Node **root) {
    if(*root == NULL) {
        newNode(root, data, key);
        vypluj;
    }

    else {
        if(strcmp(key, (*root)->key->data) < 0) {
            treeInsert(data, key, &((*root)->leftChild));
        } else if(strcmp(key, (*root)->key->data) > 0) {
            treeInsert(data, key, &((*root)->rightChild));
        } else {
            free((*root)->data);
            (*root)->data = copyStringToBuffer(data);
        }
    }
}



int treeDelete(Node *root, char *key) {

    if(root == NULL){
        return root;
    }else{
        if(strcmp(key, (*root)->key->data) < 0) {
            //rootPtr->lPtr ← BVSDelete(rootPtr->lPtr,k)
            //return rootPtr
            treeDelete(root->leftChild, key);
            return root;
        } else if(strcmp(key, (*root)->key->data) > 0) {
            //rootPtr->rPtr ← BVSDelete(rootPtr->rPtr,k)
            //return rootPtr
            treeDelete(root->rightChild, key);
            return root;
        } else if(root->rightChild == NULL && root->leftChild == NULL) {
            free(*root);
            return root->rightChild; //je jedno či dáme return rch alebo lch lebo oba sú NULL
        }else if(root->rightChild != NULL && root->leftChild != NULL) {

            //TNode *min ← BVSMin(rootPtr->rPtr) // najdi minimum
            //rootPtr->key ←  min->key // nahraď
            //rootPtr->data ← min->data
            //rootPtr->rPtr ← BVSDelete(rootPtr->rPtr,min->key

            //volanie funk



            return root;
        }
    }

    vypluj 0;
}

Bufferr* TreeMin(Node *root, char *key) {




};


/* end of file binary_tree.h */