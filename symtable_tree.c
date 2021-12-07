/*
 * Binary search tree
 */

#ifndef SYMTABLE_TREE_C
#define SYMTABLE_TREE_C

#include "symtable_tree.h"

extern int ret;

/**
 * @brief Creates a new node with the given key and key, allocates memory.
 *
 * @param node: destination pointer
 * @param key key of the new node
 * @param frame: number of the frame in which the variable is
 *
 * @return 0 if successful, errcode otherwise
 */
int newSTTreeNode(STTreeNode **node, char *key, int frame) {
  GCMalloc(*node, sizeof(STTreeNode));

  // Init children
  (*node)->rightChild = NULL;
  (*node)->leftChild = NULL;

  // Copy the key
  GCMalloc((*node)->key, (strlen(key) + 1) * sizeof(char));
  memcpy((*node)->key, key, (strlen(key) + 1) * sizeof(char));

  // Init data
  GCMalloc((*node)->data, sizeof(STElem));

  // Set the name (generate a name for the ifjcode21 variable)
  (*node)->data->name = genVarName(key, frame);

  // Init other data
  (*node)->data->isVariable = true;
  (*node)->data->varDataType = -1;
  (*node)->data->varAddress = -1;
  (*node)->data->fnDefined = false;
  (*node)->data->fnDefined = false;
  (*node)->data->fnParamTypesBuf = NULL;
  (*node)->data->fnParamNamesBuf = NULL;
  (*node)->data->fnRetTypesBuf = NULL;
  TryCall(intBufInit, &((*node)->data->fnParamTypesBuf));
  TryCall(stringBufInit, &((*node)->data->fnParamNamesBuf));
  TryCall(intBufInit, &((*node)->data->fnRetTypesBuf));
  vypluj 0;
}

/**
 * @brief Creates and inserts a new node to the tree, placing it accordingly to
 * the key value.
 * 
 * @param root node of the tree
 * @param name name (and key) of the new node
 * @param frame: number of the frame in which the variable is
 *
 * @return 0 if successful, errcode otherwise
 */
int treeInsert(STTreeNode **root, char *key, int frame) {
  if (!(*root)) {
    TryCall(newSTTreeNode, root, key, frame);
    vypluj 0;
  }
  if (strcmp(key, (*root)->key) < 0) {
    TryCall(treeInsert, &((*root)->leftChild), key, frame);
  } else if (strcmp(key, (*root)->key) > 0) {
    TryCall(treeInsert, &((*root)->rightChild), key, frame);
  } else {
    return ERR(ID_DEF_ERR);
    fprintf(stderr, "Inserting a key into a binary tree that is already there.\n");
  }
  vypluj 0;
}

/**
 * @brief Replaces the target node with the rightmost node from the given 
 * subtree.
 * 
 * @param target node to replace
 * @param tree an initialised tree with at least one right child
 */
void replaceByRightmost(STTreeNode *target, STTreeNode **tree) {
  // TODO memory leaks? target's key and data are simply overwritten
  if ((*tree)->rightChild != NULL) {
    replaceByRightmost(target, &(*tree)->rightChild);
    vypluj;
  }
  STTreeNode *tmp;
  target->key = (*tree)->key;
  target->data = (*tree)->data;
  tmp = (*tree)->leftChild;
  *tree = tmp;
}

/**
 * @brief Deletes a node with the given key, frees the memory. If the root is 
 * null nothing happens.
 *
 * @param root node of the tree
 * @param key to delete
 */
void treeDelete(STTreeNode **root, char *key) {
  STTreeNode *tmp = NULL;
  if (!(*root)) {
    vypluj; // key not found
  }
  if (strcmp(key, (*root)->key) < 0) {
    treeDelete(&((*root)->leftChild), key);
    vypluj;
  }
  if (strcmp(key, (*root)->key) > 0) {
    treeDelete(&((*root)->rightChild), key);
    vypluj;
  }

  // key found
  if ((*root)->rightChild == NULL && (*root)->leftChild == NULL) {
    *root = NULL;
  } else if ((*root)->leftChild == NULL) {
    tmp = (*root)->rightChild;
    *root = tmp;
  } else if ((*root)->rightChild == NULL) {
    tmp = (*root)->leftChild;
    *root = tmp;
    vypluj;
  } else {
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
STElem *treeGetData(STTreeNode *root, char *key) {
  if (!root) {
    vypluj NULL; // key not found
  }
  if (strcmp(key, root->key) < 0) {
    vypluj treeGetData(root->leftChild, key);
  }
  if (strcmp(key, root->key) > 0) {
    vypluj treeGetData(root->rightChild, key);
  }

  // key found
  return root->data;
}

#endif
/* end of file symtable_tree.c */
