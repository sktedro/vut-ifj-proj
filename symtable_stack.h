/*
 * A stack for the symbol table
 */


#ifndef SYMTABLE_STACK
#define SYMTABLE_STACK

#include "misc.h"
#include "symtable_tree.h"

/**
 * @brief Allocate a new stack, initialize it and return it
 *
 * @return a new stack
 */
STStack *STStackInit() {
  STStack *stack = (STStack*) malloc(sizeof(STStack));
  if(stack == NULL) {
    // TODO memleak?
    exit(MALLOC_ERROR);
  }
  stack->top = NULL;
  return stack;
}

/**
 * @brief allocate and push a new element to the top of the stack
 *
 * @param stack
 * @param name - new element's name
 *
 * @return 0 if successful
 */
int STStackPush(STStack *stack, STTreeNode *table, int depth){
  if(!stack){
    // TODO memleak
    exit(err(MALLOC_ERROR));
  }
  STStackElem *newElem = (STStackElem*)malloc(sizeof(STStackElem));
  if(!newElem){
    // TODO memleak
    exit(err(MALLOC_ERROR));
  }
  newElem->table = table;
  newElem->depth = depth;
  newElem->next = stack->top;
  stack->top = newElem;
  return 0;
}

/**
 * @brief remove (and free it's allocated memory) the top element
 *
 * @param stack
 */
void STStackPop(STStack *stack){
  if(!stack || !stack->top){
    return;
  }
  STStackElem *tmp = stack->top;
  stack->top = stack->top->next;
  treeDestroy(&(tmp->table));
  free(tmp);
}


/**
 * @brief get the element at the top of the stack
 *
 * @param stack
 *
 * @return top element
 */
STStackElem *STStackTop(STStack *stack){
  if(!stack){
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
STStackElem *STStackBottom(STStack *stack){
  if(!stack){
    return NULL;
  }
  STStackElem *tmp = stack->top;
  if(!tmp){
    return NULL;
  }
  while(tmp->next){
    tmp = tmp->next;
  }
  return tmp;
}

/**
 * @brief get the element n elements from the top of the stack
 *
 * @param stack
 *
 * @return n-th element (if n = 0, returns stack top)
 */
STStackElem *STStackNthElem(STStack *stack, int n){
  if(!stack){
    return NULL;
  }
  STStackElem *tmp = stack->top;
  for(int i = 0; i < n; i++){
    if(!tmp){
      return NULL;
    }
    tmp = tmp->next;
  }
  return tmp;
}

/**
 * @brief Free all memory allocated by the symbol stack
 *
 * @param stack to be freed
 */
void STStackDestroy(STStack **stack){
  if(!stack || !(*stack)){
    return;
  }
  while((*stack)->top){
    STStackPop(*stack);
  }
  free(*stack);
  *stack = NULL;
}

#endif
/* end of file symtable_stack.h */