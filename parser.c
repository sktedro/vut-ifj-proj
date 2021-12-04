/*
 * Parser
 */

/** IMPORTANT !!! READ FIRST, BEFORE EDITING
 *
 * --------------------------------------------------
 * TOP PRIORITY
 * TODO když dostaneme EOF (token == NULL) někde random uprostřed pravidla
 * WE NEED MORE STATES FOR BUILT IN FUNCTIONS
 * ADD THEM TO OUR CFG TOO !!!!!!!!!!!!
 * --------------------------------------------------
 *
 * 1. code in testing
 *
 * --------------------------------------------------
 * Function             State
 * --------------------------------------------------
 * pStart             FULLY DONE
 * pReq               FULLY DONE
 * pCodeBody          seems like done
 * pFnCall            Built in functions! Other than that, it's done
 * pFnCallArgList     FULLY DONE (minor TODOs)
 * pNextFnCallArg     FULLY DONE (minor TODOs)
 * pFnCallArg         FULLY DONE (hopefully) (TODO errcode)
 * pStat              how to program the while cycle?
 * pStatWithId        might work
 * pNextAssign        might work
 * pFnArgList         might work but dependant on type
 * pNextFnArg         might work but dependant on type
 * pRetArgList        
 * pRetNextArg        
 * pFnParamTypeList 
 * pNextParamType   
 * pFnRetTypeList   
 * pNextRetType     
 * pNewIdAssign
 * pExpr              could work


* -----------------------------------------------------------------------
 */

#ifndef PARSER_C
#define PARSER_C

#include "parser.h"


extern int ret;

STStack *symtab;

// SStackElem stuff, just ignore TODO move somewhere?
SStackElem *element;
void cleanElement() {
  element->next->data = NULL;
  element->data = NULL;
}
void initElement() {
  GCMalloc(element, sizeof(SStackElem));
  GCMalloc(element->data, sizeof(char) * 100);
}

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
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;
  initElement();

  // require
  RequireToken(t_idOrKeyword, require);

  // <req>
  TryCall(pReq);

  // <codeBody>
  TryCall(pCodeBody);

  return 0;
}

/**
 * @brief Rule for <req>
 *
 * @return error code
 *
 * 02. <req>             -> "ifj21"
 */
int pReq() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // "ifj21"
  RequireToken(t_str, ifj21);

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
 * 05. <codeBody>        -> function [id] ( <fnArgList> ) <fnRetTypeList> <stat> end <codeBody>
 * 06. <codeBody>        -> global [id] : function ( <fnParamTypeList> ) <fnRetTypeList> <codeBody>
 * 07. <codeBody>        -> [id] <fnCall> <codeBody>
 */
int pCodeBody() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();

  Token *token = NULL;
  TryCall(scanner, &token);

  // If it is not EOF, it has to be an ID or a keyword
  if(token && token->type != t_idOrKeyword){
    vypluj err(SYNTAX_ERR);

  // -> eps
  } else if(!token) {
    return 0;
  }

  // -> function [id] ( <fnArgList> ) <fnRetTypeList> <stat> end <codeBody>
  if(strcmp(token->data, "function") == 0) {
    // [id]
    RequireTokenType(t_idOrKeyword);
    char *fnName = token->data;

    // Define the new function (in the symtable)
    TryCall(newFunctionDefinition, fnName);

    // Generate code
    genFnDef(fnName);

    // (
    RequireTokenType(t_leftParen);

    // <fnArgList>
    TryCall(pFnArgList, fnName);

    // )
    RequireTokenType(t_rightParen);

    // <TODO>

    // <stat>
    TryCall(pStat, fnName);

    TryCall(scanner, &token);
    printToken(token);

    if(strcmp(token->data, "end") != 0) {
      vypluj err(1);
    }

    genFnDefRet();

    // TODO asi RequireToken nefunguje po stash token alebo netuším prečo to neberie end - alex
    //RequireToken(t_idOrKeyword, "end");
    fprintf(stderr, "HAHAHA\n");
    // <codeBody>
    TryCall(pCodeBody);
  }

  // -> global [id] : function ( <fnParamTypeList> ) <fnRetTypeList> <codeBody>
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

    // <typeList>
    //TryCall(pTypeList, fnName);
    // TODO REDONE
    // )
    RequireTokenType(t_rightParen);

    // <TODO>

    // <codeBody>
    TryCall(pCodeBody);
  }

  // -> [id] <fnCall> <codeBody>
  else if(token->type == t_idOrKeyword) {

    // The function name must be in the symtab, must be a function
    if(!STFind(symtab, token->data) || STGetIsVariable(symtab, token->data)){
      LOG("Calling an undeclared function or maybe it is not even a fn");
      vypluj err(SYNTAX_ERR);
    }

    // <fnCall>
    TryCall(pFnCall, token->data);

    //Delete this, this just bypass fnCall
    //---------------------------------------
    /*TryCall(scanner, &token);
    printToken(token);

    while(token->type != t_rightParen) {
      TryCall(scanner, &token);
      printToken(token);
    }*/
    //---------------------------------------

    // <codeBody>
    TryCall(pCodeBody);

  } else {
    vypluj err(SYNTAX_ERR);
  }

  vypluj 0;
}

