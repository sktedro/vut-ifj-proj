/**
 * VUT FIT - IFJ 2021
 *
 * @file symtable.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "misc.h"
#include "symtable_stack.h"
#include "string_buffer.h"

/**
 * @brief initialization of symbol table (stack)
 *
 * @param stack: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int STInit(STStack **stack) ForceRetUse;

/**
 * @brief Push new frame on top of the stack
 *
 * @param stack - stack with frames
 *
 * @return 0 if successful, errcode otherwise
 * */
int STPush(STStack *stack) ForceRetUse;

/**
 * @brief Pops a frame from the symbol table
 *
 * @param stack - stack with frames
 */
void STPop(STStack *stack);

/**
 * @brief Insert a new element to the symbol table (at the top of the stack)
 *
 * @param stack - symbol table (stack)
 * @param key - name of that new element
 *
 * @return 0 if successful, errcode otherwise
 */
int STInsert(STStack *stack, char *key) ForceRetUse;

/**
 * @brief Finds element in tree by key and return its data as a pointer
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return STElem if it finds it, else NULL
 */
STElem *STFind(STStack *stack, char *key);

/**
 * @brief Sets name name of the element - name in IFJcode21 language
 * 
 * @param stack: symable
 * @param key
 * @param name of the new

 * @return 0 if successful, errcode otherwise
 */
int STSetName(STStack *stack, char *key, char *name) ForceRetUse;

/**
 * @brief Set isVariable boolean of a symbol table element (a variable or a 
 * function)
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param val boolean - true if it is a variable, false if it is a function

 * @return 0 if successful, errcode otherwise
 */
int STSetIsVariable(STStack *stack, char *key, bool val) ForceRetUse;

/**
 * @brief Set data type of a variable in a symbol table
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param type - data type the element represents

 * @return 0 if successful, errcode otherwise
 */
int STSetVarDataType(STStack *stack, char *key, int type) ForceRetUse;

/**
 * @brief Use to mark a function as defined
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param fnDefined - boolean value to be written to STElem->fnDefined

 * @return 0 if successful, errcode otherwise
 */
int STSetFnDefined(STStack *stack, char *key, bool fnDefined) ForceRetUse;

/**
 * @brief Use to mark a function as declared 
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param fnDeclared - boolean value to be written to STElem->fnDeclared

 * @return 0 if successful, errcode otherwise
 */
int STSetFnDeclared(STStack *stack, char *key, bool fnDeclared) ForceRetUse;

/**
 * @brief Appends a data type of a paramter of a function
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param paramType - data type of the parameter to be appended
 *
 * @return 0 if successful, errcode otherwise
 */
int STAppendParamType(STStack *stack, char *key, int paramType) ForceRetUse;

/**
 * @brief Appends a name of a paramter of a function
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param paramName - name of the parameter
 *
 * @return 0 if successful, errcode otherwise
 */
int STAppendParamName(STStack *stack, char *key, char *paramName) ForceRetUse;

/**
 * @brief Appends a data type of a return value of a function
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 * @param retType - data type of the return value to be appended
 *
 * @return 0 if successful, errcode otherwise
 */
int STAppendRetType(STStack *stack, char *key, int retType) ForceRetUse;

/**
 * @brief Returns depth of a symbol table element (if it occurs at various
 * depths, returns the highest number). Returns -1 if not found
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return depth of the element, -1 if an element doesn't exist
 */
int STGetDepth(STStack *stack, char *key) ForceRetUse;

/**
 * @brief Returns name of STElement in ifjcode21
 * 
 * @param stack: symtable
 * @param destPtr: destination pointer
 * @param key
 *
 * @return 0 if successful, errcode otherwise
 */
int STGetName(STStack *stack, char **destPtr, char *key) ForceRetUse;

/**
 * @brief Returns true if an element with name 'key' is a variable
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return true if the element is a variable
 */
bool STGetIsVariable(STStack *stack, char *key) ForceRetUse;

/**
 * @brief Returns the data type of a variable in a symbol table
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return data type of the variable
 */
int STGetVarDataType(STStack *stack, char *key) ForceRetUse;

/**
 * @brief Returns true if a function with name 'key' was already defined
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return true if the function was defined
 */
bool STGetFnDefined(STStack *stack, char *key) ForceRetUse;

/**
 * @brief Returns true if a function with name 'key' was already declared
 *
 * @param stack - symbol table
 * @param key (name) of the symbol table element
 *
 * @return true if the function was declared
 */
bool STGetFnDeclared(STStack *stack, char *key) ForceRetUse;

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
int STGetParamType(STStack *stack, char *key, int index) ForceRetUse;

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
int STGetRetType(STStack *stack, char *key, int index) ForceRetUse;

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
int STGetParamName(STStack *stack, char **destPtr, char *key, int index) ForceRetUse;

/**
 * @brief Deletes all elements of the symtable one by one and if an undefined
 * function is found, returns an error
 *
 * @param stack: symtable
 *
 * @return 0 if no undefined function was found, errcode otherwise
 */
int STFindUndefinedFunctions(STStack *stack) ForceRetUse;

#endif
/* end of file symtable.h */
