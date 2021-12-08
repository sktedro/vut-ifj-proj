/**
 * VUT FIT - IFJ 2021
 *
 * @file symtable_stack.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef SYMTABLE_STACK_C
#define SYMTABLE_STACK_C

#include "symtable_stack.h"

extern int ret;

/**
 * @brief Allocate a new stack, initialize it and return it
 *
 * @param stack: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int STStackInit(STStack **stack) {
  GCMalloc(*stack, sizeof(STStack));
  (*stack)->top = NULL;
  return 0;
}

/**
 * @brief allocate and push a new element to the top of the stack
 *
 * @param stack
 * @param table - new tree
 * @param depth of the new tree
 *
 * @return 0 if successful, errcode otherwise
 */
int STStackPush(STStack *stack, STTreeNode *table, int depth) {
  if (!stack) {
    return ERR(INTERN_ERR);
  }
  STStackElem *newElem = NULL;
  GCMalloc(newElem, sizeof(STStackElem));
  newElem->table = table;
  newElem->depth = depth;
  newElem->next = stack->top;
  stack->top = newElem;
  return 0;
}

/**
 * @brief remove the top element
 *
 * @param stack
 */
void STStackPop(STStack *stack) {
  if (!stack || !stack->top) {
    return;
  }
  stack->top = stack->top->next;
}

/**
 * @brief get the element at the top of the stack
 *
 * @param stack
 *
 * @return top element
 */
STStackElem *STStackTop(STStack *stack) {
  if (!stack) {
    return NULL;
  }
  return stack->top;
}

/**
 * @brief get the element at the bottom of the stack
 *
 * @param stack
 *
 * @return bottom element
 */
STStackElem *STStackBottom(STStack *stack) {
  if (!stack || !stack->top) {
    return NULL;
  }
  STStackElem *tmp = stack->top;
  while (tmp->next) {
    tmp = tmp->next;
  }
  return tmp;
}

/**
 * @brief get the element n elements from the top of the stack
 *
 * @param stack
 * @param n: number of the element
 *
 * @return n-th element (if n = 0, returns stack top)
 */
STStackElem *STStackNthElem(STStack *stack, int n) {
  if (!stack) {
    return NULL;
  }
  STStackElem *tmp = stack->top;
  for (int i = 0; i < n; i++) {
    if (!tmp) {
      return NULL;
    }
    tmp = tmp->next;
  }
  return tmp;
}

#endif
/* end of file symtable_stack.c */