/**
 * @brief Pushes necessary arguments to the TF from the LF (or are we using stack?)
 *
 * @return error code
 *
 * 09. <fnCall>          -> ( <fnCallArgList> )
*/
int pFnCall(char *fnName) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  if(isBuiltInFunction(fnName)) {
    TryCall(builtInFunctions, fnName);
    vypluj 0;
  } else {

    RequireTokenType(t_leftParen);
    
    STElem *fnElement = STFind(symtab, fnName);
    STElem *fnParam;
    int paramCnt = 0;
    
    while(token->type != t_rightParen) {
      TryCall(scanner, &token);

      if(token->type != t_rightParen) {
        
        // if token is not a keyword and is [ID] -> it will be variable
        
        // IDK      
        
      }


    }

    vypluj 0;
  }

  /*
  TODO toto je tu len presunuté z pExpr - všetko ohľadom volania funkcií sa handluje tu asi
  */
  // TODO read funkcie spraviť nejak normálne nie ako imbecil :peepoGiggle: - written by Tedro
  /*if(isReadFunction(token->data)) {
    // asi bude treba overiť či táto gen funkcie nenarazili na err
    genReadFunction(STGetName(symtab, element->data), token->data);

    RequireTokenType(t_leftParen);
    RequireTokenType(t_rightParen);
    vypluj 0;
  } else if(isStringOperationFunction(token->data)) {
    // ;
    if(strcmp(token->data, "substr") == 0) {
      Token *stringTMP;
      Token *start;
      Token *end;

      TryCall(scanner, &token);

      RequireTokenType(t_leftParen);
      
      TryCall(scanner, &token);
      
      if(token->type != t_str || token->type != t_idOrKeyword) {
        vypluj err(1); // TODO ADD ERR CODE
      }

      stringTMP = token;

      TryCall(scanner, &token);
      
      if(token->type != t_str || token->type != t_idOrKeyword) {
        vypluj err(1); // TODO ADD ERR CODE
      }

      start = token;

      TryCall(scanner, &token);
      
      if(token->type != t_str || token->type != t_idOrKeyword) {
        vypluj err(1); // TODO ADD ERR CODE
      }

      end = token;

      TryCall(scanner, &token);
      RequireTokenType(t_rightParen);

      // CALL GEN SUBSTR CALL FUNSTION

      vypluj 0;
    
    } else if(strcmp(token->data, "ord") == 0) {
        Token *string;
        Token *i;
        
        RequireTokenType(t_leftParen);
        
        TryCall(scanner, &token);

        if(token->type != t_str || token->type != t_idOrKeyword) {
          vypluj err(1); //TODO ADD ERR
        }

        string = token;

        TryCall(scanner, &token);

        if(token->type != t_int || token->type != t_num || token->type != t_idOrKeyword) {
          vypluj err(1);
        }

        i = token;

        RequireTokenType(t_rightParen);

        //TODO CALL GEN ORD CALL FUNCTION

    } else if(strcmp(token->data, "chr") == 0) {
        Token *i;
        RequireTokenType(t_leftParen);

        TryCall(scanner, &token);

        i = token;
        
        RequireTokenType(t_rightParen);

        // TODO ADD GEN CHR CALL FUNCTION

    } else {
        vypluj err(-1); // TODO INTERNAL ERROR
  }
  

  // Code gen: create a temporary frame
  genFnCallInit();*/

  // (
  //RequireTokenType(t_leftParen);

  // <fnCallArgList>
  //TryCall(pFnCallArgList, fnName);
  
  // )
  //RequireTokenType(t_rightParen);

  // Code gen: call the function
  //genFnCall(fnName);

  //vypluj 0;
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
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // Functions can have several args (params) - this helps keep track of which
  // one we're processing right now (will be passed as a param to nextFnCallArg)
  int argCount = 0;

  TryCall(scanner, &token);
  printToken(token);

  // -> eps
  if(token->type == t_rightParen) {
    LOG("-> eps");

    STElem *fn = STFind(symtab, fnName);
    // Amount of arguments doesn't match
    if(fn->fnParamTypesBuf && fn->fnParamTypesBuf->len != 0){
      LOG("Param amount doesn't match\n");
      vypluj err(SYNTAX_ERR); // TODO errcode
    }
    
     // Stash the token
     TryCall(stashToken, &token);
     LOG("BAF");
  } 

  // -> <fnCallArg> <nextFnCallArg>
  else{
    LOG("-> <fnCallArg> <nextFnCallArg>\n");
    argCount++;

    TryCall(stashToken, &token); // cause we didn't actually process the first arg

    // <fnCallArg>
    TryCall(pFnCallArg, fnName, argCount);

    // <nextFnCallArg>
    TryCall(pNextFnCallArg, fnName, argCount);
  }
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
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
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

    // TODO remove this later
    /*if(!fn || !fn->fnParamTypesBuf || !fn->fnParamTypesBuf->data){
      LOG("Special error\n");
      return 1;
    }*/

    // Amount of arguments doesn't match
    if(!fn->fnParamTypesBuf || fn->fnParamTypesBuf->len != argCount){
      LOG("Param amount doesn't match\n");
      vypluj err(SYNTAX_ERR); // TODO errcode
    }

    TryCall(stashToken, &token);
  }

  return 0;
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
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;
  TryCall(scanner, &token);
  printToken(token);

  STElem *fn = STFind(symtab, fnName);

  // TODO remove this later
  /*if(!fn){
    LOG("Special error\n");
    return 1;
  }*/

  int dataType = -1;

  // -> [id]
  if (STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)) {
    LOG("-> [id]\n");
    dataType = STGetVarDataType(symtab, token->data);
    // Pass the parameter by MOVE TF@paramName LF@token->data
    char *paramName = fn->fnParamNamesBuf->data[argCount - 1];
    genPassParam(paramName, token->data, symtab->top->depth);

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

    // Define it in TF (generate code) TODO commented cus segfault
    /*char *paramName;
    STGetParamName(symtab, &paramName, fnName, argCount - 1);
    genVarDef(paramName, symtab->top->depth + 1);
    genVarAssign(paramName, symtab->top->depth + 1, dataType, token->data);*/

  } else {
    LOG("NENI TO ANI PREMENNÁ A ANI LITERÁL\n");
    if(token->type == t_idOrKeyword) {
      vypluj err(ID_DEF_ERR);
    }
    vypluj err(SYNTAX_ERR);
  }

  // TODO remove this later
  /*if(!fn->fnParamTypesBuf || !fn->fnParamTypesBuf->data){
    LOG("Special error\n");
    return 1;

  }*/

  // A parameter data type doesn't match
  if(STGetParamType(symtab, fnName, argCount -1) != dataType){
    LOG("Param data type doesn't match\n");
    vypluj err(SYNTAX_ERR); // TODO errcode
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
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_idOrKeyword) {
    fprintf(stderr, " <stat>            -> eps\n");
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> local [id] : <type> <newIdAssign> <stat>
  // Should be done - just finish the pType and TODO above it
  if (strcmp(token->data, "local") == 0) {
    fprintf(stderr, "-> local [id] : <type> <newIdAssign> <stat>\n");
    // [id]
    RequireTokenType(t_idOrKeyword);
    char *newVarName = token->data;
    // Insert the new ID to the symtable
    TryCall(STInsert, symtab, newVarName);
    STSetIsVariable(symtab, newVarName, true);
    STSetName(symtab, newVarName, genName(token->data, symtab->top->depth));
    // Code gen variable definition
    genVarDef(STGetName(symtab, newVarName));

    // :
    RequireTokenType(t_colon);

    // <type>
    // TODO set data type (in the following function, I suppose)
    TryCall(typeVar, newVarName);
    
    // <newIdAssign>
    TryCall(pNewIdAssign);

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
    printToken(token);

    // <retArgList>
    if(strcmp(token->data, "return") == 0) {
      TryCall(pRetArgList);
      vypluj 0;
    }

  // [id] <statWithId> <stat> - other
  } else if (STFind(symtab, token->data)) { // A function or a variable
    LOG("<stat>            -> [id] <statWithId> <stat>\n");
    // <statWithId>
    TryCall(pStatWithId, token->data);

  // -> eps
  } else {
    LOG("-> eps");
    TryCall(stashToken, &token);
    token = NULL;
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
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // -> <fnCall>
  if(STFind(symtab, idName) 
      && !STGetIsVariable(symtab, idName) 
      && STGetFnDefined(symtab, idName)){
    pFnCall(idName);
    vypluj 0;

  // Not a function call but an assignment
  }else{
    // Get new token (to see if it is a ',' of '=')
    TryCall(scanner, &token);
    printToken(token);

    // idName must be a variable
    if(!STFind(symtab, idName) || !STGetIsVariable(symtab, idName)){
      vypluj err(SYNTAX_ERR);
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

      // [id]
      RequireTokenType(t_idOrKeyword);
      if (!STFind(symtab, token->data) 
          || !STGetIsVariable(symtab, token->data)) {
        vypluj err(SYNTAX_ERR);
      }

      // Get their names (in the generated code)
      char *id1Var = STGetName(symtab, idName);
      char *id2Var = STGetName(symtab, token->data);

      // <nextAssign>
      // Check and generate more assignments if there are some
      TryCall(pNextAssign);

      // <expr>
      // Call the shift-reduce parser and assign the result to id2Var
      char *retVarName = NULL;
      TryCall(pExpr, &retVarName);
      genVarAssign(id2Var, -1, retVarName);

      // ,
      RequireTokenType(t_comma);

      // <expr>
      // Call the shift-reduce parser and assign the result to id1Var
      retVarName = NULL;
      TryCall(pExpr, &retVarName);
      genVarAssign(id1Var, -1, retVarName);

    }else{
      vypluj err(SYNTAX_ERR);
    }
  }
  return 0;
}

//TODO not working, not sure if CFG is right
/**
 * @brief
 *
 * @return error code
 *
 * 26. <nextAssign>      -> , [id] <nextAssign> <expr> ,
 * 27. <nextAssign>      -> =
 */
int pNextAssign() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // -> =
  if (token->type == t_assignment) {
    vypluj 0;
  }

  // -> , [id] <nextAssign> <expr> ,
  if(token->type != t_comma) {
    vypluj err(SYNTAX_ERR);
  }

  // [id]
  // The next token must be a variable
  RequireTokenType(t_idOrKeyword);
  if (!STFind(symtab, token->data) || !STGetIsVariable(symtab, token->data)) {
    vypluj err(SYNTAX_ERR);
  }

  // <nextAssign>
  TryCall(pNextAssign);

  // <expr>
  // Evaluate the expression and MOVE from the retVarName to varName
  char *varName = STGetName(symtab, token->data);
  char *retVarName;
  TryCall(pExpr, &retVarName);
  genMove(varName, retVarName, symtab->top->depth);

  // ','
  RequireTokenType(t_comma);

  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 29. <fnArgList>       -> eps
 * 30. <fnArgList>       -> [id] : [type] <nextFnArg>
 */
int pFnArgList(char *fnName) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // -> eps
  if(token->type != t_idOrKeyword || isKeyword(token)) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> [id] : [type] <nextFnArg>

  // [id]
  // Append the name of the new parameter to the symtable
  STAppendParamName(symtab, fnName, token->data);
  int paramCount = 1;

  // TODO toto odkomentovať ak to je správne
  TryCall(STInsert, symtab, token->data);
  STSetName(symtab, token->data, genName(token->data, symtab->top->depth));
  element->data = token->data;
  // :
  RequireTokenType(t_colon);

  // <type>
  // Append the data type of the parameter to the symtab
  // TODO pType under construction
  TryCall(typeFnCall, fnName);
  
  // <nextFnArg>
  vypluj pNextFnArg(fnName, paramCount);
}

