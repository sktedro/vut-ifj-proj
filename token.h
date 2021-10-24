#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define ATTRIBUTEINITLEN 16

enum TokenEnum{
  t_idOrKeyword, // one token for both identificator and a keyword??
  t_expression,
  t_integer,
  t_number,
  t_scientificNumber,
  t_string,
};

// Do we need more attributes in one token? If not, this structure is redundant
// and in Token struct, there should be just char data instead of
// TokenAttribute
typedef struct TokenAttribute TokenAttribute;
struct TokenAttribute{
  char *data;
  TokenAttribute *nextAttribute;
};

typedef struct{
  int type;
  TokenAttribute *attribute;
} Token;


Token *tokenInit(int type){
  Token *token = malloc(sizeof(Token));
  if(!token){
    return NULL;
  }
  token->type = type;
  token->attribute = NULL;
  return token;
}

bool tokenAddAttribute(Token *token, char *data){
  if(!token){
    return false;
  }

  

  // Allocate an attribute
  TokenAttribute *newAttribute = malloc(sizeof(TokenAttribute));
  if(!newAttribute){
    return false;
  }

  //TODO!
  // Allocate space for data
  newAttribute->data = malloc(strlen(data) + 1);
  if(!newAttribute->data){
    return false;
  }
  newAttribute->nextAttribute = NULL;

  // .. and write the data to the allocated space
  memcpy(newAttribute->data, data, strlen(data) + 1);
  // Or just copy the *data pointer?
  // attribute->data = data;

  token->attribute = newAttribute;
/*   // TODO!!!
 * 
 *   // Attribute to which we will be writing the data
 *   TokenAttribute *attribute = token->attribute;
 * 
 *   // If there is already at least one attribute, add this one at the end
 *   if(attribute){
 *     while(attribute->nextAttribute != NULL){
 *       attribute = attribute->nextAttribute;
 *     }
 *     // attribute->nextAttribute = malloc(sizeof(TokenAttribute));
 *     attribute->nextAttribute = newAttribute;
 *     if(!attribute){
 *       return false;
 *     }
 *   }
 */




  return true;
}

void tokenAttributeDestroy(TokenAttribute *attribute){
  if(attribute->nextAttribute){
    tokenAttributeDestroy(attribute->nextAttribute);
  }
  free(attribute->data);
  free(attribute);
}

void tokenDestroy(Token *token){
  if(token){
    if(token->attribute){
      tokenAttributeDestroy(token->attribute);
    }
    free(token);
  }
}
