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
 * pStart()             FULLY DONE
 * pReq()               FULLY DONE
 * pCodeBody()          DONE
 * pFnCall()            DONE
 * pFnRet()             DONE
 * pFnCallArgList()     DONE
 * pNextFnCallArg()     DONE
 * pFnCallArg()         RETRACTOR
 * pRet()               DONE
 * pStat()              ADD GENERATING CODE
 * pStatWithId()        Waiting for pExpr()
 * pNextAssign()        Change error codes
 * pFnArgList()         DONE
 * pNextFnArg()         DONE --
 * pRetArgList()        DONE
 * pRetNextArg()        DONE
 * pTypeList()          DONE
 * pNextType()          DONE
 * pType()              DONE
 * pIdList()            DONE
 * pNextId()            DONE
 * pNewIdAssign()       DONE
 * pExprList()          DONE
 * pNextExpr()          DONE
 * pExpr()              NOT IMPLEMENTED YET -- CALL BOTTOM UP PARSER (NOT DONE YET TOO)
 * -----------------------------------------------------------------------
 */

#ifndef PARSER_C
#define PARSER_C

#include "parser.h"

// If set to 0, no debug prints will be written
#define DEBUGTOGGLE 1

extern int ret;

STStack *symtab;

//----------------------------------
// SStackElem stuff, just ignore TODO move somewhere?

SStackElem *element;


// TODO
#define RequireKeyword(str1, str2)                                             \
  if(!strEq(str1, str2)) {                                                     \
    vypluj err(SYNTAX_ERR);                                                    \
  }                                                                            \

// TODO
#define ForbidKeyword(str1, str2)                                              \
  if(strEq(str1, str2)) {                                                      \
    vypluj err(SYNTAX_ERR);                                                    \
  }                                                                            \

// Get a new token and if the type doesn't match, throw a syntax err
#define RequireTokenType(tokenType)                                            \
  TryCall(scanner, &token);                                                    \
  if(!token){                                                                  \
    return 0;                                                                  \
  }                                                                            \
  if(token->type != tokenType) {                                               \
    vypluj err(SYNTAX_ERR);                                                    \
  }                                                                            \
  printToken(token);