/**
 * @brief
 *
 * @return error code
 *
 * 31. <nextFnArg>       -> eps
 * 32. <nextFnArg>       -> , [id] : [type] <nextFnArg>
 */
int pNextFnArg(char *fnName, int paramCount) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // ,
  TryCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> , [id] : [type] <nextFnArg>

  // [id]
  // Must be an ID
  RequireTokenType(t_idOrKeyword);
  if(isKeyword(token)){
    return err(SYNTAX_ERR);
  }
  paramCount++;
  // Append the name of the new parameter to the symtable
  STAppendParamName(symtab, fnName, token->data);

  // TODO uncomment if it is correct
  TryCall(STInsert, symtab, token->data);
  STSetName(symtab, token->data, genName(token->data, symtab->top->depth));
  element->data = token->data;
  genVarDef(STGetName(symtab, token->data));

  // :
  RequireTokenType(t_colon);

  // <type>
  // TODO dependant on pType
  TryCall(typeVar, element->data);

  // <nextFnArg>
  vypluj pNextFnArg(fnName, paramCount);
}

/**
 * @brief
 *
 * @return error code
 *
 * 34. <retArgList>      -> eps
 * 35. <retArgList>      -> <expr> <retNextArg>
 */
int pRetArgList() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();

  // -> eps
  vypluj 0;
  // TODO

  // -> <expr> <retNextArg>
  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName = NULL;
  TryCall(pExpr, &retVarName);

  // <retNextArg>
  vypluj pRetNextArg();
}

