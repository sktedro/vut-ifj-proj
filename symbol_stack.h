/**
 * VUT FIT - IFJ 2021
 *
 * @file symbol_stack.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef SYMBOL_STACK_H
#define SYMBOL_STACK_H

#include "misc.h"

/*
 * @brief Allocate a new stack, initialize it and return it
 *
 * @param stack: destination pointer
 * 
 * @return 0 if successful, errcode otherwise
 */
int SStackInit(SStack **stack) ForceRetUse;

/*
 * @brief allocate and push a new element to the top of the stack
 *
 * @param stack
 * @param newElem to be pushed
 *
 * @return 0 if successful, errcode otherwise
 */
int SStackPush(SStack *stack, SStackElem *newElem) ForceRetUse;

/*
 * @brief remove (and free it's allocated memory) the top element
 *
 * @param stack
 */
SStackElem *SStackPop(SStack *stack);

/*
 * @brief get the element at the top of the stack
 *
 * @param stack
 *
 * @return top element
 */
SStackElem *SStackTop(SStack *stack) ForceRetUse;

/*
 * @brief get an element of type terminal that is closest to the top of the
 * stack. Returns NULL if none exists
 *
 * @param stack
 *
 * @return top terminal element
 */
SStackElem *SStackTopTerminal(SStack *stack) ForceRetUse;

/*
 * @brief finds an element of type terminal that is closest to the top of the
 * stack and appends a new element after it
 *
 * @param stack
 * @param newElem to be pushed
 *
 * @return 0 if successful, errcode otherwise
 */
int SStackPushAfterTopTerminal(SStack *stack, SStackElem *newElem) ForceRetUse;

/**
 * @brief Simple debugging function to print all elements on the symbol stack
 *
 * @param stack to print out
 */
void printSymbolStack(SStack *stack);

#endif
/* end of file symbol_stack.h */
