/**
 * VUT FIT - IFJ 2021
 * 
 * @file symtable_tree.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef SYMTABLE_TREE
#define SYMTABLE_TREE

#include "dynamic_int_array.h"
#include "dynamic_string_array.h"
#include "generator.h"
#include "misc.h"

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
int treeInsert(STTreeNode **root, char *key, int frame) ForceRetUse;

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
 * @param key 
 *
 * @return pointer to symbol table tree element
 */
STElem *treeGetData(STTreeNode *root, char *key);

/**
 * @brief Creates a new node with the given key and key, allocates memory.
 *
 * @param node: destination pointer
 * @param key key of the new node
 * @param frame: number of the frame in which the variable is
 *
 * @return 0 if successful, errcode otherwise
 */
int newSTTreeNode(STTreeNode **node, char *key, int frame) ForceRetUse;

/**
 * @brief Replaces the target node with the rightmost node from the given
 * subtree.
 *
 * @param target node to replace
 * @param tree an initialised tree with at least one right child
 */
void replaceByRightmost(STTreeNode *target, STTreeNode **tree);

#endif
/* end of file symtable_tree.h */
