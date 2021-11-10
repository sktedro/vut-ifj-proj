#include "misc.h"

// Enumeration of the token types
enum TokenTypeEnum{
  t_idOrKeyword, // one token for both identificator and a keyword??
  t_int,
  t_num,
  t_sciNum,
  t_str,
  t_colon,
  t_comma,
  t_rightParen,
  t_leftParen,
  t_arithmOp,
  t_strOp,
  t_relOp,
  t_assignment,
};

// Structure defining a token
typedef struct{
  int type;
  char *data;
} Token;


/**
 * @brief allocates memory for a new token and assigns it a type
 *
 * @param type of the new token
 *
 * @return token (pointer)
 */
Token *tokenInit(int type){
  Token *token = malloc(sizeof(Token));
  if(!token){
    return NULL;
  }
  token->type = type;
  token->data = NULL;
  return token;
}


/**
 * @brief add a new attribute to the token (allocate and write the data)
 *
 * @param token: token to which the new attribute should be added
 * @param data: data which are to be written to the token
 *
 * @return 0 if successful
 */
int tokenAddAttrib(Token *token, char *data){
  if(!token){
    return 1;
  }

  // Allocate space for data
  token->data = malloc(strlen(data) + 1);
  if(!token->data){
    return 1;
  }

  // .. and write the data to the allocated space
  memcpy(token->data, data, strlen(data) + 1);

  return 0;
}

/**
 * @brief Free all memory allocated for the token
 *
 * @param token to destroy
 */
void tokenDestroy(Token *token){
  if(token){
    if(token->data){
      free(token->data);
    }
    free(token);
  }
}
