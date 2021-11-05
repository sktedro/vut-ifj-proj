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
 * Replaces the target node with the rightmost node from the given subtree.
 * @param target node to replace
 * @param tree meh
 */
void replaceByRightmost(Node *target, Node **tree) {
    if((*tree)->rightChild != NULL) {
        replaceByRightmost(target, &(*tree)->rightChild);
    }
    else {
        Node *tmp;
        target->key = (*tree)->key;
        target->data = (*tree)->data;
        tmp = (*tree)->leftChild;
        free(*tree);
        *tree = tmp;
    }
}

/**
 * Deletes a node with the given key, frees the memory. If the root is null nothing happens.
 * @param root
 * @param key
 * @return
 */
void treeDelete(Node **root, char *key) {
    Node *tmp = NULL;
    if(root == NULL){
        vypluj;
    }
    if(((*root)->rightChild == NULL && (*root)->leftChild == NULL) && strcmp(key, (*root)->key->data) != 0) {
        vypluj;
    }
    if(strcmp(key, (*root)->key->data) < 0) {
        treeDelete(&((*root)->leftChild), key);
        vypluj;
    }
    if(strcmp(key, (*root)->key->data) > 0) {
        treeDelete(&((*root)->rightChild), key);
        vypluj;
    }

    if((*root)->rightChild == NULL && (*root)->leftChild == NULL) {
        free(*root);
        *root = NULL;
        vypluj;
    }
    else if((*root)->rightChild == NULL) {
        tmp = (*root)->leftChild;
        free(*root);
        *root = tmp;
    }
    else if((*root)->leftChild == NULL) {
        tmp = (*root)->rightChild;
        free(*root);
        *root = tmp;
    }
    else {
        replaceByRightmost(*root, &(*root)->leftChild);
    }
}

int treeGetData(Node *root, char *key, Buffer *data) {
    if(root == NULL) {
        vypluj -1;
    }
    if(strcmp(key, root->key->data) < 0) {
        vypluj treeGetData(root->leftChild, key, data);
    }
    else if(strcmp(key, root->key->data) > 0) {
        vypluj treeGetData(root->rightChild, key, data);
    }
    else {
        bufAppendString(root->data->data, &data);
        vypluj 0;
    }
}

/* end of file binary_tree.h */