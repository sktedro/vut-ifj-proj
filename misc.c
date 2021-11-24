/*
 * Enumerators, structure definitions and miscellaneous functions
 */

#ifndef MISC_C
#define MISC_C

#include "misc.h"

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
int err(int errCode) {
  if (errCode == 1) {
    fprintf(stderr, "Lexical analysis error.\n"); //TODO
  } else if (errCode == 99) {
    fprintf(stderr, "Internal error.\n"); //TODO
  }else{
    fprintf(stderr, "Other unknown error.\n");
  }
  vypluj errCode;
}

#endif
/* end of file misc.c */
