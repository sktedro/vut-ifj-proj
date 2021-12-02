/*
 * Operations with a token structure
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "misc.h"

/**
 * @brief allocates memory for a new token and assigns it a type
 *
 * @param token: destination pointer
 * @param type of the new token
 *
 * @return 0 if successful, errcode otherwise
 */
int tokenInit(Token **token, int type) ForceRetUse;

/**
 * @brief add a new attribute to the token (allocate and write the data)
 *
 * @param token: token to which the new attribute should be added
 * @param data: data which are to be written to the token
 *
 * @return 0 if successful, errcode otherwise
 */
int tokenAddAttrib(Token *token, char *data) ForceRetUse;

/**
 * @brief This function is for debug print of a token
 *
 * @param token
 */
void printToken(Token *token);

#endif
/* end of file token.h */
