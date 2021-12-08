/*
 * A stack for the symbol table
 */

#ifndef SYMTABLE_STACK_H
#define SYMTABLE_STACK_H

#include "misc.h"
#include "symtable_tree.h"

/**
 * @brief Allocate a new stack, initialize it and return it
 *
 * @param stack: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int STStackInit(STStack **stack) ForceRetUse;

 /**
 * @brief allocate and push a new element to the top of the stack
 *
 * @param stack
 * @param table - new tree
 * @param depth of the new tree
 *
 * @return 0 if successful, errcode otherwise
 */
int STStackPush(STStack *stack, STTreeNode *table, int depth) ForceRetUse;

/**
 * @brief remove the top element
 *
 * @param stack
 */
void STStackPop(STStack *stack);

/**
 * @brief get the element at the top of the stack
 *
 * @param stack
 *
 * @return top element
 */
STStackElem *STStackTop(STStack *stack) ForceRetUse;

/**
 * @brief get the element at the bottom of the stack
 *
 * @param stack
 *
 * @return bottom element
 */
STStackElem *STStackBottom(STStack *stack) ForceRetUse;

/**
 * @brief get the element n elements from the top of the stack
 *
 * @param stack
 * @param n: number of the element
 *
 * @return n-th element (if n = 0, returns stack top)
 */
STStackElem *STStackNthElem(STStack *stack, int n) ForceRetUse;

#endif
/* end of file symtable_stack.h */
