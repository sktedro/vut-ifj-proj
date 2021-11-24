/*
 * A stack structure implemented as a linked list
 */

#ifndef SYMBOL_STACK_H
#define SYMBOL_STACK_H

#include "misc.h"

/*
 * @brief Allocate a new stack, initialize it and return it
 *
 * @return a new stack
 */
SStack *SStackInit();

/*
 * @brief allocate and push a new element to the top of the stack
 *
 * @param stack
 * @param new element's symbol
 * @param new element's type
 *
 * @return 0 if successful
 */
int SStackPush(SStack *stack, SStackElem *newElem);

/*
 * @brief remove (and free it's allocated memory) the top element
 *
 * @param stack
 */
void SStackPop(SStack *stack);

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
 * @param symbol of the new element
 * @param type of the new element
 *
 * @return 0 if successful
 */
int SStackPushAfterTopTerminal(SStack *stack, SStackElem *newElem);

/*
 * @brief Free all memory allocated by the symbol stack
 *
 * @param stack to be freed
 */
void SStackDestroy(SStack *stack);

#endif
/* end of file symbol_stack.h */
