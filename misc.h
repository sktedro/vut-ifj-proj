/*
 * Enumerators, structure definitions and miscellaneous functions
 */

#ifndef MISC_H
#define MISC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GCINITLEN 1024


// #include "garbage_collector.h"

/*
 * ♥
 */
#define vypluj return

#define ForceRetUse __attribute__((warn_unused_result))

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

#define TryCall(FN, ...) \
  ret = FN(__VA_ARGS__);  \
  CondReturn

#define GCMalloc(ptr, len)                                                     \
  ptr = malloc(len);                                                           \
  if(!ptr) {                                                                   \
  }                                                                            \
  ret = GCInsert((void*)ptr);                                                  \
  if(ret) {                                                                    \
  }
// TODO return ret

#define GCRealloc(ptr, len)                                                    \
  GCDelete(ptr);                                                               \
  ptr = realloc(ptr, len);                                                     \
  if(!ptr) {                                                                   \
  }                                                                            \
  ret = GCInsert((void*)ptr);                                                  \
  if(ret) {                                                                    \
  }
// TODO return ret

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
  /*00*/ t_idOrKeyword, // one token for both identificator and a keyword??
  /*01*/ t_int,
  /*02*/ t_num,
  /*03*/ t_sciNum,
  /*04*/ t_str,
  /*05*/ t_colon,
  /*06*/ t_comma,
  /*07*/ t_rightParen,
  /*08*/ t_leftParen,
  /*09*/ t_arithmOp,
  /*10*/ t_strOp,
  /*11*/ t_relOp,
  /*12*/ t_assignment,
  // t_nil ???? Wasn't there, but probably should be
};

// Enumeration of the precedence table (SStackElem->op)
enum PrecTabEnum {
  /*00*/ pt_strlen,
  /*01*/ pt_mult,
  /*02*/ pt_div,
  /*03*/ pt_intDiv,
  /*04*/ pt_add,
  /*05*/ pt_sub,
  /*06*/ pt_concat,
  /*07*/ pt_relOp,
  /*08*/ pt_lParen,
  /*09*/ pt_rParen,
  /*10*/ pt_id,
  /*11*/ pt_dollar,
};

// Enumeration of symbol stack type
enum SStackTypeEnum {
  /*00*/ st_push,          // <
  /*01*/ st_reduce,        // > (reduce untill <)
  /*02*/ st_nop,           // =
  /*03*/ st_invalid,       // _
  /*04*/ st_idOrLiteral,
  /*05*/ st_operator,
  /*06*/ st_expr,
  /*07*/ st_dollar,
};

// Enumeration of IFJ21 language data types
enum IFJ21DataTypes {
  /*00*/ dt_integer,
  /*01*/ dt_number,
  /*02*/ dt_string,
  /*03*/ dt_nil,
  /*04*/ dt_boolean,
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
                           // Will be -1 for boolean
  char *data;              // ID (or expr) name or a literal value
  bool isZero;             // Will be true if the elem is a literal equal to 0
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

// Garbage collector (storage for pointers to every allocated memory block)
typedef struct {
  void **pointers;
  int ptrsAllocated;
  int ptrsUsed;
} GarbageCollector;

/*
 * Miscellaneous functions
 */

/**
 * @brief returns true if two strings are equal
 *
 * @param str1
 * @param str2
 *
 * @return true if strings are equal
 */
bool strEq(char *str1, char *str2) ForceRetUse;

/**
 * @brief Writes an error message to stdout and returns back the error code
 *
 * @param errCode
 *
 * @return errCode
 */
int err(int errCode) ForceRetUse;

int GCInit() ForceRetUse;

int GCInsert(void *ptr) ForceRetUse;

void GCDelete();

void GCCollect();


#endif
/* end of file misc.h */
