/*
 * Operations with a token structure
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "misc.h"


/**
 * @brief allocates memory for a new token and assigns it a type
 *
 * @param type of the new token
 *
 * @return token (pointer)
 */
Token *tokenInit(int type);

/**
 * @brief add a new attribute to the token (allocate and write the data)
 *
 * @param token: token to which the new attribute should be added
 * @param data: data which are to be written to the token
 *
 * @return 0 if successful
 */
int tokenAddAttrib(Token *token, char *data);

/**
 * @brief Free all memory allocated for the token
 *
 * @param token to destroy
 */
void tokenDestroy(Token *token);


#endif
/* end of file token.h */
