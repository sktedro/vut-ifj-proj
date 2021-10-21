#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"

int main(int argc, char *argv[]){
  Token *token = NULL;
  int ret;
  while(1){
    ret = scanner(&token);
    printf("returned %d while processing token:\n", ret);
    if(token){
      printf("token type: %d\n", token->type);
    }else{
      printf("no token was returned\n");
      break;
    }
  }
  return 0;
}
