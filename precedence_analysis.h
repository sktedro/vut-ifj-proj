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
SStackElem *parseToken(STStack *symtable, Token *token);

int precedenceAnalysis(STStack *symtable, Token *token);
int unaryOpRule(STStack *symtab, SStack *symstack, SStackElem *op1, SStackElem *op2);

int strLenRule(STStack *symtab, SStack *symstack, SStackElem *op1, SStackElem *op2);

#endif
