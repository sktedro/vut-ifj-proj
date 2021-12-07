/*
 * Parser
 */


#ifndef PARSER_H
#define PARSER_H

#include "misc.h"
#include "scanner.h"
#include "symtable.h"
#include "generator.h"
#include "assignment.h"
#include "precedence_analysis.h"

int pStringFunctions(char *varName);

/*
 * Rules
 */

int pStart();
int pReq();
int pCodeBody();
int pFnCall();
int pFnRet();
int pFnCallArgList();
int pNextFnCallArg();
int pFnCallArg();
int pStat();
int pStatWithId();
int pNextAssign();
int pFnDefinitionParamTypeList(char *fnName);
int pNextFnDefinitionParamType(char *fnName, int paramCount);
int pRetArgList(char *fnName);
int pRetNextArg(char *fnName, int argCount);
int pFnDeclarationParamTypeList(char *fnName);
int pNextFnDeclarationParamType(char *fnName, int paramCount);
int pFnRetTypeList(char *fnName);
int pNextRetType(char *fnName);
int pNewIdAssign(char *varName);
int pExpr();

/*
 *
 * HELPER FUNCTIONS
 *
 */
int writeFunction();
int typeFnDeclaration(char *fnName);
int fnDefinitionParamType(char *fnName, int counter);
int typeVar(char *varName);
int createParamVariables(char *fnName);
int varDataType(char *varName);
int fnDeclarationParamType(char *fnName, char* data);
int fnRetDataType(char *fnName);

/**
 * @brief Check if a string represents a data type
 *
 * @param data: input string
 *
 * @return true if the string represents a data type
 */
bool isDataType(char *data);

/**
 * @brief check if token is read function
 * 
 * @param data 
 * @return true if data is read function
 * @return false otherwise
 */
bool isReadFunction(char *data);

/**
 * @brief check if token is string operation function
 * 
 * @param data 
 * @return true if data is string operation function
 * @return false otherwise
 */
bool isStringOperationFunction(char *data);

/**
 * @brief check if token is read function in ifj21
 * 
 * @param token 
 * @return true if it is read function, destroys () behind read, generates code
 * 
 */
bool readFunction(Token *token);

/**
 * @brief Check if string is built in function
 *
 * @return if string is built in function return true, else false
 */
bool isBuiltInFunction(char *data);

/**
 * @brief If the function wasn't already defiend, define it (add it to the
 * symtable)
 *
 * @param fnName: name of the function
 *
 * @return error code
 */
int newFunctionDefinition(char *fnName);

/**
 * @brief If the function wasn't already declared, declare it (add it to the
 * symtable)
 *
 * @param fnName: name of the function
 *
 * @return error code
 */
int newFunctionDeclaration(char *fnName);

#endif
/* end of file parser.h */
