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
 * Constants
 */

// If set to 0, no debug prints will be written
#define DEBUGTOGGLE 1

// Garbage collector - initial length of the pointer array
#define GCINITLEN 1024

/*
 *  Error codes
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
#define INTERN_ERR 99     // intern error (memory allocation etc.)

/*
 * Macros
 */

// ♥
#define vypluj return
#define condVypluj CondReturn

#define ERR(errCode)                                                           \
  DEBUGTOGGLE ?                                                                \
    fprintf(stderr, "LOG: %s:%d:%s(): ", __FILE__, __LINE__, __func__) * 0     \
      + err(errCode)                                                           \
    :                                                                          \
    err(errCode)

// If ret is non-zero, return it
#define CondReturn                                                             \
  if (ret){                                                                    \
    LOG("A function returned %d", ret);                                        \
    vypluj err(ret);                                                           \
  }

// Call a function and return its return value if it is non-zero
#define TryCall(FN, ...)                                                       \
  ret = FN(__VA_ARGS__);                                                       \
  CondReturn

// Prints filename, line number, fn name and given arguments to stderr
#define LOG(fmt, ...)                                                          \
  do {                                                                         \
    if (DEBUGTOGGLE) {                                                         \
      fprintf(stderr, "LOG: %s:%d:%s(): " fmt,                                 \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);                        \
      fprintf(stderr, "\n");                                                   \
    }                                                                          \
  } while (0)

#define RuleFnInit                                                             \
  fprintf(stderr, "--------------------------------------------------\n");     \
  LOG();                                                                       \
  Token *token = NULL;                                                         \
  (void) token;

#define GetToken                                                               \
  ret = scanner(&token);                                                       \
  if(ret){                                                                     \
    LOG("Scanner returned %d", ret);                                           \
    return err(ret);                                                           \
  }                                                                            \
  if(!token){                                                                  \
    LOG("Scanner returned NULL token\n");                                      \
    return err(SYNTAX_ERR);                                                    \
  }                                                                            \
  printToken(token);

// TODO? Just ignore this for now
#define RequireKeyword(str1, str2)                                             \
  if(!strEq(str1, str2)) {                                                     \
    vypluj err(SYNTAX_ERR);                                                    \
  }                                                                            

// TODO? Just ignore this for now
#define ForbidKeyword(str1, str2)                                              \
  if(strEq(str1, str2)) {                                                      \
    vypluj err(SYNTAX_ERR);                                                    \
  }                                                                            

// Get a new token and if the type doesn't match, throw a syntax err
#define RequireTokenType(tokenType)                                            \
  GetToken;                                                                    \
  if(token->type != tokenType) {                                               \
    LOG("An unexpected token was received");                                   \
    vypluj err(SYNTAX_ERR);                                                    \
  }

// Get a new token and if the type or data don't match, throw a syntax err
#define RequireToken(tokenType, tokenData)                                     \
  GetToken;                                                                    \
  if(token->type != tokenType                                                  \
      || strcmp(token->data, tokenData) != 0) {                                \
    LOG("An unexpected token was received");                                   \
    vypluj err(SYNTAX_ERR);                                                    \
  }                                                                            \

// Precedence analysis: call a rule function and if it returned an error code
// (not -1), return it
#define TRYRULE(FN, ...)                                                       \
    rulesRet = FN(symstack, __VA_ARGS__);                                      \
    if(rulesRet != -1){                                                        \
      return rulesRet;                                                         \
    }

// Malloc, check if successful and save it to the garbage collector
#define GCMalloc(ptr, len)                                                     \
  ptr = malloc(len);                                                           \
  if(!ptr) {                                                                   \
    /*TODO return err(INTERN_ERR);                                                    */\
  }                                                                            \
  ret = GCInsert((void*)ptr);                                                  \
  if(ret) {                                                                    \
    /*TODO return err(ret);                                                           */\
  }

// Realloc, check if successful and update the pointer in the garbage collector
#define GCRealloc(ptr, len)                                                    \
  GCDelete(ptr);                                                               \
  ptr = realloc(ptr, len);                                                     \
  if(!ptr) {                                                                   \
    /*TODO return err(INTERN_ERR);                                                    */\
  }                                                                            \
  ret = GCInsert((void*)ptr);                                                  \
  if(ret) {                                                                    \
    /*TODO return err(ret);                                                           */\
  }

#define ForceRetUse __attribute__((warn_unused_result))

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

typedef struct {
  char **data;
  int len;
  int size;
} StringBuffer;

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
  bool fnDeclared;
  IntBuffer *fnParamTypesBuf;
  StringBuffer *fnParamNamesBuf;
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

// for multiple assignment element
typedef struct assignmentElement {
  char *name;    // id string
  char *label; // id value
  bool first;
  bool generated;
  char *end;
  struct assignmentElement *prev; 
  struct assignmentElement *next;
} AssignElement;

/* do not remove until var redefinition is solved
typedef struct varDefStackStruct {
  StringBuffer *data;
  struct varDefStackStruct *next;
} VarDefStack;
*/

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
bool strEq(char *str1, char *str2);

/**
 * @brief returns an integer representation (enum) of the string, if it
 * contains a data type keyword
 *
 * @param str
 *
 * @return data type enum or -1 if str is not a data type keyword
 */
int getDataTypeFromString(char *str);

/**
 * @brief Check if token is keyword in IFJ21 language
 *
 * @param token
 *
 * @return true if the string is a keyword
 */
bool isIFJ21Keyword(Token *token);

int countDigits(int value);

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