// TODO DOKUMENTACE A TAKY PŘESUNOUT NĚKAM NE MEZI PRAVIDLA
bool isExpressionParser(Token token) {
  fprintf(stderr, "is %s expression ?\n", token.data);

  if (token.type == t_leftParen) {
    vypluj true;
  } else if (token.type == t_str) {
    vypluj true;
  } else if (token.type == t_arithmOp) {
    if (strcmp(token.data, "+") == 0 || strcmp(token.data, "-") == 0 || strcmp(token.data, "*") == 0 || strcmp(token.data, "/") == 0 || strcmp(token.data, "//") == 0) {
      vypluj true;
    }
  } else if (token.type == t_int) {
    vypluj true;
  } else if (token.type == t_sciNum) {
    vypluj true;
  } else if (token.type == t_num) {
    vypluj true;
  } else if (token.type == t_idOrKeyword) {

    if (strcmp(token.data, "nil") == 0) {
      vypluj true;
    } else if (STGetIsVariable(symtab, token.data)) {
      vypluj true;
    }

  } else if (token.type == t_relOp) {
    vypluj true;
  }

  vypluj false;
}

/**
 * @brief
 *
 * @return error code
 *
 * 36. <retNextArg>      -> eps
 * 37. <retNextArg>      -> , <expr> <retNextArg>
 */
