/*
 * Operations with a token structure
 */

#ifndef TOKEN_C
#define TOKEN_C

#include "token.h"

extern int ret;

/**
 * @brief allocates memory for a new token and assigns it a type
 *
 * @param token: destination pointer
 * @param type of the new token
 *
 * @return 0 if successful, errcode otherwise
 */
int tokenInit(Token **token, int type) {
  GCMalloc(*token, sizeof(Token));
  (*token)->type = type;
  (*token)->data = NULL;
  return 0;
}

/**
 * @brief add a new attribute to the token (allocate and write the data)
 *
 * @param token: token to which the new attribute should be added
 * @param data: data which are to be written to the token
 *
 * @return 0 if successful, errcode otherwise
 */
int tokenAddAttrib(Token *token, char *data) {
  if (!token) {
    return ERR(INTERN_ERR);
  }

  // Allocate space for data and write the data to the allocated space
  GCMalloc(token->data, strlen(data) + 1);
  memcpy(token->data, data, strlen(data) + 1);

  return 0;
}

/** 
 * @brief This function is for debug print of a token
 *
 * @param token
 */
void printToken(Token *token) {
  if(!DEBUGTOGGLE){
    return;
  }
  char *type;
  switch (token->type) {
    case t_idOrKeyword:
      type = "idOrKeyword";
      break;
    case t_colon:
      type = "colon";
      break;
    case t_rightParen:
      type = "rightParen";
      break;
    case t_comma:
      type = "comma";
      break;
    case t_arithmOp:
      type = "arithmeticOperation";
      break;
    case t_assignment:
      type = "assigment";
      break;
    case t_int:
      type = "integer";
      break;
    case t_leftParen:
      type = "leftParen";
      break;
    case t_num:
      type = "num";
      break;
    case t_relOp:
      type = "relationOperation";
      break;
    case t_strOp:
      type = "stringOperation";
      break;
    case t_sciNum:
      type = "scientificNumber";
      break;
    case t_str:
      type = "string";
      break;
    default :
      fprintf(stderr, "TOKEN IS NULL\n");
      return;
  }
  fprintf(stderr, "Token type : %s, Token data : <%s>\n", type, token->data);
}

#endif
/* end of file token.c */
