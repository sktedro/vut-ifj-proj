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
  if (errCode == 1) {
    fprintf(stderr, "Lexical analysis error.\n"); //TODO
  } else if (errCode == 2) {
    fprintf(stderr, "Syntax error.\n"); //TODO
  } else if (errCode == 3) {
    fprintf(stderr, "Function/variable definition error.\n"); //TODO
  } else if (errCode == 4) {
    fprintf(stderr, "Assignment error (types might be uncompatible).\n"); //TODO
  } else if (errCode == 5) {
    fprintf(stderr, "Function parameters or return values error. Check types.\n"); //TODO
  } else if (errCode == 6) {
    fprintf(stderr, "Types of operands in an expression are not compatible.\n"); //TODO
  } else if (errCode == 7) {
    fprintf(stderr, "Semantic error.\n"); //TODO
  } else if (errCode == 8) {
    fprintf(stderr, "Unexpected nil.\n"); //TODO
  } else if (errCode == 9) {
    fprintf(stderr, "Division by zero.\n"); //TODO
  } else if (errCode == 99) {
    fprintf(stderr, "Internal error.\n"); //TODO
  }else{
    fprintf(stderr, "Other unknown error.\n");
  }

  // TODO:
  exit(errCode);
  //vypluj errCode;
}

#endif
/* end of file misc.c */