int pRetNextArg() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // ','
  TryCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> , <expr> <retNextArg>
  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName = NULL;
  TryCall(pExpr, &retVarName);

  // <retNextArg>
  vypluj pRetNextArg();
}

/**
 *
 *
 * 39. <fnParamTypeList> -> eps
 * 40. <fnParamTypeList> -> [type] <nextParamType>
 */
int pFnParamTypeList(char *fnName){ //J
  Token *token = NULL;

  TryCall(scanner, &token);
  
  //eps
  if(isDataType(token->data) == false){
    TryCall(stashToken, &token);
    vypluj 0;
  }

  TryCall(stashToken, &token);
  // [type]
  TryCall(typeFnDeclaration, fnName);

  //<nextParamType>
  vypluj pNextParamType(fnName);
}

/**
 *
 *
 * 41. <nextParamType>   -> eps
 * 42. <nextParamType>   -> , [type] <nextParamType>
 */
int pNextParamType(char *fnName) { //J

  Token *token = NULL;

  TryCall(scanner, &token);

  //eps
  if(token->type != t_comma){
    TryCall(stashToken, &token);
    vypluj 0;
  }
  
  TryCall(typeFnCall, fnName);
  
  vypluj pNextParamType(fnName);
}

/**
 *
 *
 * 44. <fnRetTypeList>   -> eps 
 * 45. <fnRetTypeList>   -> : [type] <nextRetType>
 */
