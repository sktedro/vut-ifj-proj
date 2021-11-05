#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../scanner.h"

// Calls the scanner until it returns no token. For each token it prints the
// value scanner returned, the token type and attribute data
int main(int argc, char *argv[]){
  Token *token;
  int ret;
  while(1){
    token = NULL;
    ret = scanner(&token);
    printf("Returned <%d> and token: ", ret);
    if(token){
      printf("\ttype=<%d>", token->type);
      if(token->attrib && token->attrib->data){
        printf("\tattribute=<%s>\n", token->attrib->data);
      }
    }else{
      printf("\tNULL\n");
      break;
    }
  }
  return 0;
}
