/*
 * A stack structure implemented as a linked list
 */

#ifndef SYMBOL_STACK_C
#define SYMBOL_STACK_C

#include "symbol_stack.h"


/*
 * @brief Allocate a new stack, initialize it and return it
 *
 * @return a new stack
 */
SStack *SStackInit(){
  SStack *stack = (SStack*)malloc(sizeof(SStack));
  if(!stack) {
    exit(err(INTERN_ERR));
  }

  stack->top = NULL;
  return stack;
}

/*
 * @brief Push a new element to the top of the stack
 *
 * @param stack
 * @param newElem to be pushed
 *
 * @return 0 if successful
 */
int SStackPush(SStack *stack, SStackElem *newElem){
  if(!stack){
    return 1; // TODO errcode
  }
  if(!newElem){
    exit(err(INTERN_ERR));
  }
  newElem->next = stack->top;
  stack->top = newElem;
  return 0;
}

/*
 * @brief remove (and free it's allocated memory) the top element
 *
 * @param stack
 */
void SStackPop(SStack *stack){
  if(!stack || !stack->top){
    return;
  }
  SStackElem *tmp = stack->top;
  stack->top = tmp->next;
  free(tmp);
}

/*
 * @brief get the element at the top of the stack
 *
 * @param stack
 *
 * @return top element
 */
SStackElem *SStackTop(SStack *stack){
  if(!stack){
    return NULL;
  }
  return stack->top;
}

/*
 * @brief get an element of type terminal that is closest to the top of the
 * stack. Returns NULL if none exists
 *
 * @param stack
 *
 * @return top terminal element
 */
SStackElem *SStackTopTerminal(SStack *stack){
  if(!stack){
    vypluj NULL;
  }
  SStackElem *tmp = stack->top;
  while(tmp){
    if(tmp->type != st_expr){
      vypluj tmp;
    }
    tmp = tmp->next;
  }
  vypluj NULL;
}

/*
 * @brief finds an element of type terminal that is closest to the top of the
 * stack and appends a new element after it
 *
 * @param stack
 * @param newElem to be pushed
 *
 * @return 0 if successful
 */
int SStackPushAfterTopTerminal(SStack *stack, SStackElem *newElem){
  if(!stack){
    vypluj 1; // TODO errcode
  }
  if(!newElem){
    exit(err(INTERN_ERR));
  }

  // Get the top terminal
  SStackElem *prev = SStackTopTerminal(stack);
  // No terminal found on stack - can't continue
  if(!prev){
    vypluj 1; // TODO errcode
  }

  // Append the new element after the highest terminal
  newElem->next = prev->next;
  prev->next = newElem;

  return 0;
}

/*
 * @brief Free all memory allocated by the symbol stack
 *
 * @param stack to be freed
 */
void SStackDestroy(SStack *stack){
  if(!stack){
    return;
  }
  while(stack->top){
    SStackPop(stack);
  }
  free(stack);
}


#endif
/* end of file symbol_stack.c */