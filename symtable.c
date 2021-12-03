/*
 * Symbol table
 */

#ifndef SYMTABLE_C
#define SYMTABLE_C

#include "symtable.h"

extern int ret;

/**
 * @brief initialization of symbol table (stack)
 *
 * @param stack: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int STInit(STStack **stack) {
  return STStackInit(stack);
}

/**
 * @brief Push new frame on top of the stack
 *
 * @param stack - stack with frames
 *
 * @return 0 if successful, errcode otherwise
 * */
int STPush(STStack *stack) {
  STStackElem *top = STStackTop(stack);
  int depth = 0;
  if (top != NULL) {
    depth = top->depth + 1;
  }
  if (stack) {
    TryCall(STStackPush, stack, NULL, depth);
  }
  return 0;
}

/**
 * @brief Pops a frame from the symbol table
 *
 * @param stack - stack with frames
 */
void STPop(STStack *stack) {
  STStackPop(stack);
}

/**
 * @brief Insert a new element to the symbol table (at the top of the stack)
 *
 * @param stack - symbol table (stack)
 * @param key - name of that new element
 *
 * @return 0 if successful, errcode otherwise
 */
int STInsert(STStack *stack, char *key) {
  STStackElem *frame = STStackTop(stack);
  TryCall(treeInsert, &(frame->table), key);
  return 0;
}

/**
 * @brief Finds element in tree by key and return its data as a pointer
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return STElem if it finds it, else NULL
 */
STElem *STFind(STStack *stack, char *key) {
  if(!key){
    return NULL;
  }
  int i = 0;
  STStackElem *tmp = STStackNthElem(stack, i);
  STElem *data = NULL;
  while (tmp) {
    data = treeGetData(tmp->table, key);
    if (data) {
      return data;
    }
    i++;
    tmp = STStackNthElem(stack, i);
  }
  return NULL;
}

/**
 * @brief Set isVariable boolean of a symbol table element (a variable or a 
 * function)
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param val boolean - true if it is a variable, false if it is a function
 */
void STSetIsVariable(STStack *stack, char *key, bool val) {
  STElem *data = STFind(stack, key);
  if (data) {
    data->isVariable = val;
  }
}

/**
 * @brief Set data type of a variable in a symbol table
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param type - data type the element represents
 */
void STSetVarDataType(STStack *stack, char *key, int type) {
  STElem *data = STFind(stack, key);
  if (data) {
    data->varDataType = type;
  }
}

/**
 * @brief Set address of a variable in a symbol table
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param address - new address of the element
 */
void STSetVarAddress(STStack *stack, char *key, int address) {
  STElem *data = STFind(stack, key);
  if (data) {
    data->varAddress = address;
  }
}

/**
 * @brief Use to mark a function as defined or not
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param fnDefined - boolean value to be written to STElem->fnDefined
 */
void STSetFnDefined(STStack *stack, char *key, bool fnDefined) {
  STElem *data = STFind(stack, key);
  if (data) {
    data->fnDefined = fnDefined;
  }
}

/**
 * @brief Sets name parameter of STStackElem on IFJcode21 generated name
 * 
 * @param stack 
 * @param key 
 * @param name 
 */
void STSetName(STStack *stack, char *key, char *name) {
  STElem *data = STFind(stack, key);

  if(data) {
    data->name = name;
  }
}

/**
 * @brief Appends a data type of a paramter of a function
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param paramType - data type of the parameter to be appended
 *
 * @return 0 if successful, errcode otherwise
 */
int STAppendParamType(STStack *stack, char *key, int paramType) {
  STElem *data = STFind(stack, key);
  if(paramType == -1){
    return err(SYNTAX_ERR);
  }
  if (data) {
    if (!data->fnParamTypesBuf) {
      TryCall(intBufInit, &(data->fnParamTypesBuf));
    }
    TryCall(intBufAppend, data->fnParamTypesBuf, paramType);
  }
  return 0;
}

/**
 * @brief Appends a data type of a return value of a function
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param retType - data type of the return value to be appended
 *
 * @return 0 if successful, errcode otherwise
 */
int STAppendRetType(STStack *stack, char *key, int retType) {
  STElem *data = STFind(stack, key);
  if (data) {
    if (!data->fnRetTypesBuf) {
      TryCall(intBufInit, &(data->fnRetTypesBuf));
    }
    TryCall(intBufAppend, data->fnRetTypesBuf, retType);
  }
  return 0;
}

/**
 * @brief Returns depth of a symbol table element (if it occurs at various
 * depths, returns the highest number). Returns -1 if not found
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return -1 if an element doesn't exist, depth otherwise
 */
int STGetDepth(STStack *stack, char *key) {
  int i = 0;
  STStackElem *tmp = STStackNthElem(stack, i);
  while (tmp) {
    if (treeGetData(tmp->table, key)) {
      return tmp->depth;
    }
    i++;
    tmp = STStackNthElem(stack, i);
  }
  return -1;
}

/**
 * @brief Returns true if an element with name 'key' is a variable
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return true if the element is a variable
 */
bool STGetIsVariable(STStack *stack, char *key) {
  STElem *data = STFind(stack, key);
  if (data) {
    return data->isVariable;
  }
  return false;
}

/**
 * @brief Returns the data type of a variable in a symbol table
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return data type of the variable
 */
int STGetVarDataType(STStack *stack, char *key) {
  STElem *data = STFind(stack, key);
  if (data) {
    return data->varDataType;
  }
  return -1;
}

/**
 * @brief Returns an address of a variable in a symbol table
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return address of the variable
 */
int STGetVarAddress(STStack *stack, char *key) {
  STElem *data = STFind(stack, key);
  if (data) {
    return data->varAddress;
  }
  return -1;
}

/**
 * @brief Returns true if a function with name 'key' was already defined
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return true if the function was already defined
 */
bool STGetFnDefined(STStack *stack, char *key) {
  STElem *data = STFind(stack, key);
  if (data) {
    return data->fnDefined;
  }
  return false;
}

/**
 * @brief returns a data type of a parameter of a function at index 'index'
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param index of the parameter (0 = first parameter, 1 = second parameter...)
 *
 * @return data type of a parameter of a function, -1 if the function doesn't 
 * exist
 */
int STGetParamType(STStack *stack, char *key, int index) {
  STElem *data = STFind(stack, key);
  if (data && data->fnParamTypesBuf && data->fnParamTypesBuf->len > index) {
    return data->fnParamTypesBuf->data[index];
  }
  return -1;
}

/**
 * @brief returns a data type of a return value of a function at index 'index'
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param index of the return value (0 = first value, 1 = second value...)
 *
 * @return data type of a return value of a function, -1 if the function 
 * doesn't exist
 */
int STGetRetType(STStack *stack, char *key, int index) {
  STElem *data = STFind(stack, key);
  if (data && data->fnRetTypesBuf && data->fnRetTypesBuf->len > index) {
    return data->fnRetTypesBuf->data[index];
  }
  return -1;
}

/**
 * @brief returns a name of a parameter of a function at index 'index'
 *
 * @param stack - symbol table
 * @param destPtr - destination pointer
 * @param key (name) of the symbol table element
 * @param index of the return value (0 = first value, 1 = second value...)
 *
 * @return 0 if successful, errcode otherwise
 */
int STGetParamName(STStack *stack, char **destPtr, char *key, int index) {
  STElem *data = STFind(stack, key);
  if (data && data->fnParamNamesBuf && data->fnParamNamesBuf->len > index) {
    *destPtr = data->fnParamNamesBuf->data[index];
    return 0;
  }
  return -1;
}

#endif
/* end of file symtable.c */
