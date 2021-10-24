#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef misc
#define misc

//   ♥    //
#define vypluj return

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
