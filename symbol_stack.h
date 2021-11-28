/*
 * A stack structure implemented as a linked list
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
SStackElem *SStackTop(SStack *stack);

/*
 * @brief get an element of type terminal that is closest to the top of the
 * stack. Returns NULL if none exists
 *
 * @param stack
 *
 * @return top terminal element
 */
SStackElem *SStackTopTerminal(SStack *stack);

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

/*
 * @brief Free all memory allocated by the symbol stack
 *
 * @param stack to be freed
 */
void SStackDestroy(SStack *stack);

#endif
/* end of file symbol_stack.h */
