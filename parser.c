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

/** IMPORTANT !!! READ FIRST, BEFORE EDITING
 *
 * --------------------------------------------------
 * TOP PRIORITY
 * WE NEED MORE STATES FOR BUILT IN FUNCTIONS
 * ADD THEM TO OUR CFG TOO !!!!!!!!!!!!
 * --------------------------------------------------
 *
 * 1. code in testing
 *
 * --------------------------------------------------
 * Function             State
 * --------------------------------------------------
 ****** OUTDATED (5.12.)
 * pStart                             FULLY DONE
 * pReq                               FULLY DONE
 * pCodeBody                          seems like done
 * pFnCall                            Built in functions! 
 * pFnCallArgList                     FULLY DONE (minor TODOs)
 * pNextFnCallArg                     FULLY DONE (minor TODOs)
 * pFnCallArg                         FULLY DONE (hopefully) (TODO errcode)
 * pStat                              how to program the while cycle?
 * pStatWithId                        might work
 * pNextAssign                        might work
 * pFnDefinitionParamTypeList         might work but dependant on type
 * pNextFnDefinitionParamType         might work but dependant on type
 * pRetArgList        
 * pRetNextArg        
 * pFnDeclarationParamTypeList 
 * pNextFnDeclarationParamType   
 * pFnRetTypeList   
 * pNextRetType     
 * pNewIdAssign
 * pExpr                              could work


* -----------------------------------------------------------------------
 */

#ifndef PARSER_C
#define PARSER_C

#include "parser.h"


extern int ret;

STStack *symtab;

AssignElement *assignmentElement;
AssignElement *assignmentElement2;
StringBuffer *labelBuffer;


int assigmentCounter = 0;
int assigmentGeneratedCounter = 0;
int retVarCounter = 0;
StringBuffer *varDefBuff = NULL;
int fnRetArgStashedInExpr = -1; // -1 -> nothing, otherwise returne the nth parameter of a function


/*
 *
 * Rules 
 *
 */

/**
 * @brief Starting rule for require
 *
 * @return int - 0 if success, else on error
 *
 * 01. <start>           -> require <req> <codeBody>
 */
int pStart() {
  RuleFnInit;
  TryCall(stringBufInit, &varDefBuff);
  AListInit(&assignmentElement);
  AListInit(&assignmentElement2);
  stringBufInit(&labelBuffer);

  // require
  RequireToken(t_idOrKeyword, "require");

  // <req>
  TryCall(pReq);

  genBuiltInFunctions();

  // <codeBody>
  TryCall(pCodeBody);

  vypluj 0;
}

