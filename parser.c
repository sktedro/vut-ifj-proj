/*
 * Parser
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
int assigmentCounter = 0;
int assigmentGeneratedCounter = 0;
int retVarCounter = 0;
int paramhelpCounter = 0;

// TODO macro RequireIDToken - == t_idOrKeyword && !isIFJ21Keyword

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
  AListInit(&assignmentElement);

  // require
  RequireToken(t_idOrKeyword, "require");

  // <req>
  TryCall(pReq);

  // <codeBody>
  TryCall(pCodeBody);

  vypluj 0;
}

/**
 * @brief Rule for <req>
 *
 * @return error code
 *
 * 02. <req>             -> "ifj21"
 */
int pReq() {
  RuleFnInit;

  // "ifj21"
  RequireToken(t_str, "ifj21");

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

  // 05. <codeBody>        -> function [id] ( <fnDefinitionParamTypeList> ) <fnRetTypeList> <stat> end <codeBody>
  if(strcmp(token->data, "function") == 0) {

    genComment("New function definition");

    // Code gen Create an unconditional jump behind the function
    char *fnBypassLabelName = genLabelName();
    genUnconditionalJump(fnBypassLabelName);

    // [id]
    RequireTokenType(t_idOrKeyword);
    char *fnName = token->data;

    // Define the new function (in the symtable)
    TryCall(newFunctionDefinition, fnName);

    // Generate code
    genFnDef(fnName);

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

    // <stat>
    TryCall(pStat, fnName);

    // end
    RequireToken(t_idOrKeyword, "end");

    // Return from the function
    genPopframe();
    genReturnInstruction();

    // Code gen Create a label here, behind the function
    genLabel(fnBypassLabelName);

    genComment2("Function definition done");

    // <codeBody>
    TryCall(pCodeBody);
  }

  // 06. <codeBody>        -> global [id] : function ( <fnDeclarationParamTypeList> ) <fnRetTypeList> <codeBody>
  else if(strcmp(token->data, "global") == 0) {

    genComment("New function declaration");

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

    genComment2("Function declaration done");

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

 
  
  
  genFnCallInit();

  genComment("Processing function call arguments");
  TryCall(pFnCallArgList, fnName);
  genComment2("Processing function call arguments done");

  // )
  RequireTokenType(t_rightParen);
  
  // Get the function element from the symtable
  STElem *fnElement = STFind(symtab, fnName);
  if(!fnElement){
    return ERR(SYNTAX_ERR);
  }

  genFnCall(fnName);
  retVarCounter = 0;
  paramhelpCounter = 0;
  assigmentCounter = 0;
  assigmentGeneratedCounter = 0;

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

  // -> eps
  if(token->type == t_rightParen) {
    LOG("-> eps");

    STElem *fn = STFind(symtab, fnName);
    // Amount of arguments doesn't match
    if(fn->fnParamTypesBuf && fn->fnParamTypesBuf->len != 0){
      LOG("Param amount doesn't match\n");
      vypluj ERR(SYNTAX_ERR);
    }
    
     // Stash the token
     vypluj stashToken(&token);
  } 

  /*if(isBuiltInFunction("write")){
    writeFunction(token);
  }*/ //idk či to je dobre ale tu niekde to má asi byť 

  // -> <fnCallArg> <nextFnCallArg>

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
  printToken(token);
  
  // -> , <fnCallArg> <nextFnCallArg>
  // ,
  if(token->type == t_comma) {
    argCount++;

    // <fnCallArg>
    TryCall(pFnCallArg, fnName, argCount);
    
    // <nextFnCallArg>
    TryCall(pNextFnCallArg, fnName,argCount);

  // -> , <fnCallArg> <nextFnCallArg>
  }else{

    STElem *fn = STFind(symtab, fnName);

    // Amount of arguments doesn't match
    if(!fn->fnParamTypesBuf || fn->fnParamTypesBuf->len != argCount){
      LOG("Param amount doesn't match\n");
      vypluj ERR(SYNTAX_ERR);
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
  printToken(token);

  STElem *fn = STFind(symtab, fnName);

  int dataType = -1;

  // -> [id]
  if (STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)) {
    LOG("-> [id]\n");
    dataType = STGetVarDataType(symtab, token->data);
    // Pass the parameter by MOVE TF@paramName LF@token->data
    //char *paramName = fn->fnParamNamesBuf->data[argCount - 1]; UNUSED VARIABLE
    // TODO shouldn't we use this?

    char *name;
    GCMalloc(name, sizeof(char) * 100);
    sprintf(name, "$%d", paramhelpCounter);
    genVarDefTF(name);
    genPassParam(name, token->data);
    paramhelpCounter++;

  // -> [literal]
  } else if (token->type == t_int || token->type == t_num ||
      token->type == t_sciNum || token->type == t_str) {
    LOG("-> [literal]\n");
    if(token->type == t_int){
      dataType = dt_integer;
    }else if(token->type == t_num || token->type == t_sciNum){
      dataType = dt_number;
    }else if(token->type == t_str){
      dataType = dt_string;
    }

    char *name;
    GCMalloc(name, sizeof(char) * 100);
    sprintf(name, "$%d", paramhelpCounter);
    genVarDefTF(name);
    genVarAssign(name, dataType, token->data, "TF");
    paramhelpCounter++;

  } else {
    LOG("NENI TO ANI PREMENNÁ A ANI LITERÁL\n");
    if(token->type == t_idOrKeyword) {
      vypluj ERR(ID_DEF_ERR);
    }
    LOG("Function call argument is not an ID nor literal");
    vypluj ERR(SYNTAX_ERR);
  }

  // A parameter data type doesn't match
  if(STGetParamType(symtab, fnName, argCount - 1) != dataType){
    LOG("Param data type doesn't match\n");
    vypluj ERR(SYNTAX_ERR);
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

  // -> eps
  if (token->type != t_idOrKeyword || (token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
    fprintf(stderr, " <stat>            -> eps\n");
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> local [id] : <type> <newIdAssign> <stat>
  if (strcmp(token->data, "local") == 0) {

    fprintf(stderr, "-> local [id] : <type> <newIdAssign> <stat>\n");
    // [id]
    RequireTokenType(t_idOrKeyword);
    if(isIFJ21Keyword(token)) {
      vypluj ERR(SYNTAX_ERR);
    }

    char *newVarName = token->data;
    // Insert the new ID to the symtable
    TryCall(STInsert, symtab, newVarName);
    STSetIsVariable(symtab, newVarName, true);
    STSetName(symtab, newVarName, genName(token->data, symtab->top->depth));
    // Code gen variable definition
    genVarDefLF(STGetName(symtab, newVarName));

    // :
    RequireTokenType(t_colon);
    
    // [type]
    TryCall(varDataType, newVarName);
    
    // <newIdAssign>
    TryCall(pNewIdAssign, newVarName);

  // -> if <expr> then <stat> else <stat> end <stat>
  } else if (strcmp(token->data, "if") == 0) {
    fprintf(stderr, "-> if <expr> then <stat> else <stat> end <stat>\n");
    // Generate new label names for 'else' and 'end' (don't generate code yet)
    char *elseLabelName = genLabelName(), *endLabelName = genLabelName();

    // <expr>
    char *exprResultVarName = NULL;
    TryCall(pExpr, &exprResultVarName);

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

  // -> while <expr> do <stat> end <stat>
  } else if (strcmp(token->data, "while") == 0) {
    fprintf(stderr, "-> while <expr> do <stat> end <stat>\n");
    // Generate new label names (don't generate code yet)
    char *whileLabelName = genLabelName();
    char *startLabelName = genLabelName();
    char *endLabelName = genLabelName();

    // Code gen 'start' label
    genLabel(startLabelName);

    /*
     * TODO
     */
    // TODO Treba toto robiť nejak na novom frame??? lebo sa budú každým cyklom 
    // definovať a deklarovať nové tmp premenné
    // !!! We can't call vardef on the same variable (in one frame) more than once !!!
    // Maybe we can't `vardef_i vardef_i` but we can do `label vardef_i jump_label` ?? :shrunk:
    // <expr>
    char *exprResultVarName = NULL;
    TryCall(pExpr, &exprResultVarName);

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

  // -> return <retArgList> <stat>
  } else if (strcmp(token->data, "return") == 0) {
    LOG("-> return <retArgList> <stat>\n");

    genComment("Return encountered - returning");

    // Code gen POPFRAME
    genPopframe();

    // <retArgList>
    if(strcmp(token->data, "return") == 0) {
      TryCall(pRetArgList, fnName);
    }

    // Code gen RETURN from the function
    genReturnInstruction();

    genComment2("Return done");

  // [id] <statWithId> <stat> 
  } else if (STFind(symtab, token->data)) { // A function or a variable
    LOG("<stat>            -> [id] <statWithId> <stat>\n");
  
    // TODO kopnite Alexa ak na to ešte nespravil funkciu
    if(assignmentElement->name == NULL) {
      assignmentElement->name = STGetName(symtab, token->data);
      assignmentElement->first = true;
      assignmentElement->label = getExprLabelName(0);
      assignmentElement->prev = NULL;
      assignmentElement->next = NULL;
      assignmentElement->end = getExprEndName();
      assignmentElement->generated = false;
    }

    // <statWithId>
    TryCall(pStatWithId, token->data);

  // -> eps
  } else {
    LOG("-> eps");
    TryCall(stashToken, &token);
    vypluj 0;
  }

// <stat>
vypluj pStat(fnName);  
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

  // -> <fnCall>
  if(STFind(symtab, idName) 
      && !STGetIsVariable(symtab, idName)){
    vypluj pFnCall(idName);

  // Not a function call but an assignment
  }else{

    // Get new token (to see if it is a ',' of '=')
    GetToken;
    printToken(token);

    // idName must be a variable
    if(!STFind(symtab, idName) || !STGetIsVariable(symtab, idName)){
      vypluj ERR(SYNTAX_ERR);
    }

    // -> = <expr>
    if (token->type == t_assignment) {
      // Call the shift-reduce parser and assign the result to id2Var
      char *retVarName = NULL;
      TryCall(pExpr, &retVarName);
      
      //genVarAssign(idName, -1, retVarName); TODO commented cus segfault
    }
    
    // -> , [id] <nextAssign> <expr> , <expr>
    // In idName we have a name of the first variable in this statement
    // In token->data we'll have a name of the second one
     else if (token->type == t_comma) {
      
      if(AListGetLength(assignmentElement) == 0) {
        assignmentElement->name = STGetName(symtab, idName);
        assignmentElement->end = getExprEndName();
        assignmentElement->first = true;
        assignmentElement->label = getExprLabelName(assigmentCounter);
        assignmentElement->next = NULL;
        assigmentCounter++;
      }
      // [id]
      RequireTokenType(t_idOrKeyword);
      if (!STFind(symtab, token->data) 
          || !STGetIsVariable(symtab, token->data)) {
        vypluj ERR(SYNTAX_ERR);
      }

      // Get their names (in the generated code)
      char *id1Var = STGetName(symtab, idName);
      char *id2Var = STGetName(symtab, token->data);


      AListAdd(&assignmentElement, id2Var, getExprLabelName(assigmentCounter), false, NULL);
      assigmentCounter++;
      // <nextAssign>
      // Check and generate more assignments if there are some
      TryCall(pNextAssign);

      // <expr>
      // Call the shift-reduce parser and assign the result to id2Var
      char *retVarName = NULL;
      TryCall(pExpr, &retVarName);
      genVarAssign(id2Var, -1, retVarName, "LF");

      // ,
      RequireTokenType(t_comma);

      // <expr>
      // Call the shift-reduce parser and assign the result to id1Var
      retVarName = NULL;
      TryCall(pExpr, &retVarName);
      genVarAssign(id1Var, -1, retVarName, "LF");

    }else{
      vypluj ERR(SYNTAX_ERR);
    }
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
int pNextAssign() {
  RuleFnInit;

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

  AListAdd(&assignmentElement, STGetName(symtab, token->data), getExprLabelName(assigmentCounter), false, NULL);
  assigmentCounter++;

  // <nextAssign>
  TryCall(pNextAssign);

  // <expr>
  // Evaluate the expression and MOVE from the retVarName to varName
  /*char *varName = STGetName(symtab, token->data);*/

  if(assigmentGeneratedCounter == 0) {
    genExprFirst(assignmentElement);
  }

  char *varName = AListGetElementByIndex(assignmentElement, assigmentGeneratedCounter)->name;
  char *retVarName;
  TryCall(pExpr, &retVarName);
  printf("WE BACK\n");
  genMove(varName, retVarName);

  // ','
  RequireTokenType(t_comma);

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
  STAppendParamName(symtab, fnName, token->data);
  TryCall(STInsert, symtab, token->data);
  STSetIsVariable(symtab, token->data, true);
  STSetName(symtab, token->data, genName(token->data, symtab->top->depth));
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
  RequireTokenType(t_idOrKeyword);
  if(isIFJ21Keyword(token)){
    vypluj ERR(SYNTAX_ERR);
  }
  paramCount++;
  // Append the name of the new parameter to the symtable
  STAppendParamName(symtab, fnName, token->data);

  TryCall(STInsert, symtab, token->data);
  STSetName(symtab, token->data, genName(token->data, symtab->top->depth));
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

  TryCall(pExpr, &retVarName);

  // 34. <retArgList>      -> eps
  // If the pExpr gave us no retVarName, token wasn't an expression
  if(!retVarName){
    // But if the function needs more than zero parameters, it's a syntax err
    if(fn->fnRetTypesBuf->len != 0){
      LOG("A function requires some return arguments but we received 0");
      return ERR(SYNTAX_ERR);
    // Otherwise all is good
    }else{
      return 0;
    }
  }

  // 35. <retArgList>      -> <expr> <retNextArg>
  int argCount = 1;

  // Code gen Pass the return values down by one frame
  // Generate a new name where the return value will be written (in LF)
  char *retArgName = genRetName();
  // Define the retArgName
  genVarDefLF(retArgName);
  // Pass from TF (retArgName) to LF (retVarName)
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
    if(argCount != STFind(symtab, fnName)->fnRetTypesBuf->len){
      LOG("The amount of return arguments is not right");
      return ERR(SYNTAX_ERR);
    }
    // Otherwise just stash that token (this is the end of the return stat)
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> , <expr> <retNextArg>
  argCount++;
  char *retVarName = NULL;

  // <expr>
  TryCall(pExpr, &retVarName);
  if(!retVarName){
    // No expr found after the comma!
    LOG("An expression is required but none was received");
    return ERR(SYNTAX_ERR);
  }

  // Code gen Pass the return values down by one frame
  // Generate a new name where the return value will be written (in LF)
  char *retArgName = genRetName();
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
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();

  Token *token;
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

  // -> = <exprList>
  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName = NULL;
  TryCall(pExpr, &retVarName);
  
  STElem *var = STFind(symtab, varName);
  if(!var) {
    vypluj ERR(SYNTAX_ERR);
  }

  genMove(var->name, retVarName);

  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 */
int pExpr(char **retVarName) {
  RuleFnInit;

  GetToken;
  printToken(token);

  // If it is a function call (and fn is defined), don't call the shift-reduce parser at all
  if(token->type == t_idOrKeyword && STFind(symtab, token->data) 
    && !STGetIsVariable(symtab, token->data) && STGetFnDefined(symtab, token->data)) {
 
    genComment("Calling a function call inside a function");
    TryCall(pFnCall, token->data);
    genComment("Function call inside a function done");
    
  // If it is a nil
  } else if(strEq(token->data, "nil")) {
    // Code gen define a var, assign nil and return the name in retVarName
    *retVarName = genTmpVarDef();
    // TODO what to insert as the last param? (char *frame)
    genVarAssign(*retVarName, dt_nil, "nil", "TODO"); 

  } else if(strEq(token->data, "else")) {
    vypluj stashToken(&token); 

  // A keyword
  } else if(token->type == t_idOrKeyword && isIFJ21Keyword(token)) {
    vypluj ERR(SYNTAX_ERR);

  // An expression
  } else {
    TryCall(parseExpression, symtab, token, retVarName);
  }
  vypluj 0;
}

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
bool isDataType(char *data) {
  fprintf(stderr, "-----------------------------------------------------------\n");
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
  fprintf(stderr, "-----------------------------------------------------------\n");
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
  
  // TODO don't use that stupid global 'element'! Streľba do nohy
  // TryCall(genVarAssign, STGetName(symtab, element->data), element->dataType, token->data, "LF");
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
int newFunctionDefinition(char *fnName) {
  if (STFind(symtab, fnName) 
      && !STGetIsVariable(symtab, fnName) 
      && STGetFnDefined(symtab, fnName)) {
    LOG("Function already defined");
    vypluj ERR(ID_DEF_ERR);
  } else {
    LOG("Adding to the symtable (definition)");
    TryCall(STInsert, symtab, fnName);
    STSetIsVariable(symtab, fnName, false);
    STSetFnDefined(symtab, fnName, true);
    STSetName(symtab, fnName, genLabelName());
    
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
    STSetIsVariable(symtab, fnName, false);
    STSetFnDeclared(symtab, fnName, true);
    STSetName(symtab, fnName, genLabelName());
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
 * @brief Function for built in functions
 *
 * @return error code
 * 09. <fnCall>          -> ( <fnCallArgList> )
 *  
 */
int writeFunction(Token *token) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();

  // (
  RequireTokenType(t_leftParen);

  
  // <fnCallArgList> but somehow special for write
  GetToken;

  STElem *element;
  // char *name; TODO UNUSED VARIABLE

  // TODO alex tvrdí že to nakódí
  
  if(isLiteral(token)) {
    genWriteLiteral(token, "LF");
  } else {
    element = STFind(symtab, token->data);

    if(element == NULL) {
      vypluj ERR(SYNTAX_ERR); // TODO check if good err code
    }
    genWriteVariable(element->name, "LF");
        
  }

  GetToken;
  

  vypluj 0;
}

/**
 *
 * @param fnName
 * @return
 */
int fnRetDataType(char *fnName){
  fprintf(stderr, "-----------------------------------------------------------\n");
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
      vypluj ERR(SYNTAX_ERR);
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
  fprintf(stderr, "-----------------------------------------------------------\n");
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
      return ERR(SYNTAX_ERR);
    }

  // Otherwise, append the new data type to the function in the symtab
  }else{
    TryCall(STAppendParamType, symtab, fnName, dataType);
  }

  // Set the data type of the variable we created for passing the param
  char *varName;
  TryCall(STGetParamName, symtab, &varName, fnName, paramCount - 1);
  STSetVarDataType(symtab, varName, dataType);

  return 0;
}

int varDataType(char *varName){
  fprintf(stderr, "TYPEVAR-----------------------------------------------\n");
  fprintf(stderr, "varname : %s \n", varName);

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
    STGetParamName(symtab, &paramName, fnName, i);
    if(!paramName){
      vypluj ERR(SYNTAX_ERR);
    }
    STElem *paramVar = STFind(symtab, paramName);
    genVarDefLF(paramVar->name);

    // Assign parameters to the defined variables
    genMove(paramVar->name, genParamName());
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
