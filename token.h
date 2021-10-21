#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define ATTRIBUTEINITLEN 16

enum TokenEnum{
  identificator, // one token for both identificator and a keyword??
  keyword,
  expression,
  integer,
  number,
  scientificNumber,
  string,
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
  return token;
}

bool tokenAddAttribute(Token *token, char *data){
  if(!token){
    return false;
  }

  // Attribute to which we will be writing the data
  TokenAttribute *attribute = token->attribute;

  // If the first attribute is not allocated, allocate it now
  if(!attribute){
    attribute = malloc(sizeof(TokenAttribute));
    if(!attribute){
      return false;
    }
  // If there is already at least one attribute, add this one at the end
  }else{
    while(attribute->nextAttribute != NULL){
      attribute = attribute->nextAttribute;
    }
    attribute->nextAttribute = malloc(sizeof(TokenAttribute));
    attribute = attribute->nextAttribute;
    if(!attribute){
      return false;
    }
  }

  //TODO!
  // Allocate space for data
  attribute->data = malloc(strlen(data) + 1);
  if(!attribute->data){
    return false;
  }
  // .. and TODO write the data to the allocated space
  // Or just copy the *data pointer?
  attribute->data = data;
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
