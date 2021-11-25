/*
 * Precedence analysis
 */

#ifndef PRECEDENCE_ANALYSIS_H
#define PRECEDENCE_ANALYSIS_H

#include "char_buffer.h"
#include "misc.h"
#include "scanner.h"
#include "symbol_stack.h"
#include "symtable.h"
#include "parser.h"
#include "generator.h"

/**
 * @brief Receives a token and return a new element of the symbol stack
 * 
 * @param token to be converted to SStackElem
 * @return SStackElem* converted from token
 */
SStackElem *parseToken(STStack *symtable, Token *token);

int parseExpression(STStack *symtab, Token *token, char **returnVarName);


int iRule(SStack *symstack, SStackElem *op);
int strLenRule(SStack *symstack, SStackElem *op1, SStackElem *op2);
int bracketsRule(SStack *symstack, SStackElem *op1,
    SStackElem *op2, SStackElem *op3);
int arithmeticOperatorsRule(SStack *symstack, SStackElem *op1,
    SStackElem *op2, SStackElem *op3);
int relationalOperatorsRule(SStack *symstack, SStackElem *op1,
    SStackElem *op2, SStackElem *op3);



#endif