int pFnRetTypeList(char *fnName) {
  Token *token;

  TryCall(scanner, &token);

  if(token->type != t_colon) {
    TryCall(stashToken, &token);
    vypluj 0;
  }
  
  TryCall(typeFnDeclaration, fnName);

  TryCall(nextRetType, fnName);

  vypluj 0;

}

/**
 *
 *
 * 46. <nextRetType>     -> eps
 * 47. <nextRetType>     -> , [type] <nextRetType>
 */
int nextRetType(char *fnName) {
  Token *token;

  TryCall(scanner, &token);

  // eps
  if(token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  TryCall(typeFnDeclaration, fnName);

  TryCall(nextRetType, fnName);

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
 
int pNewIdAssign() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
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
  vypluj pExpr(&retVarName);
}

/**
 * @brief
 *
 * @return error code
 */
int pExpr(char **retVarName) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  char *varName = NULL;
  
  // If it is a function call (and fn is defined), don't call the shift-reduce parser at all
  if(STFind(symtab, token->data) 
      && !STGetIsVariable(symtab, token->data)
      && STGetFnDefined(symtab, token->data)) {

    TryCall(pFnCall, token->data);
    

  } else if(isKeyword(token) && strcmp(token->data, "nil")) {
    TryCall(stashToken, &token);

  } else {
    TryCall(parseExpression, symtab, token, &varName);
    LOG("Result is stored in %s\n", varName);
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
  if (strcmp(data, "string") == 0) {
    vypluj true;
  } else if (strcmp(data, "integer") == 0) {
    vypluj true;
  } else if (strcmp(data, "number") == 0) {
    vypluj true;
  } else if(strcmp(data, "nil")) {
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
  if(strcmp(data, "readi") == 0 
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
  if(strcmp(data, "substr") == 0 
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
  
  TryCall(genVarAssign, STGetName(symtab, element->data), element->dataType, token->data);
  vypluj true;
}

/** TODO do we ever use this?
 * @brief returns value of data type in IFJ21DataTypes
 *
 * @param string
 *
 * @return return value in range <0, 3> if it is in datastructure, else -1
 */
int getDataTypeInt(char *data) {
  if (strcmp(data, "integer") == 0) {
    vypluj dt_integer;
  } else if (strcmp(data, "number") == 0) {
    vypluj dt_number;
  } else if (strcmp(data, "string") == 0) {
    vypluj dt_string;
  } else if(strcmp(data, "nil")) {
    vypluj dt_nil;
  } else {
    vypluj -1;
  }
}

/**
 * @brief Check if token is built in function
 *
 * @return if token is built in function return true, else false
 */
bool isBuiltInFunction(char *data) {
    if (strcmp(data, "reads") == 0) {
      vypluj true;
    } else if (strcmp(data, "readi") == 0) {
      vypluj true;
    } else if (strcmp(data, "readn") == 0) {
      vypluj true;
    } else if (strcmp(data, "write") == 0) {
      vypluj true;
    } else if (strcmp(data, "tointeger") == 0) {
      vypluj true;
    } else if (strcmp(data, "substr") == 0) {
      vypluj true;
    } else if (strcmp(data, "ord") == 0) {
      vypluj true;
    } else if (strcmp(data, "chr") == 0) {
      vypluj true;
    }
  vypluj false;
}

/**
 * @brief Check if token is keyword
 *
 * @return if token is keyword return true, else false
 */
bool isKeyword(Token *token) {

  if (token->type == t_idOrKeyword) {

    if (strcmp(token->data, "do") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "else") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "end") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "function") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "global") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "if") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "integer") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "local") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "nil") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "number") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "require") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "return") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "string") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "then") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "while") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "reads") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "readi") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "readn") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "write") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "tointeger") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "substr") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "ord") == 0) {
      vypluj true;
    } else if (strcmp(token->data, "chr") == 0) {
      vypluj true;
    }
  }
  return false;
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
    LOG("Function already defiend");
    vypluj err(ID_DEF_ERR);
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
    vypluj err(1); // TODO errcode (declaration when fn is already declared/defined)
  } else {
    LOG("Adding to the symtable (declaration)");
    TryCall(STInsert, symtab, fnName);
    STSetIsVariable(symtab, fnName, false);
    STSetFnDefined(symtab, fnName, false);
    STSetName(symtab, fnName, genLabelName());
  }
  vypluj 0;
}

