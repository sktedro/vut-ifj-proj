/*
 * Binary search tree
 */

#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include "intBuffer.h"
#include "misc.h"


/**
 * @brief Creates a new node with the given key and name, allocates memory.
 *
 * @param name name (and key) of the new node
 *
 * @return ptr to the created node
 */
STTreeNode *newSTTreeNode(char *name){
  STTreeNode *node = (STTreeNode *) malloc(sizeof(STTreeNode));
  if(node == NULL){
    //TODO memleak
    exit(err(99));
  }
  node->rightChild = NULL;
  node->leftChild = NULL;
  memcpy(&(node->key), name, strlen(name) + 1);
  node->data = malloc(sizeof(STElem));
  if(!node->data){
    //TODO memleak
    exit(err(99));
  }
  memcpy(&(node->data->name), name, strlen(name) + 1);
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
void treeInsert(STTreeNode **root, char *name){
  if(*root == NULL){
    *root = newSTTreeNode(name);
    vypluj;
  }
  if(strcmp(name, (*root)->key) < 0){
    treeInsert(&((*root)->leftChild), name);
  }else if(strcmp(name, (*root)->key) > 0){
    treeInsert(&((*root)->rightChild), name);
  }
  // No need for 'else' since this function only inserts a bare node with a key
  // and a name, nothing else
}

/**
 * @brief Frees all memory allocated by a tree node
 *
 * @param elem to be freed
 */
void treeElemDestroy(STElem *data){
  free(data->name);
  intBufDestroy(data->fnParamTypesBuf);
  intBufDestroy(data->fnRetTypesBuf);
}

/**
 * @brief Destroys the whole tree and frees all used memory.
 *
 * @param root a pointer to a tree
 */
void treeDestroy(STTreeNode *root){
  if(root == NULL){
    vypluj;
  }
  treeDestroy(root->leftChild);
  treeDestroy(root->rightChild);
  treeElemDestroy(root->data);
  free(root->key);
  free(root->data);
  free(root);
  root = NULL;
}


/**
 * @brief Prints from tree to leaves, from left to right (preorder?)
 * A very ugly print
 * TODO pretty printing
 *
 * @param root root to the tree to be printed
 */
void treePrint(STTreeNode *root){
  if(root == NULL){
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
  free(*tree);
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
  if(*root == NULL){
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
    vypluj;
  }
  if((*root)->leftChild == NULL){
    tmp = (*root)->rightChild;
    free(*root);
    *root = tmp;
    vypluj;
  }
  if((*root)->rightChild == NULL){
    tmp = (*root)->leftChild;
    free(*root);
    *root = tmp;
    vypluj;
  }
  replaceByRightmost(*root, &(*root)->leftChild);
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
  if(root == NULL){
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



// TODO each line is one function - but do we need all that?
// set isVariable? This seems to be useless
// set varDataType, varAddress
// set fnDefined
// append to fnParamBuffer
// append to fnRetBuffer

/*
 * @brief Use for STTreeNodes that represent variables - variable's data
 * type and it's address, also sets boolean 'isVariable' to true
 *
 * @param data - pointer to the data structure of a tree node
 * @param varDataType - data type to be written
 * @param varAddress - address to be written
 */
// Will this function ever be used? Will we ever find out data type at the same
// time as the address?
void treeSetVarData(STElem *data, int varDataType, int varAddress){
  if(data){
    data->isVariable = true;
    data->varDataType = varDataType;
    data->varAddress = varAddress;
  }
}

/*
 * @brief Use for STTreeNodes that represent functions - boolean 'isVariable' 
 * gets set to false and boolean 'fnDefined' is set based on the paramter
 *
 * @param data - pointer to the data structure of a tree node
 * @param fnDefined - boolean value to be written to STElem->fnDefined
 */
void treeSetFnData(STElem *data, bool fnDefined){
  if(data){
    data->isVariable = false;
    data->fnDefined = fnDefined;
  }
}

/*
 * @brief appends a data type of a paramter of a function
 *
 * @param data - pointer to the data structure of a tree node
 * @param paramType - data type of the parameter to be appended
 */
void treeAppendParamType(STElem *data, int paramType){
  if(data){
    if(!data->fnParamTypesBuf){
      data->fnParamTypesBuf = intBufInit();
    }
    intBufAppend(data->fnParamTypesBuf, paramType);
  }
}

/*
 * @brief appends a data type of a return value of a function
 *
 * @param data - pointer to the data structure of a tree node
 * @param paramType - data type of the parameter to be appended
 */
void treeAppendRetType(STElem *data, int paramType){
  if(data){
    if(!data->fnRetTypesBuf){
      data->fnRetTypesBuf = intBufInit();
    }
    intBufAppend(data->fnRetTypesBuf, paramType);
  }
}

//TODO functions to get the data somehow? Or not needed?

/* end of file binary_search_tree.h */

#endif
