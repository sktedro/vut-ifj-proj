/*
 * Binary search tree
 */

#ifndef SYMTABLE_TREE_C
#define SYMTABLE_TREE_C

#include "symtable_tree.h"


/**
 * @brief Creates a new node with the given key and key, allocates memory.
 *
 * @param key key of the new node
 *
 * @return ptr to the created node
 */
STTreeNode *newSTTreeNode(char *key){
  STTreeNode *node = (STTreeNode *) malloc(sizeof(STTreeNode));
  if(!node){
    //TODO memleak
    exit(err(INTERN_ERR));
  }

  // Init children
  node->rightChild = NULL;
  node->leftChild = NULL;

  // Copy the key
  node->key = malloc((strlen(key) + 1) * sizeof(char));
  if(!node->key){
    //TODO memleak
    exit(err(INTERN_ERR));
  }
  memcpy(node->key, key, (strlen(key) + 1) * sizeof(char));

  // Init data
  node->data = malloc(sizeof(STElem));
  if(!node->data){
    //TODO memleak
    exit(err(INTERN_ERR));
  }

  // Copy key
  node->data->name = malloc((strlen(key) + 1) * sizeof(char));
  if(!node->data->name){
    //TODO memleak
    exit(err(INTERN_ERR));
  }
  memcpy(node->data->name, key, (strlen(key) + 1) * sizeof(char));

  // Init other data
  node->data->isVariable = true;
  node->data->varDataType = -1;
  node->data->varAddress = -1;
  node->data->fnDefined = false;
  node->data->fnParamTypesBuf = NULL;
  node->data->fnRetTypesBuf = NULL;
  vypluj node;
}


/**
 * @brief Creates and inserts a new node to the tree, placing it accordingly to
 * the key value.
 * 
 * @param root node of the tree
 * @param name name (and key) of the new node
 */
void treeInsert(STTreeNode **root, char *key){
  if(!(*root)){
    *root = newSTTreeNode(key);
    vypluj;
  }
  if(strcmp(key, (*root)->key) < 0) {
    treeInsert(&((*root)->leftChild), key);
  }
  else if(strcmp(key, (*root)->key) > 0) {
    treeInsert(&((*root)->rightChild), key);
  }
  else { // found the key
    fprintf(stderr, "Warning: inserting a key that is already there. Nothing will be changed.\n");
  }
  // and a name, nothing else
}


/**
 * @brief Frees all memory allocated by a tree node
 *
 * @param elem to be freed
 */
void treeElemDestroy(STElem *data){
  if(data){
    free(data->name);
    intBufDestroy(data->fnParamTypesBuf);
    intBufDestroy(data->fnRetTypesBuf);
    free(data);
  }
}

/**
 * @brief Destroys the whole tree and frees all used memory.
 *
 * @param root a pointer to a tree
 */
void treeDestroy(STTreeNode **root){
  if(!root || !(*root)){
    vypluj;
  }
  treeDestroy(&((*root)->leftChild));
  treeDestroy(&((*root)->rightChild));
  treeElemDestroy((*root)->data);
  free((*root)->key);
  free((*root));
  *root = NULL;
}


/**
 * @brief Prints from tree to leaves, from left to right (preorder?)
 *
 * @param root root to the tree to be printed
 */
void treePrint(STTreeNode *root){
  if(!root){
    vypluj;
  }
  printf("%s\n", root->key);
  treePrint(root->leftChild);
  treePrint(root->rightChild);
}


/**
 * @brief Replaces the target node with the rightmost node from the given 
 * subtree.
 * 
 * @param target node to replace
 * @param tree an initialised tree with at least one right child
 */
void replaceByRightmost(STTreeNode *target, STTreeNode **tree){
  // TODO memory leaks? target's key and data are simply overwritten
  if((*tree)->rightChild != NULL){
    replaceByRightmost(target, &(*tree)->rightChild);
    vypluj;
  }
  STTreeNode *tmp;
  target->key = (*tree)->key;
  target->data = (*tree)->data;
  tmp = (*tree)->leftChild;
  free(*tree); // free element function????? TODO
  *tree = tmp;
}


/**
 * @brief Deletes a node with the given key, frees the memory. If the root is null nothing happens.
 *
 * @param root node of the tree
 * @param key to delete
 */
void treeDelete(STTreeNode **root, char *key){
  STTreeNode *tmp = NULL;
  if(!(*root)){
    vypluj; // key not found
  }
  if(strcmp(key, (*root)->key) < 0){
    treeDelete(&((*root)->leftChild), key);
    vypluj;
  }
  if(strcmp(key, (*root)->key) > 0){
    treeDelete(&((*root)->rightChild), key);
    vypluj;
  }

  // key found
  if((*root)->rightChild == NULL && (*root)->leftChild == NULL){
    free(*root);
    *root = NULL;
  }
  else if((*root)->leftChild == NULL){
    tmp = (*root)->rightChild;
    free(*root);
    *root = tmp;
  }
  else if((*root)->rightChild == NULL){
    tmp = (*root)->leftChild;
    free(*root);
    *root = tmp;
    vypluj;
  }
  else {
    replaceByRightmost(*root, &(*root)->leftChild);
  }
}


/**
 * @brief Finds a node and returns a pointer to all its data
 *
 * @param root of a tree
 * @param key this is the key we are looking for
 * @param data where the data will be saved (buffer does not have to be initialised)
 *
 * @return pointer to symbol table tree element
 */
STElem *treeGetData(STTreeNode *root, char *key){
  if(!root){
    vypluj NULL; // key not found
  }
  if(strcmp(key, root->key) < 0){
    vypluj treeGetData(root->leftChild, key);
  }
  if(strcmp(key, root->key) > 0){
    vypluj treeGetData(root->rightChild, key);
  }

  // key found
  return root->data;
}


#endif
/* end of file symtable_tree.c */