/**
 * NOT USED TODO USE IS
 *
 * @brief Function for built in functions
 *
 * @return error code
 *  TODO ADD MORE CFG RULES FOR ALL FUNCTIONS
 * TODO ADD IT TO CFG !!!!!!!!!!!!!!!!
 */
int builtInFunctions(char *fnName) {
 //* 41. <retNextArg>      -> eps
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;


  if (strcmp(fnName, "reads") == 0) {
    
    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
    vypluj 0;
  } else if (strcmp(fnName, "readi") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(fnName, "readn") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(fnName, "write") == 0) {
    //TODO
    RequireTokenType(t_leftParen);

    while (token->type != t_rightParen) {
      TryCall(scanner, &token);
      
      if(token->type != t_rightParen) {
        genWrite(token, symtab->top->depth);
      }
    }
  } else if (strcmp(fnName, "substr") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(fnName, "ord") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(fnName, "chr") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  }
  vypluj 0;
}

int typeFnDeclaration(char *fnName){
  fprintf(stderr, "TYPE DEF FN-----------------------------------------------\n");
  fprintf(stderr, "varname : %s \n", fnName);

  Token *token;
  STElem *fnElement = STFind(symtab, fnName);

  if(fnElement == NULL) {
    vypluj err(1); // TODO add err code
  }

  TryCall(scanner, &token);

  if(isDataType(token->data) == false) {
    vypluj err(1); // TODO add err code
  }

  TryCall(intBufAppend, fnElement->fnRetTypesBuf, getDataTypeFromString(token->data));

  return 0;
}

int typeFnCall(char *fnName){
  
  // find STElem by fnName
    STElem *tmp = STFind(symtab, fnName);
    Token *token;
    // fn is not defined
    if(tmp == NULL || tmp->fnDefined == false) {
      vypluj err(1); // TODO ADD ERR CODE
    }

    TryCall(scanner, &token);
    printToken(token);

    if(isDataType(token->data) == false) {
      vypluj err(1); // TODO ADD ERR CODE
    }

    TryCall(STAppendParamType, symtab, fnName, getDataTypeFromString(token->data));
    
  return 0;
}

int typeVar(char *varName){
  fprintf(stderr, "TYPEVAR-----------------------------------------------\n");
  fprintf(stderr, "varname : %s \n", varName);

  Token *token;
  STElem *varElement = STFind(symtab, varName);


  // if variable is not variable or doesnt exist
  if(varElement == NULL) {
    vypluj err(1); // TODO add err code
  }

  TryCall(scanner, &token);
  
  if(isDataType(token->data) == false) {
    vypluj err(1); // TODO change err code
  }

  varElement->varDataType = getDataTypeFromString(token->data);

  return 0;
}

int stringFunctions(char *varName) {
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  if(strcmp(token->data, "substr") == 0) {

    RequireTokenType(t_leftParen);

    TryCall(scanner, &token);
    Token *string = NULL;
    double i;
    double j;
    char *endPtr;

    if(token->type != t_str || token->type != t_idOrKeyword) {
      vypluj err(1); // TODO CHANGE ERR CODE
    }

    string = token;

    TryCall(scanner, &token);

    if(token->type != t_int || token->type != t_num) {
      vypluj err(1); // TODO CHANGE ERR CODE
    }

    i = strtod(token->data, &endPtr);


    TryCall(scanner, &token);

    if(token->type != t_int || token->type != t_num) {
      vypluj err(1); // TODO CHANGE ERR CODE
    }

    j = strtod(token->data, &endPtr);


    RequireTokenType(t_rightParen);

    genSubstrFunction(varName, string, i, j, symtab->top->depth);

    vypluj 0;

  }

  vypluj err(1);

}