// Get a new token and if the type or data don't match, throw a syntax err
#define RequireToken(tokenType, tokenData)                                     \
  TryCall(scanner, &token);                                                    \
  if(!token){                                                                  \
    return err(SYNTAX_ERR);                                                    \
  }                                                                            \
  if(token->type != tokenType                                                  \
      || strcmp(token->data, #tokenData) != 0) {                               \
    vypluj err(SYNTAX_ERR);                                                    \
  }                                                                            \
  printToken(token);

// Prints filename, line number, fn name and given arguments to stderr
#define LOG(fmt, ...)                                                          \
  do {                                                                         \
    if (DEBUGTOGGLE) {                                                         \
      fprintf(stderr, "LOG: %s:%d:%s(): " fmt,                                 \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);                        \
      fprintf(stderr, "\n");                                                   \
    }                                                                          \
  } while (0)

void cleanElement() {
  element->next->data = NULL;
  element->data = NULL;
}

void initElement() {
  GCMalloc(element, sizeof(SStackElem));
  GCMalloc(element->data, sizeof(char) * 100);
}

//------------------------------------------------------------

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
  
  TryCall(genVarAssign, element->data, element->dataType, token->data);
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
bool isBuiltInFunction(Token *token) {
  if (token->type == t_idOrKeyword) {
    if (strcmp(token->data, "reads") == 0) {
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

/*
 * Rules
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
  }
  vypluj 0;
}

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
int pCodeBody() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();

  Token *token = NULL;
  TryCall(scanner, &token);

  // If it is not eps, it has to be an ID or keyword
  if(token && token->type != t_idOrKeyword){
    vypluj err(SYNTAX_ERR);
  }

  // 04. <codeBody>        -> eps
  if(token == NULL) {
    return 0;
  }

  // 05. <codeBody>        -> function [id] ( <fnArgList> ) <fnRet> <stat> <ret> end <codeBody>
  if(strcmp(token->data, "function") == 0) {
    // [id]
    RequireTokenType(t_idOrKeyword);

    // Define the new function (in the symtable)
    TryCall(newFunctionDefinition, token->data);

    // Generate code
    genFnDef(token->data);

    // (
    RequireTokenType(t_leftParen);

    // <fnArgList>
    TryCall(pFnArgList);

    // )
    RequireTokenType(t_rightParen);

    // <fnRet>
    TryCall(pFnRet);

    // <stat>
    TryCall(pStat);

    // <ret>
    TryCall(pRet);

    // end
    RequireToken(t_idOrKeyword, "end");

    // <codeBody>
    TryCall(pCodeBody);
  }

  // 06. <codeBody>        -> global [id] : function ( <typeList> ) <fnRet> <codeBody>
  else if(strcmp(token->data, "global") == 0) {
    // [id]
    RequireTokenType(t_idOrKeyword);

    // Declare the new function (in the symtable)
    TryCall(newFunctionDeclaration, token->data);

    // :
    RequireTokenType(t_colon);

    // function
    RequireToken(t_idOrKeyword, "function");

    // (
    RequireTokenType(t_leftParen);

    // <typeList>
    TryCall(pTypeList);

    // )
    RequireTokenType(t_rightParen);

    // <fnRet>
    TryCall(pFnRet);

    // <codeBody>
    TryCall(pCodeBody);
  }

  // 07. <codeBody>        -> [id] <fnCall> <codeBody>
  else if(token->type == t_idOrKeyword) {

    // The function name must be in the symtab, must be a function and defined
    if(!STFind(symtab, token->data) || !STGetFnDefined(symtab, token->data)){
      LOG("Calling an undefined function");
      vypluj err(SYNTAX_ERR);
    }

    // <fnCall>
    TryCall(pFnCall, token->data);

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

  // TODO create a TF?

  // (
  RequireTokenType(t_leftParen);

  // <fnCallArgList>
  TryCall(pFnCallArgList);

  // )
  RequireTokenType(t_rightParen);

  // TODO generate code to call the function

  fprintf(stderr, "FN CALL RET\n");
  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 10. <fnRet>           -> eps
 * 11. <fnRet>           -> : <type> <nextType>
 */
int pFnRet() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 10. <fnRet>           -> eps
  // :
  if (token->type != t_colon) {
    fprintf(stderr, "STASH TOKEN FNRET\n");
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 11. <fnRet>           -> : <type> <nextType>
  // <type>
  TryCall(pType);

  // <nextType>
  vypluj pNextType();
}


/**
 * @brief
 *
 * @return error code
 *
 * 12. <fnCallArgList>   -> eps
 * 13. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
 */
int pFnCallArgList() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 12. <fnCallArgList>   -> eps
  if(token->type == t_rightParen) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 13. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
  TryCall(stashToken, &token);

  //<fnCallArg>
  TryCall(pFnCallArg);

  // <nextFnCallArg>
  vypluj pNextFnCallArg();
}

/**
 * @brief
 *
 * @return error code
 *
 * 14. <nextFnCallArg>   -> eps
 * 15. <nextFnCallArg>   -> , <fnCallArg> <nextFnCallArg>
 */
int pNextFnCallArg() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);
  
  // 14. <nextFnCallArg>   -> eps
  // ,
  if(token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 15. <nextFnCallArg>   -> , <fnCallArg> <nextFnCallArg>
  // <fnCallArg>
  TryCall(pFnCallArg);
  
  // <nextFnCallArg>
  vypluj pNextFnCallArg();
}

/**
 * @brief
 *
 * @return error code
 *
 * 16. <fnCallArg>       -> [id]
 * 17. <fnCallArg>       -> [literal]
 */
int pFnCallArg() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  int tokenType = token->type;

  // -> [id] (must be a variable)
  if (STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)) {
    fprintf(stderr, "JE TO PREMENNÁ\n");
    TryCall(STPush, symtab);
    TryCall(STInsert, symtab, token->data);

    // TODO semantic actions (add to fn arg types or something)
    vypluj 0;

    // -> [literal]
  } else if (tokenType == t_int || tokenType == t_num ||
      tokenType == t_sciNum || tokenType == t_str) {
    fprintf(stderr, "JE TO LITERÁL\n");
    // TODO semantic actions
    vypluj 0;

  } else {
    fprintf(stderr, "NENI TO ANI PREMENNÁ A ANI LITERÁL\n");
    if(tokenType == t_idOrKeyword) {
      vypluj err(ID_DEF_ERR);
    }
    vypluj err(SYNTAX_ERR);

  }
  vypluj 0; // what?
}

