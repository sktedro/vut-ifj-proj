/*
 * Parser
 */


#ifndef PARSER_H
#define PARSER_H

#include "misc.h"
#include "precedence_analysis.h"
#include "scanner.h"
#include "symtable.h"
#include "generator.h"

int pStringFunctions(char *varName);

/*
 * Rules
 */

/**
 * @brief Rule for require
 *
 * @return int - 0 if success, else on error
 *
 * 01. <start>           -> require <req> <codeBody>
 */
int pStart();

/**
 * @brief
 *
 * @return error code 
 *
 * 02. <req>             -> "ifj21"
 */
int pReq();

/**
 * @brief Rule for <codebody> string
 *
 * @return int - 0 if success, else on error
 *
 * 04. <codeBody>        -> eps
 * 05. <codeBody>        -> function [id] ( <fnArgList> ) <fnRet> <stat> <ret> end <codeBody>
 * 06. <codeBody>        -> global [id] : function ( <typeList> ) <fnRet> <codeBody>
 * 07. <codeBody>        -> [id] <fnCall> <codeBody>
 */
int pCodeBody();

/**
 * @brief
 *
 * @return error code
 *
 * 09. <fnCall>          -> ( <fnCallArgList> )
 */
int pFnCall();

/**
 * @brief
 *
 * @return error code
 *
 * 10. <fnRet>           -> eps
 * 11. <fnRet>           -> : <type> <nextType>
 */
int pFnRet();

/**
 * @brief
 *
 * @return error code
 *
 * 12. <fnCallArgList>   -> eps
 * 13. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
 */
int pFnCallArgList();

/**
 * @brief
 *
 * @return error code
 *
 * 14. <nextFnCallArg>   -> eps
 * 15. <nextFnCallArg>   -> , <fnCallArg> <nextFnCallArg>
 */
int pNextFnCallArg();

/**
 * @brief
 *
 * @return error code
 *
 * 16. <fnCallArg>       -> [id]
 * 17. <fnCallArg>       -> [literal]
 */
int pFnCallArg();

/**
 * @brief
 *
 * @return error code
 *
 * 19. <ret>             -> eps
 * 20. <ret>             -> return <retArgList>
 */
int pRet();

/**
 * @brief
 *
 * @return error code
 *
 * 22. <stat>            -> eps
 * 23. <stat>            -> [id] <statWithId> <stat>
 * 24. <stat>            -> local <idList> : <type> <newIdAssign> <stat>
 * 25. <stat>            -> if <expr> then <stat> else <stat> end <stat>
 * 26. <stat>            -> while <expr> do <stat> end <stat>
 */
int pStat();

/**
 * @brief
 *
 * @return error code
 *
 * 27. <statWithId>      -> , [id] <nextAssign> <expr> , <expr>
 * 28. <statWithId>      -> = <expr>
 * 29. <statWithId>      -> <fnCall>
 */
int pStatWithId();

/**
 * @brief
 *
 * @return error code
 *
 * 31. <nextAssign>  -> , [id] <nextAssign> <expr> ,
 * 32. <nextAssign>  -> =
 */
int pNextAssign();

/**
 * NOT USED
 *
 * @brief Function for built in functions
 *
 * @return error code
 *  TODO ADD MORE CFG RULES FOR ALL FUNCTIONS
 * TODO ADD IT TO CFG !!!!!!!!!!!!!!!!
 */
int builtInFunctions();

/**
 * @brief
 *
 * @return error code
 *
 * 34. <fnArgList>       -> eps
 * 35. <fnArgList>       -> [id] : <type> <nextFnArg>
 */
int pFnArgList(char *fnName);

/**
 * @brief
 *
 * @return error code
 *
 * 36. <nextFnArg>       -> eps
 * 37. <nextFnArg>       -> , [id] : <type> <nextFnArg>
 */
int pNextFnArg();

/**
 * @brief
 *
 * @return error code
 *
 * 39. <retArgList>      -> eps
 * 40. <retArgList>      -> <expr> <retNextArg>
 */
int pRetArgList();

bool isExpressionParser(Token token);

/**
 * @brief
 *
 * @return error code
 *
 * 41. <retNextArg>      -> eps
 * 42. <retNextArg>      -> , <expr> <retNextArg>
 */
int pRetNextArg();

/**
 * @brief
 *
 * @return error code
 *
 * 44. <typeList>        -> eps
 * 45. <typeList>        -> integer <nextType>
 * 46. <typeList>        -> number <nextType>
 * 47. <typeList>        -> string <nextType>
 * 48. <typeList>        -> nil <nextType>
 */
int pTypeList();


/**
 * @brief
 *
 * @return error code
 *
 * 50. <nextType>        -> eps
 * 51. <nextType>        -> , <type> <nextType>
 */
int pNextType();


int pFnParamTypeList(char *fnName);

int pNextParamType(char *fnName);

int pFnRetTypeList(char *fnName);

int nextRetType(char *fnName);

/**
 * @brief
 *
 * @return error code
 *
 * 53. <type>            -> integer
 * 54. <type>            -> number
 * 55. <type>            -> string
 * 56. <type>            -> nil
 */
// NOT IN USE, THIS WILL BE DELETED 
//int pType(char *fnName, char *varName, bool isFnCalled);

int typeFnDeclaration(char *fnName);
int typeFnCall(char *fnName);
int typeVar(char *varName);

/**
 * @brief
 *
 * @return error code
 *
 * 58. <idList>          -> [id] <nextId>
 */
int pIdList();

/**
 * @brief
 *
 * @return error code
 *
 * 59. <nextId>          -> eps
 * 60. <nextId>          -> , [id] <nextId>
 */
int pNextId();

/**
 * @brief
 *
 * @return error code
 *
 * 62. <newIdAssign>     -> eps
 * 63. <newIdAssign>     -> = <exprList>
 */
int pNewIdAssign();

/**
 * @brief
 *
 * @return error code
 *
 * 65. <exprList>        -> <expr> <nextExpr>
 */
int pExprList();

/**
 * @brief
 *
 * @return error code
 *
 * 66. <nextExpr>        -> eps
 * 67. <nextExpr>        -> , <expr> <nextExpr>
 */
int pNextExpr();

//-----------------------------------

/**
 * @brief
 *
 * @return error code
 */
int pExpr();

/*
 *
 * HELPER FUNCTIONS
 *
 */

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

/** TODO do we ever use this?
 * @brief returns value of data type in IFJ21DataTypes
 *
 * @param string
 *
 * @return return value in range <0, 3> if it is in datastructure, else -1
 */
int getDataTypeInt(char *data);

/**
 * @brief Check if string is built in function
 *
 * @return if string is built in function return true, else false
 */
bool isBuiltInFunction(char *data);

/**
 * @brief Check if token is keyword
 *
 * @return if token is keyword return true, else false
 */
bool isKeyword(Token *token);

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
