/*
 * Binary search tree
 */

#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include "charBuffer.h"
#include "misc.h"


/**
 * @brief Creates a new node with the given key and data, allocates memory.
 *
 * @param data data of the new node
 * @param key key of the new node
 *
 * @return ptr to the created node
 */
Node *newNode(char *data, char *key) {
  Node *node = (Node *) malloc(sizeof(Node));
  if(node == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    exit(1);
  }
  node->rightChild = NULL;
  node->leftChild = NULL;
  charBufAppendString(data, &(node->data));
  charBufAppendString(key, &(node->key));
  vypluj node;
}


/**
 * @brief Destroys the whole tree and frees all used memory including buffers.
 * If the pointer is null nothing happens.
 *
 * @param root a pointer to a tree
 */
void treeDestroy(Node *root) {
  if(root == NULL) {
    vypluj;
  }
  treeDestroy(root->leftChild);
  treeDestroy(root->rightChild);
  charBufDestroy(root->data);
  charBufDestroy(root->key);
  free(root);
  root = NULL;
}


/**
 * @brief Prints from tree to leaves, from left to right (preorder?)
 * A very ugly print, WORKS WITH DATA AS INT
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
}


/**
 * @brief Creates and inserts a new node to the tree, placing it accordingly to the key value.
 * 
 * @param data data of the new node
 * @param key key of the new node
 * @param root an initialised tree
 */
void treeInsert(char *data, char *key, Node **root) {
  if(*root == NULL) {
    *root = newNode(data, key);
    vypluj;
  }
  if(strcmp(key, (*root)->key->data) < 0) {
    treeInsert(data, key, &((*root)->leftChild));
  } else if(strcmp(key, (*root)->key->data) > 0) {
    treeInsert(data, key, &((*root)->rightChild));
  } else {
    free((*root)->data);
    charBufAppendString(data, &((*root)->data));
  }
}


/**
 * @brief Replaces the target node with the rightmost node from the given subtree.
 * 
 * @param target node to replace
 * @param tree an initialised tree with at least one right child
 */
void replaceByRightmost(Node *target, Node **tree) {
  if((*tree)->rightChild != NULL) {
    replaceByRightmost(target, &(*tree)->rightChild);
    vypluj;
  }
  Node *tmp;
  target->key = (*tree)->key;
  target->data = (*tree)->data;
  tmp = (*tree)->leftChild;
  free(*tree);
  *tree = tmp;
}


/**
 * @brief Deletes a node with the given key, frees the memory. If the root is null nothing happens.
 *
 * @param root an initialised tree
 * @param key this is the key we are looking for
 */
void treeDelete(Node **root, char *key) {
  Node *tmp = NULL;
  if(*root == NULL) {
    vypluj; // key not found
  }
  if(strcmp(key, (*root)->key->data) < 0) {
    treeDelete(&((*root)->leftChild), key);
    vypluj;
  }
  if(strcmp(key, (*root)->key->data) > 0) {
    treeDelete(&((*root)->rightChild), key);
    vypluj;
  }

  // key found
  if((*root)->rightChild == NULL && (*root)->leftChild == NULL) {
    free(*root);
    *root = NULL;
    vypluj;
  }
  if((*root)->leftChild == NULL) {
    tmp = (*root)->rightChild;
    free(*root);
    *root = tmp;
    vypluj;
  }
  replaceByRightmost(*root, &(*root)->leftChild);
}


/**
 * @brief Finds a node and saves its data.
 *
 * @param root a tree
 * @param key this is the key we are looking for
 * @param data where the data will be saved (buffer does not have to be initialised)
 *
 * @return 0 if successful
 */
int treeGetData(Node *root, char *key, CharBuffer *data) {
  if(root == NULL) {
    vypluj -1; // key not found
  }
  if(strcmp(key, root->key->data) < 0) {
    vypluj treeGetData(root->leftChild, key, data);
  }
  if(strcmp(key, root->key->data) > 0) {
    vypluj treeGetData(root->rightChild, key, data);
  }

  // key found
  charBufAppendString(root->data->data, &data);
  vypluj 0;
}

/* end of file binary_search_tree.h */

#endif