/**
 * @brief
 *
 * @return error code
 *
 * 19. <ret>             -> eps
 * 20. <ret>             -> return <retArgList>
 */
int pRet() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // -> eps
  if(strcmp(token->data, "return") != 0) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // <retArgList>
  TryCall(pRetArgList);
    
  vypluj 0;
}

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
int pStat() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_idOrKeyword) {
    fprintf(stderr, " <stat>            -> eps\n");
    TryCall(stashToken, &token);
    token = NULL;
    vypluj 0;
  }

  // -> local <idList> : <type> <newIdAssign> <stat>
  if (strcmp(token->data, "local") == 0) {
    fprintf(stderr, "<stat>            -> local <idList> : <type> <newIdAssign> <stat>\n");
    // <idList>
    TryCall(pIdList);

    // :
    RequireTokenType(t_colon);

    // <type>
    TryCall(pType);
    
    // <newIdAssign>
    TryCall(pNewIdAssign);
    
    // <stat>
    TryCall(pStat);

    vypluj 0;
  }

  // -> if <expr> then <stat> else <stat> end <stat>
  else if (strcmp(token->data, "if") == 0) {
    fprintf(stderr, "<stat>            -> if <expr> then <stat> else <stat> end <stat>\n");
    // <expr>
    // TODO semantic actions - use that retVarName - the result of the expr is there
    char *retVarName = NULL;
    TryCall(pExpr, &retVarName);

    // then
    TryCall(scanner, &token);
    printToken(token);
    fprintf(stderr, "TU\n");

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "then") == 0)) {
      vypluj err(SYNTAX_ERR);
    }

    TryCall(STPush, symtab);

    // <stat>
    TryCall(pStat);
    STPop(symtab);

    // else
    TryCall(scanner, &token);
    printToken(token);

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "else") == 0)) {
      vypluj err(SYNTAX_ERR);
    }
    TryCall(STPush, symtab);

    // <stat>
    TryCall(pStat);
    fprintf(stderr, "WE ARE BACK IN STAT\n");

    // end
    TryCall(scanner, &token);
    if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
      vypluj err(SYNTAX_ERR); // todo call the fancy new function
    }
    STPop(symtab);

    // <stat>
    vypluj pStat();
  }

  // -> while <expr> do <stat> end <stat>
  else if (strcmp(token->data, "while") == 0) {
    fprintf(stderr, "<stat>            -> while <expr> do <stat> end <stat>\n");
    // <expr>
    // TODO semantic actions - use that retVarName - the result of the expr is there
    char *retVarName = NULL;
    TryCall(pExpr, &retVarName);

    // do
    TryCall(scanner, &token);
    printToken(token);

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "do") == 0)) {
      vypluj err(SYNTAX_ERR); // todo function for keywords
    }

    // <stat>
    TryCall(pStat);

    // end
    TryCall(scanner, &token);
    if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
      vypluj err(SYNTAX_ERR); // todo call keyword function
    }
    TryCall(stashToken, &token); // WHY??
    vypluj 0;

  } else if(strcmp(token->data, "return") == 0) {
    TryCall(pExpr, NULL); // TODO NAHRADIŤ NULL
    vypluj 0;
  } else if(strcmp(token->data, "end") == 0) {

    printToken(token);

    TryCall(stashToken, &token);
    vypluj 0;
  }

  // -> [id] <statWithId> <stat> - built in write function call
   else if (strcmp(token->data, "write") == 0) {
    TryCall(pFnCall, "write");
    TryCall(pStat);
    vypluj 0;
    // TODO other built in functions call?
  }

  // -> [id] <statWithId> <stat> - other
  else if (STFind(symtab, token->data)) { // could be any id
    fprintf(stderr, "<stat>            -> [id] <statWithId> <stat>\n");
    // TODO generate something

    // <statWithId>
    TryCall(pStatWithId, token->data);

    // <stat>
    vypluj pStat();
  }
  // -> eps
  else {
    TryCall(stashToken, &token);
    token = NULL;
    vypluj 0;
  }
}


