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
    return 1;
  }

  // Allocate space for data and write the data to the allocated space
  GCMalloc(token->data, strlen(data) + 1);
  memcpy(token->data, data, strlen(data) + 1);

  return 0;
}

/**
 * @brief Free all memory allocated for the token
 *
 * @param token to destroy
 */
void tokenDestroy(Token **token) {
  if (token) {
    if (*token) {
      if ((*token)->data) {
        free((*token)->data);
      }
      free(*token);
      *token = NULL;
    } else {
      fprintf(stderr, "Warning: you called token destroy and gave me *token = NULL.\n");
    }

  } else {
    fprintf(stderr, "Warning: you called token destroy and gave me token = NULL.\n");
  }
}

#endif
/* end of file token.c */
