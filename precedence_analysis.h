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


#define TRYRULE(FN, ...)                  \
    rulesRet = FN(symstack, __VA_ARGS__); \
    if(rulesRet != -1){                   \
      return rulesRet;                    \
    }


/**
 * @brief A precedence analysis algorithm. Parses an expression, checks for
 * errors and generates code
 *
 * @param symtab: symbol table containing variables and functions
 * @param token: first token of the expression
 * @param returnVarName: pointer to a string where this function writes the 
 * name of the variable where the expression result is stored (in the generated
 * code)
 *
 * @return 0 if successful, errcode otherwise 
 */
int parseExpression(STStack *symtab, Token *token, char **returnVarName) 
  ForceRetUse;

/**
 * @brief Reduce step of the precedence analysis - reduce an atomic expression
 * that is on top of the stack - checks if a rule for the expression exists and
 * generates code if it does
 *
 * @param symstack: symbol stack
 *
 * @returns 0 if successful, errcode otherwise 
 */
int reduceStep(SStack *symstack) ForceRetUse;


/*
 * Rule functions
 */


/**
 * @brief Check rules for symbols on the stack that need reducing
 *
 * @param symstack: symbol stack
 * @param opSymbols: amount of symbols that are to be reduces (eg. 2 for #str)
 *
 * @return 0 if successful, errcode otherwise 
 */
int checkRules(SStack *symstack, int opSymbols) ForceRetUse;

/**
 * @brief "i rule" - reduce i to E -just convert the i to E and push it back 
 * to the stack
 *
 * @param symstack: symbol stack
 * @param op: the symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 */
int iRule(SStack *symstack, SStackElem *op) ForceRetUse;

/**
 * @brief "#E rule" - reduce "#E" to "E" - generate code for strlen
 *
 * @param symstack: symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 */
int strLenRule(SStack *symstack, SStackElem *op1, SStackElem *op2) ForceRetUse;

/**
 * @brief "(E) rule" - destroy the parantheses and push the E back
 *
 * @param symstack: symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 * @param op3: third symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 */
int bracketsRule(SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) ForceRetUse;

/**
 * @brief "arithmetic operator rule" - reduces a arithmetic operation - 
 * addition, subtraction, multiplication, division, integer division and 
 * concatenation (+ - * / // ..) - check the data types and everything and 
 * generate code
 *
 * @param symstack - symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 * @param op3: third symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 *
 * pt_concat only works with two strings
 * pt_{mult, add, sub} work with integers and numbers
 * pt_div works with numbers and returns a number
 * pt_intDiv works with integers and returns an integer
 */
int arithmeticOperatorsRule(SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) ForceRetUse;

/**
 * @brief "relational operator rule" - reducing relational expressions - 
 * == ~= < > <= >= - check the data types and everything and generate code
 *
 * @param symstack - symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 * @param op3: third symbol to be reduced
 *
 * @return 0 if successful, -1 if no rule was found, errcode otherwise 
 */
int relationalOperatorsRule(SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) ForceRetUse;


/*
 * Rule helper functions
 */


/**
 * @brief Pop 'amount' of symbols from the stack and pass it down by
 * parameters. Also pop the '<' beneath them
 *
 * @param symstack: symbol stack
 * @param op1: first operand
 * @param op2: operator
 * @param op3: second operand
 * 
 * @return 0 if successful, errcode otherwise
 */
int getSymbolsForReduction(SStack *symstack, SStackElem **op1, 
    SStackElem **op2, SStackElem **op3, int amount) ForceRetUse;

/**
 * @brief If an operand is not an integer, convert it (generate code)
 *
 * @param op that should be an integer
 *
 * @return 0 if successful, errcode otherwise 
 */
int ensureInteger(SStackElem *op) ForceRetUse;

/**
 * @brief If an operand is not a number, convert it (generate code)
 *
 * @param op that should be a number
 *
 * @return 0 if successful, errcode otherwise 
 */
int ensureNumber(SStackElem *op) ForceRetUse;

/**
 * @param Check data types of operators of a binary operation (and convert them
 * if necessary)
 *
 * @param op1: first operand
 * @param op2: operator
 * @param op3: second operand
 *
 * @return 0 if successful, errcode otherwise 
 */
