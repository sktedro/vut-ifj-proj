#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"

int main(int argc, char *argv[]){
  Token *token;
  int ret;
  while(1){
    token = NULL;
    ret = scanner(&token);
    printf("returned %d while processing token:\n", ret);
    if(token){
      printf("token type: %d\n", token->type);
      if(token->attribute){
        if(token->attribute->data){
          printf("attribute data: %s\n", token->attribute->data);
        }
      }
    }else{
      printf("no token was returned\n");
      break;
    }
  }
  return 0;
}
