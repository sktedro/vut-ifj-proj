#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

enum TokenEnum{
  t_idOrKeyword, // one token for both identificator and a keyword??
  t_int,
  t_num,
  t_sciNum,
  t_str,
  t_rightParen,
  t_leftParen,
  t_arithmOp,
  t_strOp,
  t_relOp,
  t_assignment,
};

// Do we need more attributes in one token? If not, this structure is redundant
// and in Token struct, there should be just char data instead of
// TokenAttrib
typedef struct TokenAttrib TokenAttrib;

struct TokenAttrib{
  char *data;
  TokenAttrib *nextAttrib;
};

typedef struct{
  int type;
  TokenAttrib *attrib;
} Token;


Token *tokenInit(int type){
  Token *token = malloc(sizeof(Token));
  if(!token){
    return NULL;
  }
  token->type = type;
  token->attrib = NULL;
  return token;
}

bool tokenAddAttrib(Token *token, char *data){
  if(!token){
    return false;
  }

  

  // Allocate an attribute
  TokenAttrib *newAttrib = malloc(sizeof(TokenAttrib));
  if(!newAttrib){
    return false;
  }

  //TODO
  // Allocate space for data
  newAttrib->data = malloc(strlen(data) + 1);
  if(!newAttrib->data){
    return false;
  }
  newAttrib->nextAttrib = NULL;

  // .. and write the data to the allocated space
  memcpy(newAttrib->data, data, strlen(data) + 1);

  token->attrib = newAttrib;

/*   // TODO
 *   // How to write to the nextAttrib??
 *   // Might not be needed though, so it's ok for now
 * 
 *   // Attrib to which we will be writing the data
 *   TokenAttrib *attrib = token->attrib;
 * 
 *   // If there is already at least one attrib, add this one at the end
 *   if(attrib){
 *     while(attrib->nextAttrib != NULL){
 *       attrib = attrib->nextAttrib;
 *     }
 *     // attrib->nextAttrib = malloc(sizeof(TokenAttrib));
 *     attrib->nextAttrib = newAttrib;
 *     if(!attrib){
 *       return false;
 *     }
 *   }
 */




  return true;
}

void tokenAttribDestroy(TokenAttrib *attrib){
  if(attrib->nextAttrib){
    tokenAttribDestroy(attrib->nextAttrib);
  }
  free(attrib->data);
  free(attrib);
}

void tokenDestroy(Token *token){
  if(token){
    if(token->attrib){
      tokenAttribDestroy(token->attrib);
    }
    free(token);
  }
}
