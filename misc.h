/*
 * Enumerators, structure definitions and miscellaneous functions
 */

#ifndef MISC_H
#define MISC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ♥
 */
#define vypluj return

/*
 *  Error returns (will be enum?)
 */
#define LEX_ERR 1         // lexical analysis error
#define SYNTAX_ERR 2      // syntax error
#define ID_DEF_ERR 3      // undefined/redefined function/variable
#define ASS_ERR 4         // assignment error, types incompatible
#define PARAM_RET_ERR 5   // wrong type/number of function parameters/returns
#define TYPE_EXPR_ERR 6   // types incompatible in expression
#define OTHER_SEM_ERR 7   // other semantic error
#define NIL_ERR 8         // unexpected nil
#define DIV_BY_ZERO_ERR 9 // integer division by zero

#define INTERN_ERR 99 // intern error (memory allocation etc.)

/*
 * Macros
 */
#define CondReturn \
  if (ret)         \
    vypluj err(ret)
#define condVypluj CondReturn

/*
 * Enumerations
 */

// Enumeration of states of the finite state machine
// The commented out states are not used, as instead of setting them as the
// next state, the token is instantly returned
enum FSMEnum {
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
enum TokenTypeEnum {
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
  // t_nil ???? Wasn't there, but probably should be
};

// Enumeration of the precedence table
enum PrecTabEnum {
  pt_strlen,
  pt_mult,
  pt_div,
  pt_intDiv,
  pt_add,
  pt_sub,
  pt_concat,
  pt_relOp,
  pt_lParen,
  pt_rParen,
  pt_id,
  pt_dollar
};

// Enumeration of symbol stack type
enum SStackTypeEnum {
  st_push,    // <
  st_reduce,  // > (reduce untill <)
  st_nop,     // =
  st_invalid, // _
  st_idOrLiteral,
  st_operator,
  st_expr,
  st_dollar
};

// Enumeration of IFJ21 language data types
enum IFJ21DataTypes {
  dt_integer,
  dt_number,
  dt_string,
  dt_nil
};

/*
 * Structures
 */

// Structure defining the char buffer
typedef struct {
  char *data;
  int len;  // Actual buffer data length
  int size; // Size allocated for the buffer
} CharBuffer;

// Structure defining the int buffer
typedef struct {
  int *data;
  int len;  // Actual buffer data length
  int size; // Size allocated for the buffer
} IntBuffer;

// Structure defining a token
typedef struct {
  int type;
  char *data;
} Token;

// Symbol stack element
typedef struct SStackElem {
  int type;                // Symbol stack type enum (st_)
  int op;                  // Precedence table enum (pt_)
  bool isId;               // So we know if there's an ID or a literal in *data
  int dataType;            // If it is an ID or a literal, we need the datatype
  char *data;              // ID, literal,...
  struct SStackElem *next;
} SStackElem;

// Symbol stack
typedef struct {
  SStackElem *top;
} SStack;

// Symbol table element
typedef struct {
  char *name;
  bool isVariable; // var or fn
  int varDataType;
  int varAddress;
  bool fnDefined;
  IntBuffer *fnParamTypesBuf;
  IntBuffer *fnRetTypesBuf;
} STElem;

// Symbol table tree structure for the binary search tree.
typedef struct symbolTableTreeNode {
  char *key;    // id string
  STElem *data; // id value
  struct symbolTableTreeNode *leftChild;
  struct symbolTableTreeNode *rightChild;
} STTreeNode;

// Symbol table stack element
typedef struct symbolTableStackElem {
  STTreeNode *table;
  int depth;
  struct symbolTableStackElem *next;
} STStackElem;

// Symbol table stack
typedef struct {
  STStackElem *top;
} STStack;

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
int err(int errCode);

#endif
/* end of file misc.h */