/**
 * @brief
 *
 * @return error code
 *
 * 27. <statWithId>      -> , [id] <nextAssign> <expr> , <expr>
 * 28. <statWithId>      -> = <expr>
 * 29. <statWithId>      -> <fnCall>
 */
int pStatWithId(char *idName) {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // -> <fnCall>
  if(STFind(symtab, idName) 
      && !STGetIsVariable(symtab, idName) 
      && STGetFnDefined(symtab, idName)){
    // TODO built in functions! Some code is currently in pExpr
    pFnCall(idName);
    // TODO generate code to jump to the function label
    vypluj 0;


  // Not a function call but an assignment
  }else{
    // Get new token (to see if it is a ',' of '=')
    TryCall(scanner, &token);
    printToken(token);

    // -> , [id] <nextAssign> <expr> , <expr>
    // In idName we have a name of the first variable in this statement
    // In token->data we have a name of the second one
    if (token->type == t_comma) {

      // [id]
      RequireTokenType(t_idOrKeyword);
      if (!STFind(symtab, token->data) || !STGetIsVariable(symtab, token->data)) {
        vypluj err(SYNTAX_ERR);
      }

      // TODO define variables for idName and token->data
      genVarDef(token->data, symtab->top->depth);

      // <nextAssign>
      // Check and generate more assignments if there are some
      TryCall(pNextAssign);

      // <expr>
      char *retVarName = NULL;
      TryCall(pExpr, &retVarName);

      genVarAssign(token->data, -1 ,retVarName);

      // ,
      RequireTokenType(t_comma);

      retVarName = NULL;
      TryCall(pExpr, &retVarName);
      // TODO use that retvarname (MOVE it to idName we got as a param)

      vypluj 0;

    // -> = <expr>
    }else if (strcmp(token->data, "=") == 0) {
    
      char *retVarName = NULL;
      TryCall(pExpr, &retVarName);
      // TODO code gen to MOVE the retVarName to idName
      return 0;

    }else{
      vypluj err(SYNTAX_ERR);
    }
  }
}

//TODO not working, not sure if CFG is right
/**
 * @brief
 *
 * @return error code
 *
 * 31. <nextAssign>  -> , [id] <nextAssign> <expr> ,
 * 32. <nextAssign>  -> =
 */
int pNextAssign() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 32. <nextAssign>  -> =
  if (token->type == t_assignment) {
    vypluj 0;
  }

  // 31. <nextAssign>  -> , [id] <nextAssign> <expr> ,
  if(token->type != t_comma) {
    vypluj err(SYNTAX_ERR);
  }

  // [id]
  // TODO semantic actions???
  TryCall(scanner, &token);
  printToken(token);

  if (!(token->type == t_idOrKeyword && STFind(symtab, token->data) && STGetIsVariable(symtab, token->data))) {
    vypluj err(SYNTAX_ERR);
  }

  genVarDef(token->data, symtab->top->depth);

  // <nextAssign>
  TryCall(pNextAssign);

  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName;
  TryCall(pExpr, &retVarName);

  // ','
  RequireTokenType(t_comma);

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
int pBuiltInFunctions() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  if (strcmp(token->data, "reads") == 0) {

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
    vypluj 0;
  } else if (strcmp(token->data, "readi") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(token->data, "readn") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(token->data, "write") == 0) {
    //TODO
    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(token->data, "substr") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(token->data, "ord") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  } else if (strcmp(token->data, "chr") == 0) {
    //TODO

    while (token->type != t_rightParen) {
      ret = scanner(&token);
      CondReturn;
    }
  }
  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 34. <fnArgList>       -> eps
 * 35. <fnArgList>       -> [id] : <type> <nextFnArg>
 */
int pFnArgList() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 34. <fnArgList>       -> eps
  if(!(token->type == t_idOrKeyword && !isKeyword(token))) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 35. <fnArgList>       -> [id] : <type> <nextFnArg>
  // [id]
  genVarDef(token->data, symtab->top->depth);
  TryCall(STInsert, symtab, token->data);

  // :
  RequireTokenType(t_colon);

  // <type>
  TryCall(pType);

  // <nextFnArg>
  vypluj pNextFnArg();
}

