/**
 * VUT FIT - IFJ 2021
 *
 * @file parser.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef PARSER_C
#define PARSER_C

#include "parser.h"

extern int ret;

STStack *symtab;

// Used to define all variables of a function at its end
DynamicStringArray *varDefBuf = NULL;

// Used for multi-assignments
LinkedList *multiAssignIdList;
LinkedList *multiAssignRetList;
DynamicStringArray *labelList;


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
int pStart() {
  RuleFnInit;

  // Initialize the dynamic array for variable declaration ('VARDEF')
  TryCall(dynStrArrInit, &varDefBuf);

  // Initialize the data structures for the multiassignment
  TryCall(LLInit, &multiAssignIdList);
  TryCall(LLInit, &multiAssignRetList);
  TryCall(dynStrArrInit, &labelList);

  // require
  RequireToken(t_idOrKeyword, "require");

  // <req>
  TryCall(pReq);

  // Generate code for all built in functions
  genBuiltInFunctions();

  // <codeBody>
  TryCall(pCodeBody);

  vypluj 0;
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 02. <req>                        -> "ifj21"
 *
 * @return 0 if successful, errcode otherwise
 */
int pReq() {
  RuleFnInit;

  // "ifj21"
  RequireTokenType(t_str);
  if(!strEq(token->data, "ifj21")){
    return ERR(OTHER_SEM_ERR);
  }

  // Generate code
  genStart();

  vypluj 0;
}

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
int pCodeBody() {
  RuleFnInit;

  TryCall(scanner, &token);

  // If it is not EOF, it has to be an ID or a keyword
  if(token && token->type != t_idOrKeyword){
    LOG("An unexpected token in codeBody");
    vypluj ERR(SYNTAX_ERR);

  // 04. <codeBody>        -> eps
  } else if(!token) {
    vypluj 0;
  }

  // 05. <codeBody>        -> function [id] ( <fnDefinitionParamTypeList> )
  //                          <fnRetTypeList> <stat> end <codeBody>
  if(strcmp(token->data, "function") == 0) {

    genComment("New function definition");

    // Labels
    char *fnBypassLabelName = genLabelName("");
    char *varDefStart = genLabelName("");
    char *varDefJumpBack = genLabelName("");
    char *varDefBypass = genLabelName("");

    // Code gen Create an unconditional jump behind the function
    genUnconditionalJump(fnBypassLabelName);

    // [id]
    RequireTokenType(t_idOrKeyword);
    char *fnName = token->data;

    // Define the new function (in the symtable)
    TryCall(newFunctionDefinition, token);
    genFnDef(fnName);

    // Push a new empty frame to the symtable
    TryCall(STPush, symtab);

    // Generate an unconditional jump for variable declarations
    genComment("Jumping to variable declaration");
    genUnconditionalJump(varDefStart);
   
    // Start of fn body code
    genComment("Jump back here from declarations");
    genLabel(varDefJumpBack);
    
    // (
    RequireTokenType(t_leftParen);

    // <fnDefinitionParamTypeList>
    TryCall(pFnDefinitionParamTypeList, fnName);

    // )
    RequireTokenType(t_rightParen);

    // <fnRetTypeList>
    TryCall(pFnRetTypeList, fnName);

    // Generate definitions of parameter variables of this function
    TryCall(createParamVariables, fnName);

    // <stat>
    TryCall(pStat, fnName);

    // end
    RequireToken(t_idOrKeyword, "end");

    // Jump past the declarations
    genUnconditionalJump(varDefBypass);

    // Generate all declarations (and assign nils) and jump back
    genComment("Variable declarations");
    genLabel(varDefStart);
    for(int i = 0; i < varDefBuf->len; i++) {
      genVarDefLF(varDefBuf->data[i]);
      TryCall(genAssignLiteral, varDefBuf->data[i], dt_nil, "nil", "LF");
    }
    genComment("Jump from var declarations");
    genUnconditionalJump(varDefJumpBack);

    // End of function label
    genComment("Skip variable declaration");
    genLabel(varDefBypass);

    // Return the required amount of nils
    TryCall(genNilsReturn, STFind(symtab, fnName)->fnRetTypesBuf->len);

    // Return from the function
    genPopframe();
    genReturnInstruction();

    // Bypass the function by jumping here from the start
    genComment2("Function definition done");
    genLabel(fnBypassLabelName);

    // Pop the new frame from the symtable
    STPop(symtab);

    resetParamCounter();
    
    // <codeBody>
    TryCall(pCodeBody);

  // 06. <codeBody>        -> global [id] : function ( <fnDeclarationParamTypeList> ) <fnRetTypeList> <codeBody>
  } else if(strcmp(token->data, "global") == 0) {

    // [id]
    RequireTokenType(t_idOrKeyword);
    char *fnName = token->data;

    // Declare the new function (in the symtable)
    TryCall(newFunctionDeclaration, fnName);

    // :
    RequireTokenType(t_colon);

    // function
    RequireToken(t_idOrKeyword, "function");

    // (
    RequireTokenType(t_leftParen);

    // <fnDeclarationParamTypeList>
    TryCall(pFnDeclarationParamTypeList, fnName);

    // )
    RequireTokenType(t_rightParen);

    // <fnRetTypeList>
    TryCall(pFnRetTypeList, fnName);

    // <codeBody>
    TryCall(pCodeBody);
  }

  // 07. <codeBody>        -> [id] <fnCall> <codeBody>
  else if(token->type == t_idOrKeyword) {

    genComment("Calling a function from the global frame");

    // The functions name must be in the symtab, must be a function
    if(!STFind(symtab, token->data) || STGetIsVariable(symtab, token->data)){
      LOG("Calling an undeclared function or maybe it is not even a fn");
      vypluj ERR(SYNTAX_ERR);
    }

    // <fnCall>
    TryCall(pFnCall, token->data);

    genComment2("Function call from the global frame done");

    // <codeBody>
    TryCall(pCodeBody);

  } else {
    vypluj ERR(SYNTAX_ERR);
  }

  vypluj 0;
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 09. <fnCall>          -> ( <fnCallArgList> )
 *
 * @param fnName: name of the function which is being called
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnCall(char *fnName) {
  RuleFnInit;

  // (
  RequireTokenType(t_leftParen);

  // Treat the 'write' function call specially
  if(strEq(fnName, "write")) {
    genComment("Calling write functions");
    resetParamCounter();
    genCreateframe();
    TryCall(pFnCallArgList, fnName);

  } else {
    genComment("Calling a function");

    genCreateframe();

    // Pass call arguments to the functino
    genComment("Processing function call arguments");
    resetParamCounter();
    TryCall(pFnCallArgList, fnName);
    genComment2("Processing function call arguments done");

    // Check if the function is in the symtable
    if(!STFind(symtab, fnName)){
      return ERR(ID_DEF_ERR);
    }
    
    // Generate the function call and reset the counters
    genFnCall(fnName);
    resetParamCounter();
  }

  // )
  RequireTokenType(t_rightParen);
    
  vypluj 0;
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 10. <fnCallArgList>   -> eps
 * 11. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
 *
 * @param fnName: name of the function which is being called
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnCallArgList(char *fnName) {
  RuleFnInit;

  // Functions can have several args (params) - this helps keep track of which
  // one we're processing right now (will be passed as a param to nextFnCallArg)
  int argCount = 0;

  GetToken;

  // 10. <fnCallArgList>   -> eps
  if(token->type == t_rightParen) {

    // Check if the amount of parameters is fine (not for 'write' function)
    if(!strEq(fnName, "write")){
      STElem *fn = STFind(symtab, fnName);
      // Amount of arguments doesn't match
      if(fn->fnParamTypesBuf && fn->fnParamTypesBuf->len != 0){
        LOG("Param amount doesn't match\n");
        vypluj ERR(PARAM_RET_ERR);
      }
    }
    
    // Stash the token
    vypluj stashToken(&token);
  } 

  // 11. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
  argCount++;

  // Stash the token, cause we didn't actually process the first arg
  TryCall(stashToken, &token); 
  // <fnCallArg>
  TryCall(pFnCallArg, fnName, argCount);

  // <nextFnCallArg>
  TryCall(pNextFnCallArg, fnName, argCount);

  vypluj 0;
}

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
int pFnCallArg(char *fnName, int argCount) {
  RuleFnInit;

  GetToken;
  int dataType = -1;
  char *paramName = genParamVarName("");

  // Pass a parameter to the function
  // 14. <fnCallArg>       -> [id]
  if (STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)) {
    dataType = STGetVarDataType(symtab, token->data);
    // Pass the parameter by MOVE TF@paramName LF@token->data
    char *varName = NULL;
    TryCall(STGetName, symtab, &varName, token->data);
    genVarDefTF(paramName);
    TryCall(genPassParam, paramName, varName);

  // Pass a literal
  // 15. <fnCallArg>       -> [literal]
  } else if (token->type == t_int || token->type == t_num
      || token->type == t_sciNum || token->type == t_str
      || (token->type == t_idOrKeyword && strEq(token->data, "nil"))) {
    if(token->type == t_int){
      dataType = dt_integer;
    } else if(token->type == t_num || token->type == t_sciNum){
      dataType = dt_number;
    } else if(token->type == t_str){
      dataType = dt_string;
    } else if(token->type == t_idOrKeyword) {
      dataType = dt_nil;
    }

    // Pass the parameter
    genVarDefTF(paramName);
    if(strcmp(fnName, "write") == 0 && strcmp(token->data, "nil") == 0) {
      genAssignLiteralStringNil(paramName, "TF");
    } else {
      TryCall(genAssignLiteral, paramName, dataType, token->data, "TF");
    }

  } else {
    if(token->type == t_idOrKeyword) {
      vypluj ERR(ID_DEF_ERR);
    }
    vypluj ERR(SYNTAX_ERR);
  }

  // 'Write' built in function
  if(strEq(fnName, "write")){
    genWrite(paramName);

  // A parameter data type doesn't match
  }else if(STGetParamType(symtab, fnName, argCount - 1) != dataType){
    LOG("Param data type doesn't match\n");
    vypluj ERR(PARAM_RET_ERR);
  }

  vypluj 0;
}

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
int pNextFnCallArg(char *fnName, int argCount) {
  RuleFnInit;

  GetToken;
  
  // -> , <fnCallArg> <nextFnCallArg>
  if(token->type == t_comma) {
    argCount++;

    // <fnCallArg>
    TryCall(pFnCallArg, fnName, argCount);
    
    // <nextFnCallArg>
    TryCall(pNextFnCallArg, fnName,argCount);

  // -> eps
  }else{
    // Check if the amount of parameters is fine (not for 'write' function)
    if(!strEq(fnName, "write")){
      STElem *fn = STFind(symtab, fnName);
      // Amount of arguments doesn't match
      if(!fn->fnParamTypesBuf || fn->fnParamTypesBuf->len != argCount){
        LOG("Param amount doesn't match");
        vypluj ERR(PARAM_RET_ERR);
      }
    }

    TryCall(stashToken, &token);
  }

  vypluj 0;
}

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
int pStat(char *fnName) {
  RuleFnInit;

  GetToken;

  // 17. <stat>            -> eps
  if (token->type != t_idOrKeyword || (token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 19. <stat>            -> local [id] : [type] <newIdAssign> <stat>
  if (strcmp(token->data, "local") == 0) {

    // [id]
    RequireIDToken(token);

    // Check if the name doesn't already exist as a function
    if(STFind(symtab, token->data) && !STGetIsVariable(symtab, token->data)){
      return ERR(ID_DEF_ERR);
    }

    // Insert the new ID to the symtable
    char *newVarName = token->data;
    TryCall(STInsert, symtab, newVarName);
    TryCall(STSetIsVariable, symtab, newVarName, true);
    TryCall(STSetName, symtab, newVarName, genVarName(token->data, symtab->top->depth));

    // Code gen variable definition
    char *name;
    TryCall(STGetName, symtab, &name, newVarName);
    TryCall(appendToVarDeclarationList, name);

    // :
    RequireTokenType(t_colon);
    
    // [type]
    TryCall(varDataType, newVarName);
    
    // <newIdAssign>
    TryCall(pNewIdAssign, newVarName);

  // 20. <stat>            -> if <expr> then <stat> else <stat> end <stat>
  } else if (strcmp(token->data, "if") == 0) {
    genComment("IF start");

    // Generate new label names for 'else' and 'end' (don't generate code yet)
    char *elseLabelName = genLabelName("else"), *endLabelName = genLabelName("end");

    // <expr>
    char *exprResultVarName = NULL;
    TryCall(pExpr, &exprResultVarName, dt_boolean);

    // then
    RequireToken(t_idOrKeyword, "then");
    // Code gen conditional jump to the 'else' label name
    genJumpIfFalse(elseLabelName, exprResultVarName);

    // <stat>                               
    TryCall(STPush, symtab);
    TryCall(pStat, fnName);
    STPop(symtab);

    // Code gen unconditional jump to the 'end' label name
    genUnconditionalJump(endLabelName);

    // else
    RequireToken(t_idOrKeyword, "else");

    // Code gen label for 'else'
    genLabel(elseLabelName);

    // <stat>
    TryCall(STPush, symtab);
    TryCall(pStat, fnName);
    STPop(symtab);

    // end
    RequireToken(t_idOrKeyword, "end");

    // Code gen label for 'end'
    genLabel(endLabelName);

    genComment("IF done");

    // 21. <stat>            -> while <expr> do <stat> end <stat>
  } else if (strcmp(token->data, "while") == 0) {
    genComment("WHILE start");

    // Generate new label names (don't generate code yet)
    char *whileLabelName = genLabelName("");
    char *startLabelName = genLabelName("");
    char *endLabelName = genLabelName("");

    // Code gen 'start' label
    genLabel(startLabelName);

    // <expr>
    char *exprResultVarName = NULL;
    TryCall(pExpr, &exprResultVarName, dt_boolean);

    // Code gen conditional jump to 'while'
    genJumpIfTrue(whileLabelName, exprResultVarName);
    // Code gen unconditional jump to 'end'
    genUnconditionalJump(endLabelName);

    // do
    RequireToken(t_idOrKeyword, "do");

    // Code gen label for 'while'
    genLabel(whileLabelName);

    // <stat>
    TryCall(STPush, symtab);
    TryCall(pStat, fnName);
    STPop(symtab);

    // end
    RequireToken(t_idOrKeyword, "end");

    // Code gen unconditional jump to 'start'
    genUnconditionalJump(startLabelName);

    // Code gen label for 'end'
    genLabel(endLabelName);

    genComment("WHILE done");

  // 22. <stat>            -> return <retArgList> <stat>
  } else if (strcmp(token->data, "return") == 0) {
    genComment("Return encountered - returning");

    // <retArgList>
    if(strcmp(token->data, "return") == 0) {
      TryCall(pRetArgList, fnName);
    }

    // Reset the counter
    resetRetCounter();

    // Code gen POPFRAME
    genPopframe();

    // Code gen RETURN from the function
    genReturnInstruction();

    genComment2("Return done");

  // 22. <stat>            [id] <statWithId> <stat>
  } else if (STFind(symtab, token->data)) { // A function or a variable
    // <statWithId>
    TryCall(pStatWithId, token->data);

  // -> eps
  } else {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // <stat>
  vypluj pStat(fnName);  
}

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
int pStatWithId(char *idName) {
  RuleFnInit;

  // -> <fnCall>
  if(STFind(symtab, idName) && !STGetIsVariable(symtab, idName)){
    TryCall(pFnCall, idName);
    vypluj 0;
  }

  // Not a function call but an assignment
  GetToken;
    
  // idName must be a variable
  if(!STFind(symtab, idName) || !STGetIsVariable(symtab, idName)){
    vypluj ERR(SYNTAX_ERR);
  }

  // -> = <expr>
  if (token->type == t_assignment) {

    // Get the name of the variable where the expr result should be stored
    char *destVarName;
    TryCall(STGetName, symtab, &destVarName, idName);

    // Call the shift-reduce parser and assign the result to retVarName
    char *retVarName = NULL;
    int idType = STGetVarDataType(symtab, idName);
    TryCall(pExpr, &retVarName, idType);

    if(!retVarName){
      // If this is null, the expr was a function call and we need to fetch
      // the return value from ret_0 (which is in TF!)
      genMoveToLF(destVarName, "!ret_0");
    }else{
      // Otherwise the ret val is in LF
      genMove(destVarName, retVarName);
    }

  // Multiassignment
  // In idName we have a name of the first variable in this statement
  // In token->data we'll have a name of the second one
  // -> , [id] <nextAssign> <expr> , <expr>
  }else if (token->type == t_comma) {
    char *endLabel = genLabelName("multiassign_end");

    // Append the name (in ifjcode) of the first id in this multiassignment to
    // the multiassignment id list
    char *name = NULL;
    TryCall(STGetName, symtab, &name, idName);
    TryCall(LLAppend, &multiAssignIdList, name, STGetVarDataType(symtab, idName));

    // [id]
    RequireTokenType(t_idOrKeyword);
    if (!STFind(symtab, token->data) 
        || !STGetIsVariable(symtab, token->data)) {
      vypluj ERR(SYNTAX_ERR);
    }

    // Do the same with the second id (append to the id list)
    TryCall(STGetName, symtab, &name, token->data);
    TryCall(LLAppend, &multiAssignIdList, name, STGetVarDataType(symtab, token->data));

    // When this becomes true, it indicates we have enough return values (from 
    // functions and expressions) to assign to all ids in the id list
    bool assignmentDone = false;

    // <nextAssign>
    // Check and generate more assignments if there are some
    GetToken;

    // If we need more values, get more values
    if(!assignmentDone){
      // <expr>
      TryCall(processExpr, &assignmentDone, endLabel);
    }

    // If we need more values, get more values
    if(!assignmentDone){
      // ,
      GetToken;
      if(token->type == t_comma){
        // <expr>
        TryCall(processExpr, &assignmentDone, endLabel);
      }
    }

    // Generate jumps to the function calls and expression evaluations
    for(int i = labelList->len - 1; i > 0; i--){
      genUnconditionalJump(labelList->data[i]);
    }

    // Generate the multiassignment end label
    genLabel(endLabel);

    // Finally, assign the return values from ret list to ids in id list
    int varCount = LLGetLength(multiAssignIdList);
    for(int i = 1; i < varCount; i++) {
      char *id = LLGetElemByIndex(multiAssignIdList, i)->name;
      char *ret = LLGetElemByIndex(multiAssignRetList, i)->name;
      genMove(id, ret);
    }

  } else {
    vypluj ERR(SYNTAX_ERR);
  }
  
  vypluj 0;
}

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
int pNextAssign(bool *assignmentDone, char *endLabel) {
  RuleFnInit;

  GetToken;

  // -> =
  if (token->type == t_assignment) {
    // We're done fetching ids
    vypluj 0;
  }

  if(token->type != t_comma) {
    vypluj ERR(SYNTAX_ERR);
  }

  // [id]
  // The next token must be a variable
  RequireTokenType(t_idOrKeyword);
  if (!STFind(symtab, token->data) || !STGetIsVariable(symtab, token->data)) {
    vypluj ERR(SYNTAX_ERR);
  }
  // Get the variables name in ifjcode and append it to the id list
  char *name = NULL;
  int dataType = STGetVarDataType(symtab, token->data);
  TryCall(STGetName, symtab, &name, token->data);
  TryCall(LLAppend, &multiAssignIdList, name, dataType);

  // <nextAssign>
  TryCall(pNextAssign, assignmentDone, endLabel);

  // If we still don't have enough values to assign to all ids, get another
  if(!assignmentDone){
    // <expr>
    TryCall(processExpr, assignmentDone, endLabel);

    // If the next token is a comma, consume it
    GetToken;
    if(token->type != t_comma){
      TryCall(stashToken, &token);
    }
  }

  vypluj 0;
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 29. <fnDefinitionParamTypeList>       -> eps
 * 30. <fnDefinitionParamTypeList>       -> [id] : [type] <nextFnDefinitionParamType>
 *
 * @param fnName: name of the function of which param types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnDefinitionParamTypeList(char *fnName) {
  RuleFnInit;
  int paramCount = 0;

  GetToken;

  // -> eps
  if(token->type != t_idOrKeyword || isIFJ21Keyword(token)) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> [id] : [type] <nextFnDefinitionParamType>

  paramCount++;

  // [id]
  // Update the symtable
  TryCall(STAppendParamName, symtab, fnName, token->data);
  TryCall(STInsert, symtab, token->data);
  TryCall(STSetIsVariable, symtab, token->data, true);
  char *newName = genVarName(token->data, symtab->top->depth);
  TryCall(STSetName, symtab, token->data, newName);

  // :
  RequireTokenType(t_colon);

  // [type]
  // Append the data type of the parameter to the symtab
  TryCall(fnDefinitionParamType, fnName, paramCount);

  // <nextFnDefinitionParamType>
  vypluj pNextFnDefinitionParamType(fnName, paramCount);
}

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
int pNextFnDefinitionParamType(char *fnName, int paramCount) {
  RuleFnInit;

  GetToken;

  // -> eps
  if (token->type != t_comma) {
    // TODO check amount of data types (if there is x of them in declaration 
    // and y of them in definition and x!=y -> err)
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> , [id] : [type] <nextFnDefinitionParamType>

  // [id]
  // Must be an ID, can't be a keyword
  RequireIDToken(token);
  paramCount++;

  // Update the symtable
  TryCall(STAppendParamName, symtab, fnName, token->data);
  TryCall(STInsert, symtab, token->data);
  TryCall(STSetIsVariable, symtab, token->data, true);
  char *newName = genVarName(token->data, symtab->top->depth);
  TryCall(STSetName, symtab, token->data, newName);

  // :
  RequireTokenType(t_colon);

  // [type]
  // Append the data type of the parameter to the symtab
  TryCall(fnDefinitionParamType, fnName, paramCount);

  // <nextFnDefinitionParamType>
  vypluj pNextFnDefinitionParamType(fnName, paramCount);
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 34. <retArgList>      -> eps
 * 35. <retArgList>      -> <expr> <retNextArg>
 *
 * @param fnName: name of the function of which ret types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pRetArgList(char *fnName) {
  RuleFnInit;

  char *retVarName = NULL;

  // 34. <retArgList>      -> eps
  // Generate simple return with no arguments
  if(STFind(symtab, fnName) && STFind(symtab, fnName)->fnRetTypesBuf->len == 0){
    genPopframe();
    genReturnInstruction();
    return 0;
  }

  // 35. <retArgList>      -> <expr> <retNextArg>

  // Generate code for the expression
  TryCall(pExpr, &retVarName, STGetRetType(symtab, fnName, 0));

  int argCount = 1;

  // Code gen Define and assign the return values
  // Generate a new name where the return value will be written (in LF)
  char *retArgName = genRetVarName("");
  // Define the retArgName
  genVarDefLF(retArgName);
  // Pass from retArgName to retVarName
  genMove(retArgName, retVarName);

  // <retNextArg>
  TryCall(pRetNextArg, fnName, argCount);

  vypluj 0;
}

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
int pRetNextArg(char *fnName, int argCount) {
  RuleFnInit;

  GetToken;

  // -> eps
  if (token->type != t_comma) {
    // Check if the amount of return arguments is right
    if(argCount > STFind(symtab, fnName)->fnRetTypesBuf->len){
      LOG("Returning too many values from a function");
      return ERR(PARAM_RET_ERR);
    }
    // Otherwise, we received the last argument - if there are not enough of
    // them, fill the rest up with nils
    int nilsAmount = STFind(symtab, fnName)->fnRetTypesBuf->len - argCount;
    TryCall(genNilsReturn, nilsAmount);
    // And just stash that token
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> , <expr> <retNextArg>
  int expectedType = STFind(symtab, fnName)->fnRetTypesBuf->data[argCount];
  argCount++;
  char *retVarName = NULL;

  // <expr>
  TryCall(pExpr, &retVarName, expectedType);
  if(!retVarName){
    // No expr found after the comma!
    LOG("An expression is required but none was received");
    return ERR(SYNTAX_ERR);
  }

  // Code gen Pass the return values down by one frame
  // Generate a new name where the return value will be written (in LF)
  char *retArgName = genRetVarName("");
  // Define the retArgName
  genVarDefLF(retArgName);
  // Pass from TF (retArgName) to LF (retVarName)
  genMove(retArgName, retVarName);

  // <retNextArg>
  vypluj pRetNextArg(fnName, argCount);
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 39. <fnDeclarationParamTypeList> -> eps
 * 40. <fnDeclarationParamTypeList> -> [type] <nextFnDeclarationParamType>
 *
 * @param fnName: name of the function of which param types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnDeclarationParamTypeList(char *fnName) {
  RuleFnInit;

  GetToken;
  
  // 39. <fnDeclarationParamTypeList> -> eps 
  if(token->type == t_rightParen) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 40. <fnDeclarationParamTypeList> -> [type] <nextFnDeclarationParamType>
  int paramCount = 1;
  
  // [type]
  TryCall(fnDeclarationParamType, fnName, token->data);

  // <nextFnDeclarationParamType>
  TryCall(pNextFnDeclarationParamType, fnName, paramCount);

  vypluj 0;
}

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
int pNextFnDeclarationParamType(char *fnName, int paramCount) {
  RuleFnInit;

  GetToken;

  // 41. <nextFnDeclarationParamType>   -> eps
  if(token->type != t_comma){
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 42. <nextFnDeclarationParamType>   -> , [type] <nextFnDeclarationParamType>
  paramCount++;

  GetToken;

  // [type]
  TryCall(fnDeclarationParamType, fnName, token->data);
  
  // <nextFnDeclarationParamType>
  TryCall(pNextFnDeclarationParamType, fnName, paramCount);

  return 0;
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 44. <fnRetTypeList>   -> eps 
 * 45. <fnRetTypeList>   -> : [type] <nextRetType>
 *
 * @param fnName: name of the function of which ret types we are processing
 *
 * @return 0 if successful, errcode otherwise
 */
int pFnRetTypeList(char *fnName) {
  RuleFnInit;

  GetToken;

  // 44. <fnRetTypeList>   -> eps
  if(token->type != t_colon) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 45. <fnRetTypeList>   -> : [type] <nextRetType>
  
  // Used to keep track of how many variables were returned in a single function
  int retVarCounter = 0;

  // [type]
  TryCall(fnRetDataType, fnName, &retVarCounter);

  // <nextRetType>
  TryCall(pNextRetType, fnName, &retVarCounter);

  vypluj 0;
}

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
int pNextRetType(char *fnName, int *retVarCounter) {
  RuleFnInit;

  GetToken;

  // eps
  if(token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // [type]
  TryCall(fnRetDataType, fnName, retVarCounter);

  // <nextRetType>
  TryCall(pNextRetType, fnName, retVarCounter);

  vypluj 0;
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * 49. <newIdAssign>     -> eps
 * 50. <newIdAssign>     -> = <expr>
 *
 * @param varName: name of the variable to which we are assigning
 *
 * @return 0 if successful, errcode otherwise
 */
int pNewIdAssign(char *varName) {
  RuleFnInit;

  GetToken;

  // -> eps
  if (token->type != t_assignment) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // <expr>
  char *retVarName = NULL;
  int type = STGetVarDataType(symtab, varName);
  TryCall(pExpr, &retVarName, type);
  
  STElem *var = STFind(symtab, varName);

  // Check if the variable exists in the symtable
  if(!var) {
    vypluj ERR(SYNTAX_ERR);
  }

  if(!retVarName){
    // If this is null, the expr was a function call and we need to fetch
    // the values from ret_0 (which is in TF!)
    genMoveToLF(var->name, "!ret_0");
  }else{
    // Otherwise the ret val is in LF
    genMove(var->name, retVarName);
  }

  vypluj 0;
}

/**
 * @brief A function representing rules for a non-terminal in the CFG
 * <expr>
 *
 * @param retVarName: destination pointer for the expression result
 * @param expectedType: data type the result of the expression should be
 *
 * @return 0 if successful, errcode otherwise
 */
int pExpr(char **retVarName, int expectedType) {
  RuleFnInit;

  GetToken;

  // If it is a function call (and fn is defined), don't call the shift-reduce 
  // parser at all
  if(token->type == t_idOrKeyword && STFind(symtab, token->data) 
    && !STGetIsVariable(symtab, token->data)) {

    genComment("Calling a function inside a function");
    TryCall(pFnCall, token->data);
    genComment("Function call inside a function done");
    
  // If it is a nil
  } else if(strEq(token->data, "nil")) {
    // Code gen define a var, assign nil and return the name in retVarName
    *retVarName = genTmpVarName();
    TryCall(genAssignLiteral, *retVarName, dt_nil, "nil", "LF"); 

  // If it it the 'else' keyword, end
  } else if(strEq(token->data, "else")) {
    TryCall(stashToken, &token); 

  // A keyword is forbidden here
  } else if(token->type == t_idOrKeyword && isIFJ21Keyword(token)) {
    vypluj ERR(SYNTAX_ERR);

  // An expression
  } else {
    int retVarType = -1;
    // Call the shift-reduce parser
    TryCall(parseExpression, symtab, token, retVarName, &retVarType);

    if(!(*retVarName)){
      // An expression is required but the precedence analysis didn't find any
      vypluj ERR(SYNTAX_ERR);
    }
    if(retVarType != expectedType && expectedType != dt_boolean) {
      // Assignment error if the type doesn't match (can be a boolean though)
      vypluj ERR(ASS_ERR);
    }
  }
  vypluj 0;
}

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
int processExpr(bool *assignmentDone, char *endLabel) {

  // Get next token info before calling pExpr
  Token *token = NULL;
  GetToken;
  char *fnName = token->data;
  TryCall(stashToken, &token);

  char *retVarName = NULL;
  int dataType = -1;

  // Gen jump to bypass the function call or expression
  char *bypassLabel = genLabelName("bypass");
  genUnconditionalJump(bypassLabel);

  // Generate label to jump to to evaluate the function call or expression
  char *evalLabel = genLabelName("eval");
  TryCall(dynStrArrAppend, labelList, evalLabel);
  genLabel(evalLabel);

  // Call the pExpr (<expr>)
  TryCall(pExpr, &retVarName, dataType);
    
  // It was a function: append the return values to the ret list
  if(STFind(symtab, fnName) && !STGetIsVariable(symtab, fnName)){

    // Get the amount of variables returned from the function
    int retVarsAmount = STFind(symtab, fnName)->fnRetTypesBuf->len;

    // Append all return values to multiAssignRetList one by one
    // ... also generate code to move the ret values from TF to LF
    resetRetCounter();
    for(int i = 0; i < retVarsAmount; i++){
      retVarName = genRetVarName("");

      // Move the return value from TF to a temporary variable in LF
      char *tmp = genTmpVarName();
      TryCall(appendToVarDeclarationList, tmp);
      genMoveToLF(tmp, retVarName);

      // Append it to the ret list
      dataType = STGetRetType(symtab, fnName, i);
      TryCall(LLAppend, &multiAssignRetList, tmp, dataType);
    }
    resetRetCounter();

  // An expression: append the return value to the ret list
  }else{
    // Add the return value (result) to multiAssignRetList
    TryCall(LLAppend, &multiAssignRetList, retVarName, dataType);
  }
  
  // Jump to the end after the assignment
  genUnconditionalJump(endLabel);

  // Generate the bypass label
  genLabel(bypassLabel);

  // If we have enough values to assign to the id list, we're done
  if(LLGetLength(multiAssignIdList) <= LLGetLength(multiAssignRetList)){
    *assignmentDone = true;
  }

  return 0;
}

/**
 * @brief Appends a variables name (in ifjcode) to the declaration list so it
 * can be declared later (to avoid multiple declarations)
 *
 * @param name of the variable in ifjcode
 *
 * @return err code
 */
int appendToVarDeclarationList(char *name) {
  if(!varDefBuf){
    return ERR(INTERN_ERR);
  }
  for(int i = 0; i < varDefBuf->len; i++) {
    if(strcmp(name, varDefBuf->data[i]) == 0) {
      vypluj 0;
    }
  }
  TryCall(dynStrArrAppend, varDefBuf, name)
  vypluj 0;
}

/**
 * @brief Check if a string represents a data type
 *
 * @param data: input string
 *
 * @return true if the string represents a data type
 */
bool isDataType(char *data) {
  if (   strEq(data, "string")
      || strEq(data, "integer")
      || strEq(data, "number")
      || strEq(data, "nil")) {
    vypluj true;
  } else { 
    vypluj false;
  }
}

/**
 * @brief If the function wasn't already defiend, define it (add it to the
 * symtable)
 *
 * @param token containing the name of the function
 *
 * @return 0 if successful, errcode otherwise
 */
int newFunctionDefinition(Token *token) {
  // Redefinition
  if (STFind(symtab, token->data) 
      && !STGetIsVariable(symtab, token->data) 
      && STGetFnDefined(symtab, token->data)) {
    LOG("Function already defined");
    vypluj ERR(ID_DEF_ERR);

  // Definition with the name of a keyword
  } else if (isIFJ21Keyword(token)){
    LOG("Definition of a function with a keyword as a name");
    vypluj ERR(SYNTAX_ERR);

  } else {
    LOG("Adding definition of %s (function) to the symtable", token->data);
    TryCall(STInsert, symtab, token->data);
    TryCall(STSetIsVariable, symtab, token->data, false);
    TryCall(STSetFnDefined, symtab, token->data, true);
    TryCall(STSetName, symtab, token->data, genLabelName(""));
  }
  vypluj 0;
}

/**
 * @brief If the function wasn't already declared, declare it (add it to the
 * symtable)
 *
 * @param fnName: name of the function
 *
 * @return 0 if successful, errcode otherwise
 */
int newFunctionDeclaration(char *fnName) {
  // Redeclaration
  if (STFind(symtab, fnName)) {
    LOG("Function already declared");
    vypluj ERR(ID_DEF_ERR);

  } else {
    LOG("Adding declaration of %s (function) to the symtable", fnName);
    TryCall(STInsert, symtab, fnName);
    TryCall(STSetIsVariable, symtab, fnName, false);
    TryCall(STSetFnDeclared, symtab, fnName, true);
    TryCall(STSetName, symtab, fnName, genLabelName(""));
  }
  vypluj 0;
}

/**
 * @brief Returns true if the token represents a literal (integer, number or a
 * string)
 *
 * @param token
 *
 * @return true if the token is a literal
 */
bool isLiteral(Token *token) {
  if(    token->type == t_str
      || token->type == t_num
      || token->type == t_sciNum
      || token->type == t_int
      || strEq(token->data, "nil")) {
    vypluj true;
  }
  vypluj false;
}

/**
 * @brief Process a data type of a return value of a function (when declaring
 * or defining it)
 *
 * @param fnName: name of the function
 * @param retVarCounter: number of return values already processed
 *
 * @return 0 if successful, errcode otherwise
 */
int fnRetDataType(char *fnName, int *retVarCounter){
  Token *token = NULL;

  // Get the new data type
  GetToken;
  if(isDataType(token->data) == false) {
    LOG("A data type is required but we did not receive one");
    vypluj ERR(SYNTAX_ERR);
  }
  int dataType = getDataTypeFromString(token->data);

  (*retVarCounter)++;

  // If the function was declared and we're defining it, check if params match
  if(STGetFnDeclared(symtab, fnName) && STGetFnDefined(symtab, fnName)){
    int dataTypeInSymtab = STGetRetType(symtab, fnName, *retVarCounter - 1);
    // If they don't match -> err
    if(dataTypeInSymtab != dataType){
      LOG("Parameter types between fn decl and def don't match");
      vypluj ERR(PARAM_RET_ERR);
    }
  }

  // Otherwise, append the new data type to the function in the symtab
  TryCall(STAppendRetType, symtab, fnName, dataType);

  vypluj 0;
}

/**
 * @brief Process a data type of a parameter of a function when declaring it
 *
 * @param fnName: name of the function
 * @param data: data type in a string
 *
 * @return 0 if successful, errcode otherwise
 */
int fnDeclarationParamType(char *fnName, char *data) {
  STElem *fn = STFind(symtab, fnName);
  // Check if function is in the symtable
  if(!fn) {
    vypluj ERR(INTERN_ERR);
  }

  // Append the data type to the function in the symtable
  int dataType = getDataTypeFromString(data);
  TryCall(STAppendParamType, symtab, fnName, dataType); 

  vypluj 0;
}

/**
 * @brief Process a data type of a parameter of a function when defining it
 *
 * @param fnName: name of the function
 * @param paramCount: the counter of parameters
 *
 * @return 0 if successful, errcode otherwise
 */
int fnDefinitionParamType(char *fnName, int paramCount){
  LOG();
  Token *token = NULL;

  // Get the data type of the parameter
  GetToken;
  if(!isDataType(token->data)) {
    LOG("A data type is required but we did not receive one");
    vypluj ERR(SYNTAX_ERR);
  }
  int dataType = getDataTypeFromString(token->data);

  // If the function was declared and we're definining it, check if params match
  if(STGetFnDeclared(symtab, fnName) && STGetFnDefined(symtab, fnName)){
    int dataTypeInSymtable = STGetParamType(symtab, fnName, paramCount - 1);
    // If they don't match -> err
    if(dataTypeInSymtable != dataType){
      LOG("Parameter types between fn decl and def don't match");
      return ERR(PARAM_RET_ERR);
    }
  }

  // Otherwise, append the new data type to the function in the symtab
  TryCall(STAppendParamType, symtab, fnName, dataType);

  // Set the data type of the variable we created for passing the param
  char *varName;
  TryCall(STGetParamName, symtab, &varName, fnName, paramCount - 1);
  TryCall(STSetVarDataType, symtab, varName, dataType);

  vypluj 0;
}

/**
 * @brief Process data type of a variable when declaring or defining it
 *
 * @param varName: name of the variable
 *
 * @return 0 if successful, errcode otherwise
 */
int varDataType(char *varName){
  Token *token;
  STElem *var = STFind(symtab, varName);

  // If the variable is not really a variable or doesn't exist
  if(var == NULL) {
    vypluj ERR(ID_DEF_ERR);
  }

  GetToken;
  
  if(isDataType(token->data) == false) {
    vypluj ERR(TYPE_EXPR_ERR); // TODO change err code
  }

  var->varDataType = getDataTypeFromString(token->data);

  vypluj 0;
}

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
int createParamVariables(char *fnName){
  // Get the function from the symtable
  STElem *fn = STFind(symtab, fnName);
  if(!fn){
    // The function is not in the symtable!
    return ERR(ID_DEF_ERR);
  }

  // Generate definitions of the parameter variables
  int paramCount = fn->fnParamNamesBuf->len;
  
  // Assign the parameters one by one
  for(int i = 0; i < paramCount; i++){
    char *paramName;
    TryCall(STGetParamName, symtab, &paramName, fnName, i);
    STElem *paramVar = STFind(symtab, paramName);
    TryCall(appendToVarDeclarationList, paramVar->name);

    // Assign parameters to the defined variables
    genMove(paramVar->name, genParamVarName(""));
  }

  vypluj 0;
}

#endif
/* end of file parser.c */
