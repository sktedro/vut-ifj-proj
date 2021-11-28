/*
 * Enumerators, structure definitions and miscellaneous functions
 */

#ifndef MISC_C
#define MISC_C

#include "misc.h"

/*
 * Miscellaneous functions
 */


bool strEq(char *str1, char *str2){
  return !strcmp(str1, str2);
}
/**
 * @brief Writes an error message to stdout and returns back the error code
 *
 * @param errCode
 *
 * @return errCode
 */
int err(int errCode) {
  fprintf(stderr, "ERROR: ");
  if (errCode == LEX_ERR) {
    fprintf(stderr, "Lexical analysis error.\n"); //TODO
  } else if (errCode == SYNTAX_ERR) {
    fprintf(stderr, "Syntax error.\n"); //TODO
  } else if (errCode == ID_DEF_ERR) {
    fprintf(stderr, "Function/variable definition error.\n"); //TODO
  } else if (errCode == ASS_ERR) {
    fprintf(stderr, "Assignment error (types might be uncompatible).\n"); //TODO
  } else if (errCode == PARAM_RET_ERR) {
    fprintf(stderr, "Function parameters or return values error. Check types.\n"); //TODO
  } else if (errCode == TYPE_EXPR_ERR) {
    fprintf(stderr, "Data types of operands in an expression are not compatible.\n"); //TODO
  } else if (errCode == OTHER_SEM_ERR) {
    fprintf(stderr, "Semantic error.\n"); //TODO
  } else if (errCode == NIL_ERR) {
    fprintf(stderr, "Unexpected nil.\n"); //TODO
  } else if (errCode == DIV_BY_ZERO_ERR) {
    fprintf(stderr, "Division by zero error.\n"); //TODO
  } else if (errCode == INTERN_ERR) {
    fprintf(stderr, "Internal error.\n"); //TODO
  }else{
    fprintf(stderr, "Unknown error.\n");
  }

  // TODO:
  exit(errCode);
  //vypluj errCode;
}

#endif
/* end of file misc.c */