/**
 * @brief
 *
 * @return error code
 *
 * 36. <nextFnArg>       -> eps
 * 37. <nextFnArg>       -> , [id] : <type> <nextFnArg>
 */
int pNextFnArg() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // ,
  TryCall(scanner, &token);
  printToken(token);

  // 36. <nextFnArg>       -> eps
  if (token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 37. <nextFnArg>       -> , [id] : <type> <nextFnArg>
  // [id]
  TryCall(scanner, &token);
  printToken(token);

  TryCall(STInsert, symtab, token->data);
  element->data = token->data;
  genVarDef(token->data, symtab->top->depth);
  // :
  RequireTokenType(t_colon);

  // <type>
  TryCall(pType, &token);
  printToken(token);

  // <nextFnArg>
  vypluj pNextFnArg(&token);
}

/**
 * @brief
 *
 * @return error code
 *
 * 39. <retArgList>      -> eps
 * 40. <retArgList>      -> <expr> <retNextArg>
 */
int pRetArgList() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();

  // 39. <retArgList>      -> eps
  // TODO

  // 40. <retArgList>      -> <expr> <retNextArg>
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
 * 41. <retNextArg>      -> eps
 * 42. <retNextArg>      -> , <expr> <retNextArg>
 */
int pRetNextArg() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // ','
  TryCall(scanner, &token);
  printToken(token);

  // 41. <retNextArg>      -> eps
  if (token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 42. <retNextArg>      -> , <expr> <retNextArg>
  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName = NULL;
  TryCall(pExpr, &retVarName);

  // <retNextArg>
  vypluj pRetNextArg();
}

/** // TODO this makes me want to vomit as well as most functions below (retraktor comming soon)
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
int pTypeList() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // eps or type
  if(!isDataType(token->data)) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // <nextType>
  vypluj pNextType();
}

/**
 * @brief
 *
 * @return error code
 *
 * 50. <nextType>        -> eps
 * 51. <nextType>        -> , <type> <nextType>
 */
int pNextType() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  // ,
  TryCall(scanner, &token);
  printToken(token);

  // 50. <nextType>        -> eps
  if (token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 51. <nextType>        -> , <type> <nextType>
  // <type>
  TryCall(pType);

  // <nextType>
  vypluj pNextType();
}

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
int pType() {
  fprintf(stderr,"-----------------------------------------------------------\n");
  fprintf(stderr,"TYPE\n");
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  if(!isDataType(token->data)) {
    vypluj err(SYNTAX_ERR);
  }

  // Adding data type to existing variable in ST
  // else pType() is called as data type of function, do nothing
  if (element->data != NULL) {
    STSetVarDataType(symtab, element->data, getDataTypeInt(token->data));
    // TODO call gen function for fn param definition -> fn not done yet
    genVarDef(element->data, symtab->top->depth);

  }
  vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 58. <idList>          -> [id] <nextId> - variable declaration
 */
int pIdList() {
  fprintf(stderr,"-----------------------------------------------------------\n");
  fprintf(stderr," ID LIST\n");
  Token *token = NULL;

  // [id]
  TryCall(scanner, &token);
  printToken(token);

  if (!(token->type == t_idOrKeyword && !isKeyword(token))) {
    vypluj err(SYNTAX_ERR);
  }

  if (STFind(symtab, token->data)) {
    if(STGetIsVariable(symtab, token->data)) { // redeclaring a variable, the user deserves a slap
      fprintf(stderr, "warning: declaring a variable again\n");
      element->data = token->data;
      genVarDef(token->data, symtab->top->depth); // already declared, should be just an assignment, is this correct?? TODO
    } else { // can't function call here
      vypluj err(SYNTAX_ERR);
    }
  }
  // declaring a new variable
  TryCall(STInsert, symtab, token->data);
  STSetIsVariable(symtab, token->data, true);
  STSetFnDefined(symtab, token->data, false);
  printf("sdsd\n");
  strcpy(element->data,token->data);

  // <nextId>
  vypluj pNextId();
}

/**
 * @brief
 *
 * @return error code
 *
 * 59. <nextId>          -> eps
 * 60. <nextId>          -> , [id] <nextId>
 */
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
      genVarDef(token->data, symtab->top->depth); // already declared, shoud be just an assignment, is this correct?? TODO
    } else { // -> eps - TODO WHAT??
      TryCall(stashToken, &token);
      vypluj 0;
    }
  }

  // declaring a new variable
  TryCall(STInsert, symtab, token->data);
  STSetIsVariable(symtab, token->data, true);
  STSetFnDefined(symtab, token->data, false);

  // <nextId>
  vypluj pNextId();
}

