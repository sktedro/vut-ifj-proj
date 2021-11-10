/*
 * Enumerators, structure definitions and miscellaneous functions
 */

#ifndef MISC
#define MISC

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/*
 * ♥    
 */
#define vypluj return


/*
 * Enumerations:
 */

//TODO
// Enumeration of symbol stack type
enum SStackTypeEnum{
  st_,
  st
};

//TODO arithm. operators, relation operators and so on?
// Enumeration of symbol stack symbol
enum SStackSymbolEnum{
  sym_dollar,
  sym_nonterminal
};


/*
 * Structures
 */

// Symbol stack element
typedef struct SStackElem{
  int type;
  int symbol;
  struct SStackElem *next;
} SStackElem;

// Symbol stack
typedef struct{
  SStackElem *top;
} SStack;


/*
 * Miscellaneous functions
 */

/**
 * @brief Writes an error message to stdout and returns back the error code
 *
 * @param errCode
 *
 * @return errCode
 */
int err(int errCode){
  if(errCode == 1){
    fprintf(stderr, "Lexical analysis error.\n"); //TODO
  }else if(errCode == 99){
    fprintf(stderr, "Internal error.\n"); //TODO
  }
  vypluj errCode;
}

#endif
