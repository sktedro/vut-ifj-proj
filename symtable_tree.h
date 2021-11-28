/*
 * Binary search tree
 */

#ifndef SYMTABLE_TREE
#define SYMTABLE_TREE

#include "int_buffer.h"
#include "misc.h"

/**
 * @brief Creates a new node with the given key and key, allocates memory.
 *
 * @param node: destination pointer
 * @param key key of the new node
 *
 * @return 0 if successful, errcode otherwise
 */
int newSTTreeNode(STTreeNode **node, char *key) ForceRetUse;

/**
 * @brief Creates and inserts a new node to the tree, placing it accordingly to
 * the key value.
 *
 * @param root node of the tree
 * @param name name (and key) of the new node
 *
 * @return 0 if successful, errcode otherwise
 */
int treeInsert(STTreeNode **root, char *key) ForceRetUse;

/**
 * @brief Replaces the target node with the rightmost node from the given
 * subtree.
 *
 * @param target node to replace
 * @param tree an initialised tree with at least one right child
 */
void replaceByRightmost(STTreeNode *target, STTreeNode **tree);

/**
 * @brief Deletes a node with the given key, frees the memory. If the root is null nothing happens.
 *
 * @param root node of the tree
 * @param key to delete
 */
void treeDelete(STTreeNode **root, char *key);

/**
 * @brief Finds a node and returns a pointer to all its data
 *
 * @param root of a tree
 * @param key this is the key we are looking for
 * @param data where the data will be saved (buffer does not have to be initialised)
 *
 * @return pointer to symbol table tree element
 */
STElem *treeGetData(STTreeNode *root, char *key);

#endif
/* end of file symtable_tree.h */
