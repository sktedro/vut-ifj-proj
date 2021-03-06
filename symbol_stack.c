/**
 * VUT FIT - IFJ 2021
 *
 * @file symbol_stack.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef SYMBOL_STACK_C
#define SYMBOL_STACK_C

#include "symbol_stack.h"

extern int ret;

/**
 * @brief Allocate a new stack, initialize it and return it
 *
 * @param stack: destination pointer
 * 
 * @return 0 if successful, errcode otherwise
 */
int SStackInit(SStack **stack) {
  GCMalloc(*stack, sizeof(SStack));
  if (!(*stack)) {
    return ERR(INTERN_ERR);
  }
  (*stack)->top = NULL;
  return 0;
}

/**
 * @brief Push a new element to the top of the stack
 *
 * @param stack
 * @param newElem to be pushed
 *
 * @return 0 if successful, errcode otherwise
 */
int SStackPush(SStack *stack, SStackElem *newElem) {
  if (!stack) {
    return ERR(INTERN_ERR);
  }
  if (!newElem) {
    return ERR(INTERN_ERR);
  }
  newElem->next = stack->top;
  stack->top = newElem;
  return 0;
}

/**
 * @brief remove (and <edited not to> free it's allocated memory) the top element
 *
 * @param stack
 */
SStackElem *SStackPop(SStack *stack) {
  if (!stack || !stack->top) {
    return NULL;
  }
  SStackElem *tmp = stack->top;
  stack->top = tmp->next;
  return tmp;
}

/**
 * @brief get the element at the top of the stack
 *
 * @param stack
 *
 * @return top element
 */
SStackElem *SStackTop(SStack *stack) {
  if (!stack) {
    return NULL;
  }
  return stack->top;
}

/**
 * @brief get an element of type terminal that is closest to the top of the
 * stack. Returns NULL if none exists
 *
 * @param stack
 *
 * @return top terminal element
 */
SStackElem *SStackTopTerminal(SStack *stack) {
  if (!stack) {
    vypluj NULL;
  }
  SStackElem *tmp = stack->top;
  while (tmp) {
    if (tmp->type != st_expr) {
      vypluj tmp;
    }
    tmp = tmp->next;
  }
  vypluj NULL;
}

/**
 * @brief finds an element of type terminal that is closest to the top of the
 * stack and appends a new element after it (above)
 *
 * @param stack
 * @param newElem to be pushed
 *
 * @return 0 if successful, errcode otherwise
 */
int SStackPushAfterTopTerminal(SStack *stack, SStackElem *newElem) {
  if (!stack || !stack->top) {
    vypluj ERR(SYNTAX_ERR); // TODO errcode
  }
  if (!newElem) {
    return ERR(INTERN_ERR);
  }

  SStackElem *tmp = stack->top;
  if(tmp->type != st_expr){
    // The top terminal is the stack top
    TryCall(SStackPush, stack, newElem);
    return 0;
  }
  while(tmp->next){
    // tmp->next is the top terminal
    if(tmp->next->type != st_expr){
      // Put '<' after (above) the tmp
      newElem->next = tmp->next;
      tmp->next = newElem;
      return 0;
    }
    tmp = tmp->next;
  }

  // This line should never be ran, but in case it does...
  return ERR(SYNTAX_ERR);
}

/**
 * @brief Simple debugging function to print all elements on the symbol stack
 *
 * @param stack to print out
 */
void printSymbolStack(SStack *stack) {
  if(!DEBUGTOGGLE){
    return;
  }
  int len = 0;
  SStackElem *element = SStackTop(stack);
  fprintf(stderr, "--------STACK------------\n");
  while (element != NULL) {
    fprintf(stderr, "Element type: %d\n", element->type);
    fprintf(stderr, "Element op: %d\n", element->op);
    if (element->data != NULL) {
      fprintf(stderr, "Element data: %s\n", element->data);
    } else {
      fprintf(stderr, "Element data is NULL\n");
    }
    fprintf(stderr, "Element isId: %d\n", element->isId);
    fprintf(stderr, "Element dataType: %d\n", element->dataType);
    fprintf(stderr, "-------------------------\n");

    if (element->next == NULL) {
      fprintf(stderr, "Next element is NULL\n");
      fprintf(stderr, "Total length is: %d\n", len + 1);
      fprintf(stderr, "-------------------------\n");
      return;
    } else {
      element = element->next;
      len++;
    }
  }
  fprintf(stderr, "Stack is empty\n");
}

#endif
/* end of file symbol_stack.c */