int checkDataTypesOfBinOps(SStackElem *op1, SStackElem *op2, 
    SStackElem *op3) ForceRetUse;


/*
 *
 * Creating and destroying symbols
 *
 */


/**
 * @brief Receives a token and return a new symbol of the symbol stack
 *
 * @param symtab: the symbol table 
 * @param token to be converted to SStackElem
 * @param newSymbol: destination pointer
 *
 * @return 0 if successful, errcode otherwise 
 */
int parseToken(STStack *symtab, Token *token, SStackElem **newSymbol) 
  ForceRetUse;

/**
 * @brief Allocates a new symbol and initializes it (a symbol to be inserted to 
 * the symbol stack)
 *
 * @param newSymbol: destination pointer
 * @param symbol: enum value of symbol stack type 
 *
 * @return 0 if successful, errcode otherwise 
 */
int allocateSymbol(SStackElem **newSymbol, int symbol) ForceRetUse;

/**
 * @brief Allocates a new symbol and sets its data based on parameters
 *
 * @param newSymbol: destination pointer
 * @param type
 * @param op: precedence table enum
 * @param isId: bool
 * @param dataType: data type enum
 * @param data: string representing the symbol
 *
 * @return 0 if successful, errcode otherwise 
 */
int createSymbol(SStackElem **newSymbol, int type, int op, bool isId, 
    int dataType, char *data) ForceRetUse;


/*
 *
 * Helper functions
 *
 */


/**
 * @brief Init before the precedence analysis begins. Init the symbol stack,
 * push '$' to it, check if we received a token from the parser and check if
 * the token is a function (we don't parse functions here)
 *
 * @param symtab: symbol table
 * @param symstack: where to save the initialized symbol stack
 * @param token: the token received from the parser
 *
 * @return 0 if successful, errcode otherwise 
 */
int precedenceAnalysisInit(STStack *symtab, SStack **symstack, Token **token) 
  ForceRetUse;

/**
 * @brief If we're still receiving tokens that are a part of the expression, we
 * fetch it in this function and convert it into a symbol. Otherwise the new
 * symbol is just a 'reduce' symbol. Also sets the exprCanEnd to false if the
 * last symbol was an operator (and vice versa)
 *
 * @param symtab: symbol table
 * @param token: destination pointer for the token
 * @param inputSymbol: destination pointer for the new symbol
 * @param fetchNewToken: boolean representing if we're (not) done fetching tokens
 * @param exprCanEnd
 * @param exprEnd
 *
 * @returns 0 if successful, errcode otherwise 
 */
int getNewSymbol(STStack *symtab, Token **token, SStackElem **inputSymbol, 
    bool fetchNewToken, bool *exprCanEnd, bool *exprEnd) ForceRetUse;

/**
 * @brief Get a new token and set exprEnd to true if the new token is not a 
 * part of the expression
 *
 * @param token: address where the new token should be saved
 * @param exprCanEnd
 * @param exprEnd
 *
 * @return 0 if successful, errcode otherwise 
 */
int fetchNewToken(Token **token, bool exprCanEnd, bool *exprEnd) ForceRetUse;

/**
 * @brief Check if a a symbol represents a zero literal (an int or a num)
 *
 * @param operand as a symbol from the symbol stack
 *
 * @return true if the number is a zero
 */
bool isZero(SStackElem *operand);

/**
 * @brief returns true if a symbol is an binary arithmetic operator
 *
 * @param op: symbol
 *
 * @return true if a symbol is an binary arithmetic operator
 */
bool isBinArithmOp(SStackElem *op);

/**
 * @brief check if the token is allowed to be a part of the expression
 *
 * @param token
 *
 * @returns true if token is allowed in the expression
 */
bool isTokenAllowedInExpr(Token *token);

/**
 * @brief checks if the token is an ID or a literal
 *
 * @param token
 *
 * @return true if the token is an ID or a literal
 */
bool isTokenIdOrLiteral(Token *token);

/**
 * @brief Checks if there is only $E in the stack
 *
 * @param symstack: symbol stack
 *
 * @returns true if there is only $E in the stack
 */
bool isExprAtomic(SStack *symstack);

/**
 * @brief Simple debugging function to print all elements on the symbol stack
 *
 * @param stack to print out
 */
void debugPrint(SStack *stack);

#endif
/* end of file precedence_analysis.h*/