/*
 * FUNCTIONS TO REMOVE since CFG changed (but code might be useful)

int pFnRet(char *fnName) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 10. <fnRet>           -> eps
  // :
  if (token->type != t_colon) {
    LOG("Function with no return values\n");
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 11. <fnRet>           -> : <type> <nextType>

  int typeCount = 1;
  (void)typeCount; // TODO remove
  // <type>
  // TODO dependant on pType
  TryCall(pType, fnName, NULL, false);

  // <nextType>
  // TODO pass the typeCount, too
  TryCall(pNextType, fnName);

  return 0;
}

int pTypeList(char *fnName) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // integer/number/string/nil
  if(isDataType(token->data)) {
    int dataType = getDataTypeFromString(token->data);
    TryCall(STAppendParamType, symtab, fnName, dataType);

    // <nextType>
    TryCall(pNextType, fnName);

  // eps
  }else{
    TryCall(stashToken, &token);
  }

  vypluj 0;
}

int pNextType(char *fnName) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 51. <nextType>        -> , <type> <nextType>
  // ,
  if(token->type == t_comma){

    // <type>
    // TODO we're not calling pType here, btw. According to CFG, we should
    RequireTokenType(t_idOrKeyword);
    int dataType = getDataTypeFromString(token->data);
    TryCall(STAppendParamType, symtab, token->data, dataType);

    // <nextType>
    TryCall(pNextType, fnName);

  // 50. <nextType>        -> eps
  }else{
    TryCall(stashToken, &token);
  }

  vypluj 0;
}

int pType(char *fnName, char *varName, bool isFnCalled) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  fprintf(stderr, "VAR NAME = %s\n", varName);
  LOG();
  //Token *token = NULL;
  //TryCall(scanner, &token); // TOHLE JEN ABY SE DALO DÁL DEBUGOVAT

  // TODO this is sus pType() in calling fn ??????????? - alex
  // when fn is called to be sure all input 
  if(isFnCalled) {

    if(fnName == NULL || varName == NULL) {
      vypluj err(1); // TODO ADD ERR CODE
    }

    TryCall(typeFnCall, fnName, varName);

  } else if(varName != NULL) {

    TryCall(typeVar, varName);

    vypluj 0;
  } else if(fnName != NULL) {
    
    TryCall(typeFnDeclaration, fnName);

  } else {
    fprintf(stderr, "pType intern ERROR\n");
    vypluj err(1); // TODO ADD ERR CODE
  }

  vypluj 0;
}

int pIdList() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // [id]
  TryCall(scanner, &token);
  printToken(token);

  if (token->type != t_idOrKeyword || isKeyword(token)) {
    vypluj err(SYNTAX_ERR);
  }

  // TODO toto je nedobre:
  if (STFind(symtab, token->data)) {
    if(STGetIsVariable(symtab, token->data)) { // redeclaring a variable, the user deserves a slap
      fprintf(stderr, "warning: declaring a variable again\n");
      element->data = token->data;
      genVarDef(STGetName(symtab, token->data)); // already declared, should be just an assignment, is this correct?? TODO
    } else { // can't function call here
      vypluj err(SYNTAX_ERR);
    }

  }

  // Declare a new variable
  TryCall(STInsert, symtab, token->data);
  STSetIsVariable(symtab, token->data, true);
  STSetFnDefined(symtab, token->data, false);
  strcpy(element->data, token->data);

  // <nextId>
  TryCall(pNextId);

  return 0;
}

int pNextId() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // ,
  TryCall(scanner, &token);
  printToken(token);

  // 59. <nextId>          -> eps
  if (token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 60. <nextId>          -> , [id] <nextId>
  TryCall(scanner, &token);
  printToken(token);

  // [id]
  if (STFind(symtab, token->data)) {
    if(STGetIsVariable(symtab, token->data)) { // redeclaring a variable, the user deserves a slap
      fprintf(stderr, "warning: declaring a variable again\n");
      genVarDef(STGetName(symtab, token->data)); // already declared, shoud be just an assignment, is this correct?? TODO
    } else { // -> eps - TODO WHAT??
      TryCall(stashToken, &token);
      vypluj 0;
    }
  }

  // declaring a new variable
  TryCall(STInsert, symtab, token->data);
  STSetIsVariable(symtab, token->data, true);
  STSetFnDefined(symtab, token->data, false);
  STSetName(symtab, token->data, genName(token->data, symtab->top->depth));

  // <nextId>
  vypluj pNextId();
}
*/

#endif
/* end of file parser.c */
