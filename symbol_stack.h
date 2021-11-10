#include <stdlib.h>
#include "misc.h"

//TODO
enum SStackTypeEnum{
  st_,
  st
};

//TODO
enum SStackSymbolEnum{
  ss_,
  ss
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


SStack *SStackInit(){
  SStack *stack = (SStack*)malloc(sizeof(SStack));
  if(!stack){
    return NULL;
  }
  stack->top = NULL;
  return stack;
}

int SStackPush(SStack *stack, int symbol, int type){
  if(!stack){
    return 1;
  }
  SStackElem *newElem = (SStackElem*)malloc(sizeof(SStackElem));
  if(!newElem){
    return 1;
  }
  newElem->symbol = symbol;
  newElem->type = type;
  newElem->next = stack->top;
  stack->top = newElem;
  return 0;
}

void SStackPop(SStack *stack){
  if(!stack || !stack->top){
    return;
  }
  SStackElem *tmp = stack->top;
  stack->top = tmp->next;
  free(tmp);
}

SStackElem *SStackTop(SStack *stack){
  if(!stack){
    return NULL;
  }
  return stack->top;
}

// TODO 
// More operations would probably be useful - eg. find first terminal element
// Not sure yet, what will be needed, so I won't blindly implement it..


void SStackDestroy(SStack *stack){
  if(!stack){
    return;
  }
  while(stack->top){
    SStackPop(stack);
  }
  free(stack);
}