/**
 * @brief
 *
 * @return error code
 *
 * 62. <newIdAssign>     -> eps
 * 63. <newIdAssign>     -> = <exprList>
 */
int pNewIdAssign() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 62. <newIdAssign>     -> eps
  // =
  if (!(token->type == t_assignment && !strcmp(token->data, "="))) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 63. <newIdAssign>     -> = <exprList>
  // <exprList>
  vypluj pExprList();
}

/**
 * @brief
 *
 * @return error code
 *
 * 65. <exprList>        -> <expr> <nextExpr>
 */
int pExprList() {
  fprintf(stderr, "EXPR LIST \n");
  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName = NULL;
  TryCall(pExpr, &retVarName);

  // <nextExpr>
  vypluj pNextExpr();
}

/**
 * @brief parsing <nextExpr> (rule 66, 67)
 *
 * @return error code
 *
 * 66. <nextExpr>        -> eps
 * 67. <nextExpr>        -> , <expr> <nextExpr>
 */
int pNextExpr() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  LOG();
  Token *token = NULL;

  TryCall(scanner, &token);
  printToken(token);

  // 66. <nextExpr>        -> eps
  // ,
  if (token->type != t_comma) {
    TryCall(stashToken, &token);
    vypluj 0;
  }

  // 67. <nextExpr>        -> , <expr> <nextExpr>
  // <expr>
  // TODO semantic actions - use that retVarName - the result of the expr is there
  char *retVarName = NULL;
  TryCall(pExpr, &retVarName);

  // <nextExpr>
  vypluj pNextExpr();
}

int pStringFunctions(char *varName) {
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

//-----------------------------------
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
      // TODO read funkcie spraviť nejak normálne nie ako imbecil :peepoGiggle: - written by Tedro
    
    if(isReadFunction(token->data)) {
      // asi bude treba overiť či táto gen funkcie nenarazili na err
      genReadFunction(element->data, token->data, symtab->top->depth);

      RequireTokenType(t_leftParen);
      RequireTokenType(t_rightParen);
      vypluj 0;
    } else if(isStringOperationFunction(token->data)) {

      if(strcmp(token->data, "substr") == 0) {
      
        TryCall(scanner, &token);

        if(token->type != t_leftParen) {
          vypluj err(SYNTAX_ERR);
        }
        //char *string
      
        TryCall(scanner, &token);

        if(token->type != t_rightParen) {
          vypluj err(-1); // TODO ADD ERR CODE
        }
      } else if(strcmp(token->data, "ord") == 0) {
          TryCall(scanner, &token);

          if(token->type != t_leftParen) {
          vypluj err(-1); // TODO ADD ERR CODE
          }
          TryCall(scanner, &token);

          if(token->type != t_rightParen) {
            vypluj err(-1); // TODO ADD ERR CODE
          }
      } else if(strcmp(token->data, "chr") == 0) {
          TryCall(scanner, &token);

          if(token->type != t_leftParen) {
            vypluj err(-1); // TODO ADD ERR CODE
          }

          TryCall(scanner, &token);

          if(token->type != t_rightParen) {
            vypluj err(-1); // TODO ADD ERR CODE
          }
      } else {
          vypluj err(-1); // TODO INTERNAL ERROR
      }
    }
    vypluj 0;

  } else if(isKeyword(token)) {
    TryCall(stashToken, &token);

  } else {
    TryCall(parseExpression, symtab, token, &varName);
    fprintf(stderr, "Result is stored in %s\n", varName);
  }
  vypluj 0;
}

#endif
/* end of file parser.c */
