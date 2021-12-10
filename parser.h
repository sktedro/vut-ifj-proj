/**
 * VUT FIT - IFJ 2021
 *
 * @file parser.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef PARSER_H
#define PARSER_H

#include "misc.h"
#include "scanner.h"
#include "symtable.h"
#include "generator.h"
#include "linked_list.h"
#include "precedence_analysis.h"

/*
 *
 * Rules 
 *
 */

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 01. <start>                      -> require <req> <codeBody>
 *
 * @return 0 if successful, errcode otherwise
 */
int pStart() ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 02. <req>                        -> "ifj21"
 *
 * @return 0 if successful, errcode otherwise
 */
int pReq() ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 04. <codeBody>        -> eps
 * 05. <codeBody>        -> function [id] ( <fnDefinitionParamTypeList> ) 
 *   <fnRetTypeList> <stat> end <codeBody>
 * 06. <codeBody>        -> global [id] : function ( 
 *   <fnDeclarationParamTypeList> ) <fnRetTypeList> <codeBody>
 * 07. <codeBody>        -> [id] <fnCall> <codeBody>
 *
 * @return 0 if successful, errcode otherwise
 */
int pCodeBody() ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 09. <fnCall>          -> ( <fnCallArgList> )
 *
 * @param fnName: name of the function which is being called
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnCall(char *fnName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 10. <fnCallArgList>   -> eps
 * 11. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
 *
 * @param fnName: name of the function which is being called
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnCallArgList(char *fnName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 14. <fnCallArg>       -> [id]
 * 15. <fnCallArg>       -> [literal]
 *
 * @param fnName: name of the function which is being called
 * @param argCount: number of arguments already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnCallArg(char *fnName, int argCount) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 12. <nextFnCallArg>   -> eps
 * 13. <nextFnCallArg>   -> , <fnCallArg> <nextFnCallArg>
 *
 * @param fnName: name of the function which is being called
 * @param argCount: number of arguments already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int pNextFnCallArg(char *fnName, int argCount) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 17. <stat>            -> eps
 * 18. <stat>            -> [id] <statWithId> <stat>
 * 19. <stat>            -> local [id] : [type] <newIdAssign> <stat>
 * 20. <stat>            -> if <expr> then <stat> else <stat> end <stat>
 * 21. <stat>            -> while <expr> do <stat> end <stat>
 * 22. <stat>            -> return <retArgList> <stat> 
 *
 * @param fnName: name of the function in which the statements are
 *
 * @return 0 if successful, errcode otherwise
 */
int pStat(char *fnName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 23. <statWithId>      -> , [id] <nextAssign> <expr>
 * 24. <statWithId>      -> = <expr>
 * 25. <statWithId>      -> <fnCall>
 *
 * @param idName: name of the id that is at the beginning of the statement
 *
 * @return 0 if successful, errcode otherwise
 */
int pStatWithId(char *idName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 26. <nextAssign>      -> , [id] <nextAssign> <expr>
 * 27. <nextAssign>      -> =
 *
 * @param assignmentDone: true if we already received enough values to fill the
 * id list on the left side of the multiassignment
 * @param endLabel: where to jump after the multiassignment is done
 *
 * @return 0 if successful, errcode otherwise
 */
int pNextAssign(bool *assignmentDone, char *endLabel) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 29. <fnDefinitionParamTypeList>       -> eps
 * 30. <fnDefinitionParamTypeList>       -> [id] : [type] <nextFnDefinitionParamType>
 *
 * @param fnName: name of the function of which param types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnDefinitionParamTypeList(char *fnName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 31. <nextFnDefinitionParamType>       -> eps
 * 32. <nextFnDefinitionParamType>       -> , [id] : [type] <nextFnDefinitionParamType>
 *
 * @param fnName: name of the function of which param types we are processing
 * @param paramCount: number of parameters already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int pNextFnDefinitionParamType(char *fnName, int paramCount) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 34. <retArgList>      -> eps
 * 35. <retArgList>      -> <expr> <retNextArg>
 *
 * @param fnName: name of the function of which ret types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pRetArgList(char *fnName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 36. <retNextArg>      -> eps
 * 37. <retNextArg>      -> , <expr> <retNextArg>
 *
 * @param fnName: name of the function of which ret types we are processing
 * @param argCount: the amount of ret arguments already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int pRetNextArg(char *fnName, int argCount) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 39. <fnDeclarationParamTypeList> -> eps
 * 40. <fnDeclarationParamTypeList> -> [type] <nextFnDeclarationParamType>
 *
 * @param fnName: name of the function of which param types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnDeclarationParamTypeList(char *fnName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 41. <nextFnDeclarationParamType>   -> eps
 * 42. <nextFnDeclarationParamType>   -> , [type] <nextFnDeclarationParamType>
 *
 * @param fnName: name of the function of which param types we are processing
 * @param paramCount: number of parameters already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int pNextFnDeclarationParamType(char *fnName, int paramCount) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 44. <fnRetTypeList>   -> eps 
 * 45. <fnRetTypeList>   -> : [type] <nextRetType>
 *
 * @param fnName: name of the function of which ret types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnRetTypeList(char *fnName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 46. <pNextRetType>     -> eps
 * 47. <pNextRetType>     -> , [type] <nextRetType>
 *
 * @param fnName: name of the function of which ret types we are processing
 * @param retVarCounter: number of return values already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int pNextRetType(char *fnName, int *retVarCounter) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 49. <newIdAssign>     -> eps
 * 50. <newIdAssign>     -> = <expr>
 *
 * @param varName: name of the variable to which we are assigning
 *
 * @return 0 if successful, errcode otherwise
 */
int pNewIdAssign(char *varName) ForceRetUse;

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * <expr>
 *
 * @param retVarName: destination pointer for the expression result
 * @param expectedType: data type the result of the expression should be
 *
 * @return 0 if successful, errcode otherwise
 */
int pExpr(char **retVarName, int expectedType) ForceRetUse;

/*
 *
 * Helper functions
 *
 */

/** 
 * @brief Process an expression (or a function call) for multiassignment
 *
 * @param assignmentDone: pointer that is set to true if we received enough
 * return values from expressions and function calls on the right side to fill
 * all ids in the id list (left side)
 * @param endLabel: name of the label generated after the assignment
 *
 * @return 0 if successful, errcode otherwise
 */
int processExpr(bool *assignmentDone, char *endLabel) ForceRetUse;

/**
 * @brief Appends a variables name (in ifjcode) to the declaration list so it
 * can be declared later (to avoid multiple declarations)
 *
 * @param name of the variable in ifjcode
 *
 * @return err code
 */
int appendToVarDeclarationList(char *name);

/**
 * @brief Check if a string represents a data type
 *
 * @param data: input string
 *
 * @return true if the string represents a data type
 */
bool isDataType(char *data);

/**
 * @brief If the function wasn't already defiend, define it (add it to the
 * symtable)
 *
 * @param token containing the name of the function
 *
 * @return 0 if successful, errcode otherwise
 */
int newFunctionDefinition(Token *token) ForceRetUse;

/**
 * @brief If the function wasn't already declared, declare it (add it to the
 * symtable)
 *
 * @param fnName: name of the function
 *
 * @return 0 if successful, errcode otherwise
 */
int newFunctionDeclaration(char *fnName) ForceRetUse;

/**
 * @brief Returns true if the token represents a literal (integer, number or a
 * string)
 *
 * @param token
 *
 * @return true if the token is a literal
 */
bool isLiteral(Token *token);

/**
 * @brief Process a data type of a return value of a function (when declaring
 * or defining it)
 *
 * @param fnName: name of the function
 * @param retVarCounter: number of return values already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int fnRetDataType(char *fnName, int *retVarCounter) ForceRetUse;

/**
 * @brief Process a data type of a parameter of a function when declaring it
 *
 * @param fnName: name of the function
 * @param data: data type in a string
 *
 * @return 0 if successful, errcode otherwise
 */
int fnDeclarationParamType(char *fnName, char *data) ForceRetUse;

/**
 * @brief Process a data type of a parameter of a function when defining it
 *
 * @param fnName: name of the function
 * @param paramCount: the counter of parameters
 *
 * @return 0 if successful, errcode otherwise
 */
int fnDefinitionParamType(char *fnName, int paramCount) ForceRetUse;

/**
 * @brief Process data type of a variable when declaring or defining it
 *
 * @param varName: name of the variable
 *
 * @return 0 if successful, errcode otherwise
 */
int varDataType(char *varName) ForceRetUse;

/**
 * @brief Define all parameters needed so we can successfully pass the
 * parameters to a function when calling it (define a 'param' variable to which
 * the parameter will be passed and generate code to assign the value of this
 * variable to the parameter name)
 *
 * @param fnName
 *
 * @return 0 if successful, errcode otherwise
 */
int createParamVariables(char *fnName) ForceRetUse;

#endif
/* end of file parser.h */
