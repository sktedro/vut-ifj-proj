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
  // Error if there is a variable with the same key already in that frame
  STTreeNode *tree = stack->top->table;
  STElem *existingElem = treeGetData(tree, key);
  if(existingElem && existingElem->isVariable){
    return ERR(ID_DEF_ERR);
  }
  STStackElem *frame = STStackTop(stack);
  TryCall(treeInsert, &(frame->table), key, stack->top->depth);
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
 * @brief Sets name name of the element - name in IFJcode21 language
 * 
 * @param stack: symable
 * @param key
 * @param name of the new

 * @return 0 if successful, errcode otherwise
 */
int STSetName(STStack *stack, char *key, char *name) {
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  char *newName;
  GCMalloc(newName, sizeof(char) * (strlen(name) + 1));
  memcpy(newName, name, strlen(name) + 1);
  STFind(stack, key)->name = newName;
  return 0;
}

/**
 * @brief Set isVariable boolean of a symbol table element (a variable or a 
 * function)
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param val boolean - true if it is a variable, false if it is a function

 * @return 0 if successful, errcode otherwise
 */
int STSetIsVariable(STStack *stack, char *key, bool val) {
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  STFind(stack, key)->isVariable = val;
  return 0;
}

/**
 * @brief Set data type of a variable in a symbol table
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param type - data type the element represents

 * @return 0 if successful, errcode otherwise
 */
int STSetVarDataType(STStack *stack, char *key, int type) {
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  STFind(stack, key)->varDataType = type;
  return 0;
}

/**
 * @brief Use to mark a function as defined
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param fnDefined - boolean value to be written to STElem->fnDefined

 * @return 0 if successful, errcode otherwise
 */
int STSetFnDefined(STStack *stack, char *key, bool fnDefined) {
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  // Redefinition of a function!
  if(STFind(stack, key)->fnDefined == true){
    return ERR(ID_DEF_ERR);
  }
  // Declaring a function after it has been already defined!
  STFind(stack, key)->fnDefined = fnDefined;
  return 0;
}

/**
 * @brief Use to mark a function as declared 
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param fnDeclared - boolean value to be written to STElem->fnDeclared

 * @return 0 if successful, errcode otherwise
 */
int STSetFnDeclared(STStack *stack, char *key, bool fnDeclared) {
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  // Redeclaration of a function!
  if(STFind(stack, key)->fnDeclared == true){
    return ERR(ID_DEF_ERR);
  }
  // Declaring a function after it has been already defined!
  if(STFind(stack, key)->fnDefined == true){
    return ERR(ID_DEF_ERR);
  }
  STFind(stack, key)->fnDeclared = fnDeclared;
  return 0;
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
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  if(paramType == -1){
    return ERR(SYNTAX_ERR);
  }
  TryCall(intBufAppend, STFind(stack, key)->fnParamTypesBuf, paramType);
  return 0;
}

/**
 * @brief Appends a name of a paramter of a function
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param paramName - name of the parameter
 *
 * @return 0 if successful, errcode otherwise
 */
int STAppendParamName(STStack *stack, char *key, char *paramName) {
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  if(!paramName){
    return ERR(SYNTAX_ERR);
  }
  TryCall(stringBufAppend, STFind(stack, key)->fnParamNamesBuf, paramName);
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
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  TryCall(intBufAppend, STFind(stack, key)->fnRetTypesBuf, retType);
  return 0;
}

/**
 * @brief Returns depth of a symbol table element (if it occurs at various
 * depths, returns the highest number). Returns -1 if not found
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return depth of the element, -1 if an element doesn't exist
 */
int STGetDepth(STStack *stack, char *key) {
  if(!STFind(stack, key)){
    return ERR(-1);
  }
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
 * @brief Returns name of STElement in ifjcode21
 * 
 * @param stack: symtable
 * @param destPtr: destination pointer
 * @param key
 *
 * @return 0 if successful, errcode otherwise
 */
int STGetName(STStack *stack, char **destPtr, char *key) {
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  *destPtr = STFind(stack, key)->name;
  return 0;
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
  // TODO check if elem exists
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
  if(!STFind(stack, key)){
    return ERR(-1);
  }
  return STFind(stack, key)->varDataType;
}

/**
 * @brief Returns true if a function with name 'key' was already defined
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return true if the function was defined
 */
bool STGetFnDefined(STStack *stack, char *key) {
  // TODO check if elem exists
  STElem *data = STFind(stack, key);
  if (data) {
    return data->fnDefined;
  }
  return false;
}

/**
 * @brief Returns true if a function with name 'key' was already declared
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return true if the function was declared
 */
bool STGetFnDeclared(STStack *stack, char *key) {
  // TODO check if elem exists
  STElem *data = STFind(stack, key);
  if (data) {
    return data->fnDeclared;
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
  if(!STFind(stack, key)){
    return ERR(-1);
  }
  STElem *data = STFind(stack, key);
  if (data->fnParamTypesBuf->len > index) {
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
  if(!STFind(stack, key)){
    return ERR(-1);
  }
  STElem *data = STFind(stack, key);
  if (data->fnRetTypesBuf->len > index) {
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
  if(!STFind(stack, key)){
    return ERR(SYNTAX_ERR);
  }
  STElem *data = STFind(stack, key);
  if (data->fnParamNamesBuf->len > index) {
    *destPtr = data->fnParamNamesBuf->data[index];
    return 0;
  }
  return -1;
}

/**
 * @brief Deletes all elements of the symtable one by one and if an undefined
 * function is found, returns an error
 *
 * @param stack: symtable
 *
 * @return 0 if no undefined function was found, errcode otherwise
 */
int STFindUndefinedFunctions(STStack *stack){
  STStackElem *tree = stack->top;
  while(tree->table){
    if(STFind(stack, tree->table->key)
        && !STGetIsVariable(stack, tree->table->key) 
        && !STGetFnDefined(stack, tree->table->key)){
      return ERR(ID_DEF_ERR);
    }
    treeDelete(&(tree->table), tree->table->key);
  }
  return 0;
}

#endif
/* end of file symtable.c */
