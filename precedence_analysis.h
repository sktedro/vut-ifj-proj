/*
 * Precedence analysis
 */

#ifndef PRECEDENCE_ANALYSIS_H
#define PRECEDENCE_ANALYSIS_H

#include "char_buffer.h"
#include "misc.h"
#include "scanner.h"
#include "symbol_stack.h"

/**
 * @brief Receives a token and return a new element of the symbol stack
 * 
 * @param token to be converted to SStackElem
 * @return SStackElem* converted from token
 */
SStackElem *parseToken(Token *token);

int precedenceAnalysis(STStack *symtable, Token *token);

#endif
