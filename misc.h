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
 * Enumerations
 */



// Enumeration of states of the finite state machine
// The commented out states are not used, as instead of setting them as the
// next state, the token is instantly returned
enum FSMEnum{
  s_start,

    // Identificator or a keyword
  s_idOrKeyword,

    // Number literals
  s_int,
  s_num,
  s_scientific,
  s_needNum,
  s_sciNum,

    // Comments
  s_comment,
  s_unknownComment,
  s_singleLineComment,
  s_multiLineComment,
  s_multiLineCommentPossibleEnd,

    // Operators
  s_arithmOpDash,
  s_arithmOpDiv,
  s_dot,
  s_tilde,
  s_relOpSimple,
  s_assignment,
  // s_colon,
  // s_comma,
  // s_arithmOp,
  s_strOp,
  // s_relOp,

    // Parentheses
  // s_leftParen,
  // s_rightParen,

    // String literals
  s_strStart,
  // s_strEnd
};


// Enumeration of the token types
enum TokenTypeEnum{
  t_idOrKeyword, // one token for both identificator and a keyword??
  t_int,
  t_num,
  t_sciNum,
  t_str,
  t_colon,
  t_comma,
  t_rightParen,
  t_leftParen,
  t_arithmOp,
  t_strOp,
  t_relOp,
  t_assignment,
};


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



// Structure defining the buffer
typedef struct{
  char *data;
  int len; // Actual buffer data length
  int size; // Size allocated for the buffer
} Buffer;


// Structure defining a token
typedef struct{
  int type;
  char *data;
} Token;


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


// Node structure for the binary search tree.
typedef struct node {
    Buffer *key; // id string
    Buffer *data; // id value
    struct node *leftChild;
    struct node *rightChild;
} Node;



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
