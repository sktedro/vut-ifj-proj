#include <stdio.h>
#include <string.h>
#include "../../token.h"

int main(){
  // Init:
  Token *intToken, *relOpToken;
  tokenInit(&intToken, t_int);
  tokenInit(&relOpToken, t_relOp);
  if(!intToken || !relOpToken){
    printf("Tokens could not be initialized!\n");
    return 1;
  }
  if(intToken->type != t_int){
    printf("Int token has a wrong type assigned (or none)!\n");
    return 1;
  }
  if(relOpToken->type != t_relOp){
    printf("Relation operator token has a wrong type assigned (or none)!\n");
    return 1;
  }
  if(intToken->data || relOpToken->data){
    printf("A token has an attribute after initialization!\n");
    return 1;
  }

  char str[] = "abcdsa";
  // Add an attribute
  tokenAddAttrib(intToken, str);
  if(!intToken || !intToken->data){
    printf("Adding an attribute was unsuccessful!\n");
    return 1;
  }
  if(strcmp(intToken->data, str)){
    printf("An attribute was added but its content is wrong! ('%s' vs '%s')\n", intToken->data, str);
    return 1;
  }
  return 0;
}
