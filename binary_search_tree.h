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
 * Node structure for the binary search tree.
 */
typedef struct node {
    Buffer *key; // id string
    Buffer *data; // id value
    struct node *leftChild;
    struct node *rightChild;
} Node;

/**
 * Selected node to work with.
 */
Node *activeNode = NULL;

/**
 * Creates a new node with the given key and data, allocates memory.
 *
 * @param data data of the new node
 * @param key key of the new node
 * @return
 */
Node *newNode(char *data, char *key) {
    Node *node = (Node *) malloc(sizeof(Node));
    if(node == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(1);
    }
    node->rightChild = NULL;
    node->leftChild = NULL;
    bufAppendString(data, &(node->data));
    bufAppendString(key, &(node->key));
    vypluj node;
}

/**
 * Destroys the whole tree and frees all used memory. If the pointer is null nothing happens.
 * @param root a pointer to a tree
 */
void treeDestroy(Node *root) {
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

/**
 * A very ugly print, WORKS WITH DATA AS INT
 * Prints from tree to leaves, from left to right (preorder?)
 * TODO pretty printing
 *
 * @param root root to the tree to be printed
 */
void treePrint(Node *root) {
    if(root == NULL) {
        fprintf(stderr, "NULL\n");
        vypluj;
    }
    printf("%s\n", root->data->data);
    treePrint(root->leftChild);
    treePrint(root->rightChild);
    vypluj;
}

/**
 * Creates and inserts a new noe to the tree, placing it according to the key value.
 * @param data data of the new node
 * @param key key of the new node
 * @param root pointer to an initialised tree
 */
void treeInsert(char *data, char *key, Node **root) {
    if(*root == NULL) {
        *root = newNode(data, key);
        vypluj;
    }

    else {
        if(strcmp(key, (*root)->key->data) < 0) {
            treeInsert(data, key, &((*root)->leftChild));
        } else if(strcmp(key, (*root)->key->data) > 0) {
            treeInsert(data, key, &((*root)->rightChild));
        } else {
            free((*root)->data);
            bufAppendString(data, &((*root)->data));
        }
    }
}

/**
 * Deletes a node with the given key, frees the memory. If the root is null nothing happens.
 * @param root
 * @param key
 * @return
 */
void treeDelete(Node **root, char *key) {
    if(root == NULL){
        vypluj;
    }

    if(strcmp(key, (*root)->key->data) < 0) {
        //rootPtr->lPtr ← BVSDelete(rootPtr->lPtr,k)
        //return rootPtr
        treeDelete(&((*root)->leftChild), key);
        vypluj;
    } else if(strcmp(key, (*root)->key->data) > 0) {
        //rootPtr->rPtr ← BVSDelete(rootPtr->rPtr,k)
        //return rootPtr
        treeDelete(&((*root)->rightChild), key);
        vypluj;
    } else if((*root)->rightChild == NULL && (*root)->leftChild == NULL) {
        free(*root);
        *root = NULL;
        vypluj;
    }else if((*root)->rightChild != NULL && (*root)->leftChild != NULL) { //TODO
            //TNode *min ← BVSMin(rootPtr->rPtr) // najdi minimum
        TreeMin(root, key);
        //rootPtr->key ←  min->key // nahraď
        //rootPtr->data ← min->data
        //rootPtr->rPtr ← BVSDelete(rootPtr->rPtr,min->key

            //volanie funk

        }
}


Bufferr* TreeMin(Node *root, char *key) {

    //if rootPtr->lPtr = NULL
    //then // další levý už neexistuje
    //return rootPtr
    //else // pokračujeme vlevo
    //return BVSMin(rootPtr->lPtr)
    
    if(root->leftChild == NULL){
        return  root; //dalsi levi uz neexistuje
    }

    TreeMin(root, key);
};


/* end of file binary_tree.h */