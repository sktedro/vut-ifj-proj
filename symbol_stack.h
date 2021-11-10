#include <stdlib.h>
#include "misc.h"

//TODO
enum SStackTypeEnum{
  st_,
  st
};

//TODO arithm. operators, relation operators and so on?
enum SStackSymbolEnum{
  sym_dollar,
  sym_nonterminal
};

// implemented as a linked list

typedef struct SStackElem{
  int type;
  int symbol;
  struct SStackElem *next;
} SStackElem;

typedef struct{
  SStackElem *top;
} SStack;


/*
 * @brief Allocate a new stack, initialize it and return it
 *
 * @return a new stack
 */
SStack *SStackInit(){
  SStack *stack = (SStack*)malloc(sizeof(SStack));
  if(!stack){
    return NULL;
  }
  stack->top = NULL;
  return stack;
}

/*
 * @brief allocate and push a new element to the top of the stack
 *
 * @param stack
 * @param new element's symbol
 * @param new element's type
 *
 * @return 0 if successful
 */
int SStackPush(SStack *stack, int symbol, int type){
  if(!stack){
    return 1;
  }
  SStackElem *newElem = (SStackElem*)malloc(sizeof(SStackElem));
  if(!newElem){
    return 99;
  }
  newElem->symbol = symbol;
  newElem->type = type;
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
    return NULL;
  }
  SStackElem *tmp = stack->top;
  while(tmp && tmp->type != sym_nonterminal){
    tmp = tmp->next;
  }
  return tmp;
}

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
int SStackPushAfterTopTerminal(SStack *stack, int symbol, int type){
  if(!stack){
    return 1;
  }
  SStackElem *prev = SStackTopTerminal(stack);
  // No terminal found on stack - can't continue
  if(!prev){
    return 1;
  }
  // Allocate the new element and append it after the highest terminal
  SStackElem *newElem = (SStackElem*)malloc(sizeof(SStackElem));
  if(!newElem){
    return 99;
  }
  newElem->symbol = symbol;
  newElem->type = type;
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