/**
 * @brief 
 * 
 *
 *
 * @return error code
 *
 * 02. <req>             -> "ifj21"
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
 * @brief Rule for <codebody> string
 *
 * @return int - 0 if success, else on error
 *
 * 04. <codeBody>        -> eps
 * 05. <codeBody>        -> function [id] ( <fnDefinitionParamTypeList> ) <fnRetTypeList> <stat> end <codeBody>
 * 06. <codeBody>        -> global [id] : function ( <fnDeclarationParamTypeList> ) <fnRetTypeList> <codeBody>
 * 07. <codeBody>        -> [id] <fnCall> <codeBody>
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

    // labels
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

    TryCall(STPush, symtab);

    // generate an unconditional jump to definitions
    genComment("Jumping to variable declaration");
    genUnconditionalJump(varDefStart);
   
    // start of fn body code
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
    createParamVariables(fnName);
    // TODO

    // <stat>
    TryCall(pStat, fnName);

    // end
    RequireToken(t_idOrKeyword, "end");

    // jump past the declarations
    genUnconditionalJump(varDefBypass);

    // generate all declarations and jump back
    genComment("Variable declarations");
    genLabel(varDefStart);
    for(int i = 0; i < varDefBuff->len; i++) {
      genVarDefLF(varDefBuff->data[i]);
      genAssignLiteral(varDefBuff->data[i], dt_nil, "nil", "LF");
    }
    genComment("Jump from var declarations");
    genUnconditionalJump(varDefJumpBack);

    // end of function label
    genComment("Skip variable declaration");
    genLabel(varDefBypass);

    // Return the required amount of nils
    genNilsReturn(STFind(symtab, fnName)->fnRetTypesBuf->len);

    // Return from the function
    genPopframe();
    genReturnInstruction();

    // Code gen Create a label here, behind the function
   
    genComment2("Function definition done");
    genLabel(fnBypassLabelName);

    STPop(symtab);

    resetParamCounter();
    
    // <codeBody>
    TryCall(pCodeBody);
  }

  // 06. <codeBody>        -> global [id] : function ( <fnDeclarationParamTypeList> ) <fnRetTypeList> <codeBody>
  else if(strcmp(token->data, "global") == 0) {

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

    // The function name must be in the symtab, must be a function
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
 *
 * @return error code
 *
 * 09. <fnCall>          -> ( <fnCallArgList> )
*/
int pFnCall(char *fnName) {
  RuleFnInit;

  // (
  RequireTokenType(t_leftParen);

  if(strEq(fnName, "write")) {
    genComment("Calling write functions");
    resetParamCounter();
    genFnCallInit();
    TryCall(pFnCallArgList, fnName);

  } else {
    genComment("Calling a function");

    genFnCallInit();

    genComment("Processing function call arguments");
    resetParamCounter();
    TryCall(pFnCallArgList, fnName);
    genComment2("Processing function call arguments done");

    // Get the function element from the symtable
    STElem *fnElement = STFind(symtab, fnName);
    if(!fnElement){
      return ERR(SYNTAX_ERR);
    }

    genFnCall(fnName);
    retVarCounter = 0;
    resetParamCounter();
    assigmentCounter = 0;
    assigmentGeneratedCounter = 0;
  }

  // )
  RequireTokenType(t_rightParen);
    
  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 10. <fnCallArgList>   -> eps
 * 11. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
 */
int pFnCallArgList(char *fnName) {
  RuleFnInit;

  // Functions can have several args (params) - this helps keep track of which
  // one we're processing right now (will be passed as a param to nextFnCallArg)
  int argCount = 0;

  GetToken;
  printToken(token);

  // 10. <fnCallArgList>   -> eps
  if(token->type == t_rightParen) {
    LOG("-> eps");

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

  LOG("-> <fnCallArg> <nextFnCallArg>\n");
  argCount++;

  TryCall(stashToken, &token); // cause we didn't actually process the first arg

  // <fnCallArg>
  TryCall(pFnCallArg, fnName, argCount);

  // <nextFnCallArg>
  TryCall(pNextFnCallArg, fnName, argCount);
  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 12. <nextFnCallArg>   -> eps
 * 13. <nextFnCallArg>   -> , <fnCallArg> <nextFnCallArg>
 */
int pNextFnCallArg(char *fnName, int argCount) {
  RuleFnInit;

  GetToken;
  
  // -> , <fnCallArg> <nextFnCallArg>
  // ,
  if(token->type == t_comma) {
    argCount++;

    // <fnCallArg>
    TryCall(pFnCallArg, fnName, argCount);
    
    // <nextFnCallArg>
    TryCall(pNextFnCallArg, fnName,argCount);

  // -> eps
  }else{
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
 * @brief
 *
 * @return error code
 *
 * 14. <fnCallArg>       -> [id]
 * 15. <fnCallArg>       -> [literal]
 */
int pFnCallArg(char *fnName, int argCount) {
  RuleFnInit;

  GetToken;
  int dataType = -1;
  char *paramName = genParamVarName("");

  // Pass a parameter to the function
  // 14. <fnCallArg>       -> [id]
  if (STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)) {
    LOG("-> [id]\n");
    dataType = STGetVarDataType(symtab, token->data);
    // Pass the parameter by MOVE TF@paramName LF@token->data
    //char *paramName = fn->fnParamNamesBuf->data[argCount - 1]; UNUSED VARIABLE
    // TODO shouldn't we use this?

    char *varName = NULL;
    TryCall(STGetName, symtab, &varName, token->data);
    genVarDefTF(paramName);
    genPassParam(paramName, varName);

  // 15. <fnCallArg>       -> [literal]
  } else if (token->type == t_int || token->type == t_num ||
      token->type == t_sciNum || token->type == t_str ||
      (token->type == t_idOrKeyword && strcmp(token->data, "nil") == 0)) {
    LOG("-> [literal]\n");
    if(token->type == t_int){
      dataType = dt_integer;
    } else if(token->type == t_num || token->type == t_sciNum){
      dataType = dt_number;
    } else if(token->type == t_str){
      dataType = dt_string;
    } else if(token->type == t_idOrKeyword) {
      dataType = dt_nil;
    }

    genVarDefTF(paramName);
    if(strcmp(fnName, "write") == 0 && strcmp(token->data, "nil") == 0) {
      genAssignLiteralStringNil(paramName, "TF");
    } else {
      genAssignLiteral(paramName, dataType, token->data, "TF");
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
 * @brief
 *
 * @return error code
 *
 * 17. <stat>            -> eps
 * 18. <stat>            -> [id] <statWithId> <stat>
 * 19. <stat>            -> local [id] : [type] <newIdAssign> <stat>
 * 20. <stat>            -> if <expr> then <stat> else <stat> end <stat>
 * 21. <stat>            -> while <expr> do <stat> end <stat>
 * 22. <stat>            -> return <retArgList> <stat> 
 */
int pStat(char *fnName) {
  RuleFnInit;

  GetToken;
  printToken(token);

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
    TryCall(condAppendToStringBuff, name);

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

    /*
     * TODO
     */
    // TODO Treba toto robiť nejak na novom frame??? lebo sa budú každým cyklom 
    // definovať a deklarovať nové tmp premenné
    // TODO ano - v pExpr dávať do bufferu
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
    LOG("-> return <retArgList> <stat>\n");

    genComment("Return encountered - returning");

    // <retArgList>
    if(strcmp(token->data, "return") == 0) {
      TryCall(pRetArgList, fnName);
    }

    // Code gen POPFRAME
    genPopframe();

    
    // Count how many return arguments we need
    int retArgsCount = 0;
    int i = 0;
    while(STGetRetType(symtab, fnName, i) != -1){
      retArgsCount ++;
      i++;
    }
    // TODO move all !ret vars from TF to LF now
    // NO! Move outside the function. Not here
    /**
      * resetRetCounter();
      * for(i = 0; i < retArgsCount; i++){
      *   char *retVarName = genRetVarName("");
      *   genMoveToLF(retVarName, retVarName);
      * }
      */

    // Code gen RETURN from the function
    genReturnInstruction();

    genComment2("Return done");

  // 22. <stat>            [id] <statWithId> <stat>
  } else if (STFind(symtab, token->data)) { // A function or a variable
    LOG("<stat>            -> [id] <statWithId> <stat>\n");

    // <statWithId>
    TryCall(pStatWithId, token->data);
    LOG("HUHU");

  // -> eps
  } else {
    LOG("-> eps");
    TryCall(stashToken, &token);
    vypluj 0;
  }

// <stat>
vypluj pStat(fnName);  
}

int processExpr(bool *assignmentDone, char *endLabel) {
  // Get next token info before calling pexpr
  Token *token = NULL;
  GetToken;
  char *fnName = token->data;
  stashToken(&token);

  char *retVarName = NULL;
  int dataType = -1;

  // We have a function
  if(STFind(symtab, fnName) && !STGetIsVariable(symtab, fnName)){
    int retVarsAmount = STFind(symtab, fnName)->fnRetTypesBuf->len;

    char *bypassLabel = genLabelName("bypass");

    // gen jump to bypass
    genUnconditionalJump(bypassLabel);

    char *evalLabel = genLabelName("eval");
    stringBufAppend(labelBuffer, evalLabel);

    // gen label to eval expr
    genLabel(evalLabel);

    TryCall(pExpr, &retVarName, &dataType);
    
    // Append all return values to assignmentElement2 one by one
    resetRetCounter();
    for(int i = 0; i < retVarsAmount; i++){
      retVarName = genRetVarName("");
      dataType = STGetRetType(symtab, fnName, i);
      // TODO gen move to LF
      char *b = genTmpVarName();
      condAppendToStringBuff(b);
      genMoveToLF(b, retVarName);

      AListAdd(&assignmentElement2, b, NULL, false, dataType, NULL);
    }

    // gen label to jump to the end
    genUnconditionalJump(endLabel);

    // gen bypass label
    genLabel(bypassLabel);



  // a variable, not a function
  }else{
    // gen jump to bypass
    char *bypassLabel = genLabelName("bypass");

    // gen label to eval expr
    char *evalLabel = genLabelName("eval");
    stringBufAppend(labelBuffer, evalLabel);

    // gen label to eval expr
    genLabel(evalLabel);

    TryCall(pExpr, &retVarName, &dataType);

    // gen label to jump to the end
    genUnconditionalJump(endLabel);

    // gen bypass label
    genLabel(bypassLabel);

    // Add the return value (result) to assignmentElement2
    AListAdd(&assignmentElement2, retVarName, NULL, false, dataType, NULL);
  }

  if(AListGetLength(assignmentElement) <= AListGetLength(assignmentElement2)){
    *assignmentDone = true;
  }

return 0;
}
    


/**
 * @brief
 *
 * @return error code
 *
 * 23. <statWithId>      -> , [id] <nextAssign> <expr> , <expr>
 * 24. <statWithId>      -> = <expr>
 * 25. <statWithId>      -> <fnCall>
 */
int pStatWithId(char *idName) {
  RuleFnInit;

  char *retname;
  GCMalloc(retname, sizeof(char) * 20);

  retVarCounter = 0;

  // -> <fnCall>
  if(STFind(symtab, idName) && !STGetIsVariable(symtab, idName)){
    TryCall(pFnCall, idName);
    vypluj 0;
  }

  // Not a function call but an assignment
  GetToken;
  printToken(token);
    
  // idName must be a variable
  if(!STFind(symtab, idName) || !STGetIsVariable(symtab, idName)){
      vypluj ERR(SYNTAX_ERR);
  }
    // -> = <expr>
  if (token->type == t_assignment) {
      char *destVarName;
      TryCall(STGetName, symtab, &destVarName, idName);
      // TODO check data types
      // Call the shift-reduce parser and assign the result to id2Var
      char *retVarName = NULL;
      int idType = STGetVarDataType(symtab, idName);
      TryCall(pExpr, &retVarName, idType);

      if(!retVarName){
        // If this is null, the expr was a function call and we need to fetch
        // the values from ret_0 (which is in TF!)
        sprintf(retname,"!ret_%d", retVarCounter);
        retVarCounter++;
        genMoveToLF(destVarName, retname);
      }else{
        // Otherwise the ret val is in LF
        genMove(destVarName, retVarName);
      }
    vypluj 0;
    } 
    
    // In idName we have a name of the first variable in this statement
    // In token->data we'll have a name of the second one
    // -> , [id] <nextAssign> <expr> , <expr>
    if (token->type == t_comma) {
      char *endLabel = genLabelName("ASSEND");

      char *name = NULL;
      STGetName(symtab, &name, idName);
      AListAdd(&assignmentElement, name, NULL, false, STGetVarDataType(symtab, idName), NULL);

      // [id]
      RequireTokenType(t_idOrKeyword);
      if (!STFind(symtab, token->data) 
          || !STGetIsVariable(symtab, token->data)) {
        vypluj ERR(SYNTAX_ERR);
      }

      STGetName(symtab, &name, token->data);
      AListAdd(&assignmentElement, name, NULL, false, STGetVarDataType(symtab, token->data), NULL);

      // [id]

      //AListDebugPrint(assignmentElement);
      bool assignmentDone = false;


      // <nextAssign>
      // Check and generate more assignments if there are some
      // TryCall(pNextAssign);
      GetToken;

      if(!assignmentDone){
        // <expr>
        TryCall(processExpr, &assignmentDone, endLabel);
      }


      if(!assignmentDone){
        // ,
        GetToken;
        if(token->type == t_comma){
          // <expr>
          TryCall(processExpr, &assignmentDone, endLabel);
        }
      }


<<<<<<< Updated upstream
      } else {
        genAssignLiteral(id2Var, -1, retVarName, "LF");
      }
      vypluj 0;
      // ,
      RequireTokenType(t_comma);

      // <expr>
      // Call the shift-reduce parser and assign the result to id1Var
      retVarName = NULL;
      //printf("IDEME GENEROVAŤ 1 ");
      AListGenerate(assignmentElement);
      TryCall(pExpr, &retVarName, type);
      if(retVarName == NULL) {
        vypluj ERR(SYNTAX_ERR);
=======
      // tu vygenerovať jumpy na labels v labelBuffer 
      int labelCount = labelBuffer->len;
      for(int i = labelCount - 1; i > 0; i--){
        char *label = labelBuffer->data[i];
        genUnconditionalJump(label);
>>>>>>> Stashed changes
      }

      // vygenerovať label endLabel
      genLabel(endLabel);

      // a priradenia 
      // hodnôt - v asselem sú idčká a v asselem2 sú results expr
      int varCount = AListGetLength(assignmentElement);
      for(int i = 1; i < varCount; i++) {
        genMove(AListGetElementByIndex(assignmentElement, i)->name,
        AListGetElementByIndex(assignmentElement2, i)->name);
      }
        


    } else {
      vypluj ERR(SYNTAX_ERR);
    }
  
  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 26. <nextAssign>      -> , [id] <nextAssign> <expr> ,
 * 27. <nextAssign>      -> =
 */
int pNextAssign(bool *assignmentDone, char *endLabel) {
  RuleFnInit;

  char *retname;
  GCMalloc(retname, sizeof(char) * 20);
  
  GetToken;
  printToken(token);

  // -> =
  if (token->type == t_assignment) {
    vypluj 0;
  }

  // -> , [id] <nextAssign> <expr> ,
  if(token->type != t_comma) {
    vypluj ERR(SYNTAX_ERR);
  }

  // [id]
  // The next token must be a variable
  RequireTokenType(t_idOrKeyword);
  if (!STFind(symtab, token->data) || !STGetIsVariable(symtab, token->data)) {
    vypluj ERR(SYNTAX_ERR);
  }
  char *name = NULL;
  STGetName(symtab, &name, token->data);
  AListAdd(&assignmentElement, name, NULL, false, STGetVarDataType(symtab, token->data), NULL);

  // <nextAssign>
  TryCall(pNextAssign, assignmentDone, endLabel);

  if(!assignmentDone){
    // <expr>
    TryCall(processExpr, assignmentDone, endLabel);
  }

  GetToken;
  if(token->type != t_comma){
    stashToken(&token);
  }

  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 29. <fnDefinitionParamTypeList>       -> eps
 * 30. <fnDefinitionParamTypeList>       -> [id] : [type] <nextFnDefinitionParamType>
 */
int pFnDefinitionParamTypeList(char *fnName) {
  RuleFnInit;
  int paramCount = 0;


  GetToken;
  printToken(token);

  // -> eps
  if(token->type != t_idOrKeyword || isIFJ21Keyword(token)) {
    TryCall(stashToken, &token);
    vypluj 0;
  }
  // TODO môže byť param definovaný? Ak áno, musí byť premenná. Ak nie, nemôže 
  // byť definovaný (to treba ocheckovať)

  // -> [id] : [type] <nextFnDefinitionParamType>

  // [id]
  // Append the name of the new parameter to the symtable
  TryCall(STAppendParamName, symtab, fnName, token->data);
  TryCall(STInsert, symtab, token->data);
  TryCall(STSetIsVariable, symtab, token->data, true);
  TryCall(STSetName, symtab, token->data, genVarName(token->data, symtab->top->depth));
  paramCount++;

  // :
  RequireTokenType(t_colon);

  // [type]
  // Append the data type of the parameter to the symtab
  TryCall(fnDefinitionParamType, fnName, paramCount);

  // <nextFnDefinitionParamType>
  vypluj pNextFnDefinitionParamType(fnName, paramCount);
}

/**
 * @brief
 *
 * @return error code
 *
 * 31. <nextFnDefinitionParamType>       -> eps
 * 32. <nextFnDefinitionParamType>       -> , [id] : [type] <nextFnDefinitionParamType>
 */
int pNextFnDefinitionParamType(char *fnName, int paramCount) {
  RuleFnInit;

  // ,
  GetToken;
  printToken(token);

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
  // TODO môže byť param definovaný? Ak áno, musí byť premenná. Ak nie, nemôže 
  // byť definovaný (to treba ocheckovať)
  RequireIDToken(token);
  paramCount++;
  // Append the name of the new parameter to the symtable
  TryCall(STAppendParamName, symtab, fnName, token->data);

  TryCall(STInsert, symtab, token->data);
  TryCall(STSetName, symtab, token->data, genVarName(token->data, symtab->top->depth));
  // TODO uncomment if it is correct
  // genVarDef(STGetName(symtab, token->data));

  // :
  RequireTokenType(t_colon);

  // [type]
  // Append the data type of the parameter to the symtab
  TryCall(fnDefinitionParamType, fnName, paramCount);

  // <nextFnDefinitionParamType>
  vypluj pNextFnDefinitionParamType(fnName, paramCount);
}

/**
 * @brief
 *
 * @return error code
 *
 * 34. <retArgList>      -> eps
 * 35. <retArgList>      -> <expr> <retNextArg>
 */
int pRetArgList(char *fnName) {
  RuleFnInit;

  char *retVarName = NULL;

  STElem *fn = STFind(symtab, fnName);


  // 34. <retArgList>      -> eps
  // Generate simple return with no arguments
  if(fn->fnRetTypesBuf->len == 0){
    genPopframe();
    genReturnInstruction();
    return 0;
  }

  // TODO If the function should return more than zero values, return nils
  // This will be tough... How do we know where the return ends???

  int expectedType = STFind(symtab, fnName)->fnRetTypesBuf->data[0];
  TryCall(pExpr, &retVarName, expectedType);

  // 35. <retArgList>      -> <expr> <retNextArg>
  int argCount = 1;

  // Code gen Define and assign the return values
  // Generate a new name where the return value will be written (in LF)
  char *retArgName = genRetVarName("");
  // Define the retArgName
  genVarDefLF(retArgName);
  // Pass from retArgName to retVarName
  genReturn(retArgName, retVarName);

  TryCall(pRetNextArg, fnName, argCount);

  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 36. <retNextArg>      -> eps
 * 37. <retNextArg>      -> , <expr> <retNextArg>
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
    genNilsReturn(STFind(symtab, fnName)->fnRetTypesBuf->len - argCount);
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
  genReturn(retArgName, retVarName);

  // <retNextArg>
  vypluj pRetNextArg(fnName, argCount);
}

/**
 *
 *
 * 39. <fnDeclarationParamTypeList> -> eps
 * 40. <fnDeclarationParamTypeList> -> [type] <nextFnDeclarationParamType>
 */
int pFnDeclarationParamTypeList(char *fnName) {
  RuleFnInit;

  GetToken;
  printToken(token);
  
  // 39. <fnDeclarationParamTypeList> -> eps 
  if(token->type == t_rightParen) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 40. <fnDeclarationParamTypeList> -> [type] <nextFnDeclarationParamType>
  LOG("-> [type] <nextFnDeclarationParamType>");
  int paramCount = 1;
  
  // [type]
  TryCall(fnDeclarationParamType, fnName, token->data);

  // tu musíme ocheckovať či token neni prava zatvorka alebo čo, idk

  // <nextFnDeclarationParamType>
  TryCall(pNextFnDeclarationParamType, fnName, paramCount);

  return 0;
}

/**
 *
 *
 * 41. <nextFnDeclarationParamType>   -> eps
 * 42. <nextFnDeclarationParamType>   -> , [type] <nextFnDeclarationParamType>
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

  TryCall(fnDeclarationParamType, fnName, token->data);
  
  TryCall(pNextFnDeclarationParamType, fnName, paramCount);

  return 0;
}

/**
 *
 *
 * 44. <fnRetTypeList>   -> eps 
 * 45. <fnRetTypeList>   -> : [type] <pNextRetType>
 */
int pFnRetTypeList(char *fnName) {
  RuleFnInit;

  GetToken;

  // 44. <fnRetTypeList>   -> eps
  if(token->type != t_colon) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 45. <fnRetTypeList>   -> : [type] <pNextRetType>
  TryCall(fnRetDataType, fnName);

  TryCall(pNextRetType, fnName);

  vypluj 0;

}

/**
 *
 *
 * 46. <pNextRetType>     -> eps
 * 47. <pNextRetType>     -> , [type] <nextRetType>
 */
int pNextRetType(char *fnName) {
  RuleFnInit;

  GetToken;

  // eps
  if(token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  TryCall(fnRetDataType, fnName);

  TryCall(pNextRetType, fnName);

  vypluj 0;

}

/**
 * @brief
 *
 * @return error code
 *
 * 49. <newIdAssign>     -> eps
 * 50. <newIdAssign>     -> = <expr>
 */
int pNewIdAssign(char *varName) {
  RuleFnInit;

  GetToken;
  printToken(token);

  // -> eps
  // =
  if (!(token->type == t_assignment && !strcmp(token->data, "="))) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName = NULL;
  int type = STGetVarDataType(symtab, varName);
  TryCall(pExpr, &retVarName, type);
  
  STElem *var = STFind(symtab, varName);
  if(!var) {
    vypluj ERR(SYNTAX_ERR);
  }

  if(!retVarName){
    // If this is null, the expr was a function call and we need to fetch
    // the values from ret_0 (which is in TF!)
    retVarName = "!ret_0";
    genMoveToLF(var->name, retVarName);
  }else{
    // Otherwise the ret val is in LF
    genMove(var->name, retVarName);
  }

  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 */
int pExpr(char **retVarName, int expectedType) {
  RuleFnInit;

  GetToken;
  printToken(token);

  // If it is a function call (and fn is defined), don't call the shift-reduce parser at all
  if(token->type == t_idOrKeyword && STFind(symtab, token->data) 
    && !STGetIsVariable(symtab, token->data)) {
    // check types
    LOG("WE IN \n");
    STElem *element =  STFind(symtab, token->data);
    

    // tikodime asi

    // int type = STGetRetType(symtab, token->data, 0); // TODO some magic for more returns
    // LOG("TYPE FROM PEXPR: %d", type);
    // if(type != expectedType) {
    //   vypluj ERR(ASS_ERR);
    // }
    
    genComment("Calling a function inside a function");
    TryCall(pFnCall, token->data);
    genComment("Function call inside a function done");
    
  // If it is a nil
  } else if(strEq(token->data, "nil")) {
    // Code gen define a var, assign nil and return the name in retVarName
    *retVarName = genTmpVarName();

    genAssignLiteral(*retVarName, dt_nil, "nil", "LF"); 

  } else if(strEq(token->data, "else")) {
    vypluj stashToken(&token); 

  // A keyword
  } else if(token->type == t_idOrKeyword && isIFJ21Keyword(token)) {
    vypluj ERR(SYNTAX_ERR);

  // An expression
  } else {
    int retVarType = -1;
    TryCall(parseExpression, symtab, token, retVarName, &retVarType);
    LOG("Expr result is in %s", *retVarName);
    // An expression is required but the precedence analysis didn't find any
    if(!(*retVarName)){
      vypluj ERR(SYNTAX_ERR);
    }
    if(retVarType == -1) {
      vypluj ERR(INTERN_ERR);
    }
    if(retVarType != expectedType && expectedType != dt_boolean) { // bool is weird
      LOG("TYPE FROM PA: %d EXPECTED: %d ", retVarType, expectedType);
      vypluj ERR(ASS_ERR);
    }
  }
  vypluj 0;
}

/*
 *
 * HELPER FUNCTIONS
 *
 */

/**
 * @brief appends to the string buffer unless the string is already there
 *
 * @return err code
 */
int condAppendToStringBuff(char *name) {
  if(!varDefBuff){
    return ERR(INTERN_ERR);
  }
  for(int i = 0; i < varDefBuff->len; i++) {
    if(strcmp(name, varDefBuff->data[i]) == 0) {
      vypluj 0;
    }
  }
  TryCall(stringBufAppend, varDefBuff, name)
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
  LOG();

  if (   strcmp(data, "string") == 0
      || strcmp(data, "integer") == 0
      || strcmp(data, "number") == 0
      || strcmp(data, "nil") == 0) {
    vypluj true;
  } else { 
    vypluj false;
  }
}

/**
 * @brief check if token is read function
 * 
 * @param data 
 * @return true if data is read function
 * @return false otherwise
 */
bool isReadFunction(char *data) {
  LOG();

  if(    strcmp(data, "readi") == 0 
      || strcmp(data, "reads") == 0 
      || strcmp(data, "readn") == 0) {
    vypluj true;
  } 
  vypluj false;
}

/**
 * @brief check if token is string operation function
 * 
 * @param data 
 * @return true if data is string operation function
 * @return false otherwise
 */
bool isStringOperationFunction(char *data) {
  if(    strcmp(data, "substr") == 0 
      || strcmp(data, "ord") == 0 
      || strcmp(data, "chr") == 0) {
    vypluj true;
  } 
  vypluj false;
}

/**
 * @brief check if token is read function in ifj21
 * 
 * @param token 
 * @return true if it is read function, destroys () behind read, generates code
 * 
 */
bool readFunction(Token *token) {
  // if token is not a keyword returns false and stashToken
  if(token->type != t_idOrKeyword) {
    TryCall(stashToken, &token);
    vypluj false;
  }

  // if token is not a read function, returns false and stashToken
  if(!isReadFunction(token->data)) {
    TryCall(stashToken, &token);
    vypluj false;
  }
  vypluj true;
}

/**
 * @brief Check if token is built in function
 *
 * @return if token is built in function return true, else false
 */
bool isBuiltInFunction(char *data) {
  if (   strcmp(data, "reads") == 0
      || strcmp(data, "readi") == 0
      || strcmp(data, "readn") == 0
      || strcmp(data, "write") == 0
      || strcmp(data, "tointeger") == 0
      || strcmp(data, "substr") == 0
      || strcmp(data, "ord") == 0
      || strcmp(data, "chr") == 0) {
    vypluj true;
  }
  vypluj false;
}

/**
 * @brief If the function wasn't already defiend, define it (add it to the
 * symtable)
 *
 * @param fnName: name of the function
 *
 * @return error code
 */
int newFunctionDefinition(Token *token) {
  if (STFind(symtab, token->data) 
      && !STGetIsVariable(symtab, token->data) 
      && STGetFnDefined(symtab, token->data)) {
    LOG("Function already defined");
    vypluj ERR(ID_DEF_ERR);
  } else if (isIFJ21Keyword(token)){
    LOG("Definition of a function with a keyword as a name");
    vypluj ERR(SYNTAX_ERR);
  } else {
    LOG("Adding to the symtable (definition)");
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
 * @return error code
 */
int newFunctionDeclaration(char *fnName) {
  if (STFind(symtab, fnName)) {
    LOG("Function already declared");
    vypluj ERR(1); // TODO errcode (declaration when fn is already declared/defined)
  } else {
    LOG("Adding declaration of %s (function) to the symtable", fnName);
    TryCall(STInsert, symtab, fnName);
    TryCall(STSetIsVariable, symtab, fnName, false);
    TryCall(STSetFnDeclared, symtab, fnName, true);
    TryCall(STSetName, symtab, fnName, genLabelName(""));
  }
  vypluj 0;
}

bool isLiteral(Token *token) {
  if(token->type == t_str
      || token->type == t_num
      || token->type == t_sciNum
      || token->type == t_int
      || strEq(token->data, "nil")) {
    vypluj true;
  }
  vypluj false;
}

/**
 *
 * @param fnName
 * @return
 */
int fnRetDataType(char *fnName){
  LOG("fnName: %s", fnName);

  Token *token = NULL;

  // Get the new data type
  GetToken;
  if(isDataType(token->data) == false) {
    LOG("A data type is required but we did not receive one");
    vypluj ERR(SYNTAX_ERR);
  }
  int dataType = getDataTypeFromString(token->data);

  retVarCounter++;

  // If the function was declared and we're defining it, check if params match
  if(STGetFnDeclared(symtab, fnName) && STGetFnDefined(symtab, fnName)){
    int dataTypeInSymtab = STGetRetType(symtab, fnName, retVarCounter - 1);
    // If they don't match -> err
    if(dataTypeInSymtab != dataType){
      LOG("Parameter types between fn decl and def don't match");
      vypluj ERR(PARAM_RET_ERR);
    }

  // Otherwise, append the new data type to the function in the symtab
  }else{
    TryCall(STAppendRetType, symtab, fnName, dataType);
  }

  vypluj 0;
}

int fnDeclarationParamType(char *fnName, char *data) {
  STElem *fn = STFind(symtab, fnName);
  // Check if fn is in ST
  if(!fn) {
    vypluj ERR(INTERN_ERR);
  }

  // Append the data type to the function in the symtable
  int dataType = getDataTypeFromString(data);
  TryCall(STAppendParamType, symtab, fnName, dataType); 

  vypluj 0;
}

/**
 * TODO comment
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

  // Otherwise, append the new data type to the function in the symtab
  }else{
    TryCall(STAppendParamType, symtab, fnName, dataType);
  }

  // Set the data type of the variable we created for passing the param
  char *varName;
  TryCall(STGetParamName, symtab, &varName, fnName, paramCount - 1);
  TryCall(STSetVarDataType, symtab, varName, dataType);

  return 0;
}

int varDataType(char *varName){
  Token *token;
  STElem *varElement = STFind(symtab, varName);

  // if variable is not variable or doesnt exist
  if(varElement == NULL) {
    vypluj ERR(ID_DEF_ERR);
  }

  GetToken;
  
  if(isDataType(token->data) == false) {
    vypluj ERR(-1); // TODO change err code
  }

  varElement->varDataType = getDataTypeFromString(token->data);

  vypluj 0;
}

// TODO remove this???
int stringFunctions(char *varName) {
  Token *token = NULL;

  GetToken;
  printToken(token);

  if(strcmp(token->data, "substr") == 0) {

    RequireTokenType(t_leftParen);

    GetToken;
    Token *string = NULL;
    double i;
    double j;
    char *endPtr;

    if(token->type != t_str || token->type != t_idOrKeyword) {
      vypluj ERR(-1); // TODO CHANGE ERR CODE
    }

    string = token;

    GetToken;

    if(token->type != t_int || token->type != t_num) {
      vypluj ERR(-1); // TODO CHANGE ERR CODE
    }

    i = strtod(token->data, &endPtr);


    GetToken;

    if(token->type != t_int || token->type != t_num) {
      vypluj ERR(-1); // TODO CHANGE ERR CODE
    }

    j = strtod(token->data, &endPtr);


    RequireTokenType(t_rightParen);

    genSubstrFunction(varName, string, i, j, symtab->top->depth);

    vypluj 0;

  }

  vypluj ERR(1);

}

int createParamVariables(char *fnName){
  // Get the function element from the symtable
  STElem *fnElement = STFind(symtab, fnName);
  if(!fnElement){
    return ERR(SYNTAX_ERR);
  }
  // Generate definitions of the parameter variables
  if(fnElement->fnParamNamesBuf == NULL) {
    vypluj 0;
  }
  int paramCount = fnElement->fnParamNamesBuf->len;
  
  
  for(int i = 0; i < paramCount; i++){
    char *paramName;
    TryCall(STGetParamName, symtab, &paramName, fnName, i);
    STElem *paramVar = STFind(symtab, paramName);
    TryCall(condAppendToStringBuff, paramVar->name);

    // Assign parameters to the defined variables
    genMove(paramVar->name, genParamVarName(""));
  }
  vypluj 0;
}

int declareVariable(){
  // TODO declare all variables at the end of file? (instead of calling DEFVAR
  // instantly, just call this function which saves the variable name to 
  // a string buffer and at the end of the program we define all variables??
  return 0;
}


#endif
/* end of file parser.c */
