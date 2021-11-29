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
 * pStart()             DONE
 * pReq()               DONEN
 * pCodeBody()          DONE
 * pFnCall()            DONE
 * pFnRet()             DONE
 * pFnCallArgList()     DONE
 * pNextFnCallArg()     DONE
 * pFnCallArg()         RETRACTOR
 * pRet()               DONE
 * pStat()              Waiting for pExpr()
 * pStatWithId()        Waiting for pExpr()
 * pNextAssign()        Change error codes
 * pFnArgList()         Change error codes, check last 2 todos
 * pNextFnArg()         Change error codes
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

extern int ret;

STStack *symtab;

//----------------------------------
// SStackElem stuff, just ignore

SStackElem *element;



#define RequireKeyword(str1, str2)     \
  if(!strEq(str1, str2)) {             \
    vypluj err(SYNTAX_ERR);            \
  }                                    \

#define ForbidKeyword(str1, str2)      \
  if(strEq(str1, str2)) {              \
    vypluj err(SYNTAX_ERR);            \
  }                                    \

#define RequireToken(tokenType)        \
  CondCall(scanner, &token);           \
if(!token){                          \
  return 0;                          \
}                                    \
if(token->type != tokenType) {       \
  vypluj err(SYNTAX_ERR);            \
}                                    \
printToken(token);


void initElement() {
  element = malloc(sizeof(SStackElem));
  element->data = malloc(sizeof(char) * 250);

  SStackElem *tmp = malloc(sizeof(SStackElem));
  tmp->data = malloc(sizeof(char) * 250);

  element->next = tmp;

}

void cleanElement() {
  element->next->data = NULL;
  element->data = NULL;
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
  if(strcmp(data, "readi") == 0 ||
  strcmp(data, "reads") == 0 ||
  strcmp(data, "readn") == 0) {
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
    CondCall(stashToken, &token);
    vypluj false;
  }

  // if token is not a read function, returns false and stashToken
  if(!isReadFunction(token->data)) {
    CondCall(stashToken, &token);
    
    vypluj false;
  }
  
  CondCall(genVarAssign, element->data, element->dataType, token->data);
  vypluj true;
}


/**
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
      return true;
    } else if (strcmp(token->data, "else") == 0) {
      return true;
    } else if (strcmp(token->data, "end") == 0) {
      return true;
    } else if (strcmp(token->data, "function") == 0) {
      return true;
    } else if (strcmp(token->data, "global") == 0) {
      return true;
    } else if (strcmp(token->data, "if") == 0) {
      return true;
    } else if (strcmp(token->data, "integer") == 0) {
      return true;
    } else if (strcmp(token->data, "local") == 0) {
      return true;
    } else if (strcmp(token->data, "nil") == 0) {
      return true;
    } else if (strcmp(token->data, "number") == 0) {
      return true;
    } else if (strcmp(token->data, "require") == 0) {
      return true;
    } else if (strcmp(token->data, "return") == 0) {
      return true;
    } else if (strcmp(token->data, "string") == 0) {
      return true;
    } else if (strcmp(token->data, "then") == 0) {
      return true;
    } else if (strcmp(token->data, "while") == 0) {
      return true;
    } else if (strcmp(token->data, "reads") == 0) {
      return true;
    } else if (strcmp(token->data, "readi") == 0) {
      return true;
    } else if (strcmp(token->data, "readn") == 0) {
      return true;
    } else if (strcmp(token->data, "write") == 0) {
      return true;
    } else if (strcmp(token->data, "tointeger") == 0) {
      return true;
    } else if (strcmp(token->data, "substr") == 0) {
      return true;
    } else if (strcmp(token->data, "ord") == 0) {
      return true;
    } else if (strcmp(token->data, "chr") == 0) {
      return true;
    }
  }
  return false;
}

/**
 * @brief This function is for debug print of token
 *
 * @param token
 */
void printToken(Token *token) {

  switch (token->type) {
    case t_idOrKeyword:
      fprintf(stderr, "Token type : idOrKeyword, Token data : <%s>\n", token->data);
      break;
    case t_colon:
      fprintf(stderr, "Token type : colon, Token data : <%s>\n", token->data);
      break;
    case t_rightParen:
      fprintf(stderr, "Token type : rightParen, Token data : <%s>\n", token->data);
      break;
    case t_comma:
      fprintf(stderr, "Token type : comma, Token data : <%s>\n", token->data);
      break;
    case t_arithmOp:
      fprintf(stderr, "Token type : arithmeticOperation, Token data : <%s>\n", token->data);
      break;
    case t_assignment:
      fprintf(stderr, "Token type : assigment, Token data : <%s> \n", token->data);
      break;
    case t_int:
      fprintf(stderr, "Token type : integer, Token data : <%s> \n", token->data);
      break;
    case t_leftParen:
      fprintf(stderr, "Token type : leftParen, Token data : <%s> \n", token->data);
      break;
    case t_num:
      fprintf(stderr, "Token type : num, Token data : <%s> \n", token->data);
      break;
    case t_relOp:
      fprintf(stderr, "Token type : relationOperation, Token data : <%s> \n", token->data);
      break;
    case t_strOp:
      fprintf(stderr, "Token type : stringOperation, Token data : <%s> \n", token->data);
      break;
    case t_sciNum:
      fprintf(stderr, "Token type : scientificNumber, Token data : <%s> \n", token->data);
      break;
    case t_str:
      fprintf(stderr, "Token type : string, Token data : <%s> \n", token->data);
      break;
    default :
      fprintf(stderr, "TOKEN IS NULL\n");
      break;
  }

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
  fprintf(stderr, "PARSER START\n");
  Token *token = NULL;
  initElement();

  CondCall(scanner, &token);
  printToken(token);

  if (token->type == t_idOrKeyword && strcmp(token->data, "require") == 0) {
    CondCall(pReq);
    fprintf(stderr, "PARSER START -> ifj21 is \n");

    vypluj pCodeBody();
  } else {
    vypluj err(SYNTAX_ERR);
  }
  // asi už done a nie todo idk zomrieme TODO generate .ifjcode21
  genStart();
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
  fprintf(stderr, "-----------------------------------------------------------\n");
  fprintf(stderr, "PARSER REQUIRE\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  if (token->type == t_str && strcmp(token->data, "ifj21") == 0) {
    vypluj 0;
  } else {
    vypluj err(SYNTAX_ERR);
  }
}

/**
 * @brief Check if token is ID
 *
 * @return true if token is ID, otherwise false
 *
 */
int pNewFunId(Token *token) {

  if (token->type == t_idOrKeyword) {

    if (STFind(symtab, token->data) && STGetFnDefined(symtab, token->data)) {
      fprintf(stderr, "FUNCTION ALREADY EXISTS -> ERROR\n");
      vypluj err(ID_DEF_ERR);
    } else {
      fprintf(stderr, "CREATING NEW STACK\n");
      CondCall(STInsert, symtab, token->data);
      STSetIsVariable(symtab, token->data, false);
      STSetFnDefined(symtab, token->data, true);
    }

  } else {
    fprintf(stderr, "NOT A ID OR KEYWORD -> ERROR\n");
    vypluj err(SYNTAX_ERR);
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
  fprintf(stderr, "PARSER CODE BODY\n");

  Token *token = NULL;
  RequireToken(t_idOrKeyword);

  // 05. <codeBody>        -> function [id] ( <fnArgList> ) <fnRet> <stat> <ret> end <codeBody>
  if(strcmp(token->data, "function") == 0) {

    CondCall(scanner, &token);

    ret = pNewFunId(token);
    CondReturn;

    genFnDef(token->data);

    RequireToken(t_leftParen);

    //TODO DONE THIS
    CondCall(pFnArgList);

    RequireToken(t_rightParen);

    // <fnRet>
    // TODO DONE THIS
    CondCall(pFnRet);

    // <stat>
    // TODO DONE THIS
    CondCall(pStat);

    // <ret>
    // TODO DONE THIS
    CondCall(pRet);

    // end
    CondCall(scanner, &token);

    RequireKeyword(token->data, "end");

    // <codeBody>
    CondCall(pCodeBody);

    vypluj 0;

  } else if(strcmp(token->data, "global") == 0) {
    // 06. <codeBody>        -> global [id] : function ( <typeList> ) <fnRet> <codeBody>


    // [id]
    CondCall(scanner, &token);

    ret = pNewFunId(token);
    CondReturn;


    // :
    RequireToken(t_colon);


    // function
    CondCall(scanner, &token);

    if(strcmp(token->data, "function") != 0) {
      vypluj err(ID_DEF_ERR);
    }


    // (
    RequireToken(t_leftParen);


    // <typeList>
    CondCall(pTypeList);

    // )
    RequireToken(t_rightParen);

    // <fnRet>
    // TODO
    CondCall(pFnRet);

    // <codeBody>
    CondCall(pCodeBody);

    vypluj 0;

  } else if(token->type == t_idOrKeyword && STGetFnDefined(symtab, token->data)) {

    // <fnCall>
    CondCall(pFnCall);

    // <codeBody>
    CondCall(pCodeBody);

    vypluj 0;

  } else {
    vypluj err(SYNTAX_ERR);
  }

vypluj 0;
}

/**
 * @brief
 *
 * @return error code
 *
 * 09. <fnCall>          -> ( <fnCallArgList> )
 */
int pFnCall() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  fprintf(stderr, "PARSER FNCALL\n");
  Token *token = NULL;

  // (
  RequireToken(t_leftParen);

  // <fnCallArgList>
  CondCall(pFnCallArgList);

  // )
  fprintf(stderr, "back in fn call \n");
  RequireToken(t_rightParen);

  printf("FN CALL RET\n");
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
  printf("-----------------------------------------------------------\n");
  printf("FNRET\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // <fnRet>           -> eps
  if (token->type != t_colon) {
    printf("STASH TOKEN FNRET\n");
    CondCall(stashToken, &token);
    vypluj 0;
  }
    // <fnRet>           -> : <type> <nextType>
    // : processed by the previous if

    // <type>
    CondCall(pType);

    // <nextType>
    CondCall(pNextType);

  vypluj 0;
}

//"z pravidla 13 asi budeme musieť urobiť dve - ak je next token [id] tak
//sa použije jedno, ak to bude literál tak druhé, inak 12"
/**
 * @brief
 *
 * @return error code
 *
 * 12. <fnCallArgList>   -> eps
 * 13. <fnCallArgList>   -> <fnCallArg> <nextFnCallArg>
 */
int pFnCallArgList() {
  printf("-----------------------------------------------------------\n");
  printf("FN CALL ARG LIST\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> eps
  if(token->type == t_rightParen) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> <fnCallArg> <nextFnCallArg>
  CondCall(stashToken, &token);
  CondCall(pFnCallArg);
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
  printf("-----------------------------------------------------------\n");
  printf("NEXT FN CALL ARG\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);
  
  // -> eps
  if(token->type != t_comma) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // <fnCallArg>
  CondCall(pFnCallArg);
  
  // <nextFnCallArg>
  CondCall(pNextFnCallArg);
  
  vypluj 0;
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
  printf("-----------------------------------------------------------\n");
  printf("FN CALL ARG\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  int tokenType = token->type;

  // -> [id] (must be a variable)
  if (STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)) {
    fprintf(stderr, "JE TO PREMENNÁ\n");
    CondCall(STPush, symtab);
    CondCall(STInsert, symtab, token->data);

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
  printf("-----------------------------------------------------------\n");
  printf("RET\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> eps
  if(strcmp(token->data, "return") != 0) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // <retArgList>
  CondCall(pRetArgList);
    
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
  printf("-----------------------------------------------------------\n");
  printf("STAT\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_idOrKeyword) {
    CondCall(stashToken, &token);
    token = NULL;
    vypluj 0;
  }

  // -> local <idList> : <type> <newIdAssign> <stat>
  if (strcmp(token->data, "local") == 0) {

    // <idList>
    CondCall(pIdList);

    // :
    RequireToken(t_colon);

    // <type>
    CondCall(pType);
    
    // <newIdAssign>
    CondCall(pNewIdAssign);
    
    // <stat>
    CondCall(pStat);

    vypluj 0;
  }

  // -> if <expr> then <stat> else <stat> end <stat>
  else if (strcmp(token->data, "if") == 0) {
    // <expr>
    CondCall(pExpr);

    // then
    CondCall(scanner, &token);
    printToken(token);
    printf("TU\n");

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "then") == 0)) {
      vypluj err(SYNTAX_ERR);
    }

    CondCall(STPush, symtab); //TODO CHECK

    // <stat>
    CondCall(pStat);
    STPop(symtab);

    // else
    CondCall(scanner, &token);
    printToken(token);

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "else") == 0)) {
      vypluj err(SYNTAX_ERR);
    }
    CondCall(STPush, symtab);

    // <stat>
    CondCall(pStat);
    fprintf(stderr, "WE ARE BACK \n");

    CondCall(scanner, &token);

    // end
    if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
      vypluj err(SYNTAX_ERR);
    }
    fprintf(stderr, "DSDSDADASDASD\n");
    fprintf(stderr, "sDSDSDADAS\n");
    STPop(symtab);

    // <stat>
    CondCall(pStat);

    vypluj 0;
  }

  // -> while <expr> do <stat> end <stat>
  else if (strcmp(token->data, "while") == 0) {
    // <expr>
    CondCall(pExpr); //TODO

    // do
    CondCall(scanner, &token);
    printToken(token);

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "do") == 0)) {
      vypluj err(SYNTAX_ERR);
    }

    // <stat>
    CondCall(pStat);
    // end
    CondCall(scanner, &token);
    if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
      vypluj err(SYNTAX_ERR);
    }
    CondCall(stashToken, &token);
    vypluj 0;

  } else if(strcmp(token->data, "return") == 0) {
    vypluj 0;
  } else if(strcmp(token->data, "end") == 0) {
    printToken(token);
    fprintf(stderr, "SME V ENDE\n");

    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> [id] <statWithId> <stat>
  if (STFind(symtab, token->data)) {
    // TODO generate something
    fprintf(stderr, "NOOOOOOOOOOOOOOOOOOOOOOOOOO\n");
    // <statWithId>
    CondCall(pStatWithId);

    // <stat>
    vypluj pStat();

    // built in write function call
  } else if (strcmp(token->data, "write") == 0) {
    CondCall(pFnCall);
    fprintf(stderr, "BACK IN STAT\n");
    CondCall(pStat);
    vypluj 0;
    // TODO other built in functions call?
  }

  // -> eps
  else {
    CondCall(stashToken, &token);
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
int pStatWithId() {
  printf("-----------------------------------------------------------\n");
  printf("STAT WITH ID\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> , [id] <nextAssign> <expr> , <expr>
  if (token->type == t_comma) {

    // [id]
    RequireToken(t_idOrKeyword);

    if (!STGetIsVariable(symtab, token->data)) {
      vypluj err(SYNTAX_ERR);
    }

    genVarDef(token->data,symtab->top->depth);
    // <nextAssign>
    CondCall(pNextAssign);

    // <expr>
    CondCall(pExpr);

    // ,
    RequireToken(t_comma);

    vypluj pExpr();

  }

  // -> = <expr>
  if (strcmp(token->data, "=") == 0) {
    vypluj pExpr();
  }

  // -> <fnCall>
  vypluj pFnCall();
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
  printf("-----------------------------------------------------------\n");
  printf("NEXT ASSIGN\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);


  if (token->type == t_assignment) {
    vypluj 0;
  }

  // -> , [id] <nextAssign> <expr> ,
  if(token->type != t_comma) {
    vypluj err(SYNTAX_ERR);
  }

  // [id]
  // TODO semantic actions???
  CondCall(scanner, &token);
  printToken(token);

  if (!(token->type == t_idOrKeyword && STFind(symtab, token->data) && STGetIsVariable(symtab, token->data))) {
    vypluj err(SYNTAX_ERR);
  }

  genVarDef(token->data, symtab->top->depth);

  // <nextAssign>
  CondCall(pNextAssign);

  // <expr>
  // TODO semantic actions
  CondCall(pExpr);

  // ','
  RequireToken(t_comma);

  vypluj 0;
}

/**
 * NOT USED
 *
 * @brief Function for built in functions
 *
 * @return error code
 *  TODO ADD MORE CFG RULES FOR ALL FUNCTIONS
 * TODO ADD IT TO CFG !!!!!!!!!!!!!!!!
 */
int pBuiltInFunctions() {
  printf("-----------------------------------------------------------\n");
  printf("BUILt IN FUNCTIONS\n");
  Token *token = NULL;

  CondCall(scanner, &token);
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
  fprintf(stderr, "BUUUUUUUUUUUUUUUUUUUUUUUUUUUUU\n");
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
  printf("-----------------------------------------------------------\n");
  printf("FN ARG LIST\n");
  Token *token = NULL;

  CondCall(scanner, &token);

  printToken(token);

  // -> eps
  if(!(token->type == t_idOrKeyword && token->type != t_idOrKeyword)) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> [id] : <type> <nextFnArg>
  // [id]
  genVarDef(token->data, symtab->top->depth);
  CondCall(STInsert, symtab, token->data);

  // :
  RequireToken(t_colon);

  // <type>
  CondCall(pType);

  // <nextFnArg>
  vypluj pNextFnArg();




  // OLD CODE, DELETE IF THE NEW CODE WORKS
  /*
     if (token->type == t_rightParen) { // ) // tady nemáš co hledat závorku jeď podle gramatiky
     fprintf(stderr, "PRAVA ZATVORKA\n");
     stashToken(&token);
     token = NULL;
     vypluj 0; //TODO
     } else if (token->type == t_idOrKeyword) {

     if(isKeyword(token) == false) {
     genVarDef(token->data, symtab->top->depth);
     STInsert(symtab, token->data);
     } else{
     vypluj err(-1); // TODO ADD ERR
     }

     ret = scanner(&token);
     CondReturn;

     printToken(token);

     if(token->type != t_colon) { // proč je celá tahle funkce totálně divná
     vypluj err(-1); // TODO ADD ERR CODE
     }

     ret = pType();
     CondReturn;

     ret = pNextFnArg();
     CondReturn;

     vypluj 0;

     } else {
     fprintf(stderr, "ELSE\n");
     vypluj err(SYNTAX_ERR);
  //RETURN ERROR
  }

  vypluj 0;*/

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
  fprintf(stderr, "NEXT FN ARG\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> , [id] : <type> <nextFnArg>
  // [id]
  CondCall(scanner, &token);
  printToken(token);
  // TODO

  // :
  RequireToken(t_colon);

  // <type>
  CondCall(pType, &token);
  printToken(token);

  // <nextFnArg>
  vypluj pNextFnArg(&token);

  //vypluj pFnArgList(); // WHAT??
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
  printf("-----------------------------------------------------------\n");
  printf("RET ARG LIST\n");

  // -> <expr> <retNextArg>
  CondCall(pExpr);

  vypluj pRetNextArg();
}

bool isExpressionParser(Token token) {
  printf("is %s expression ?\n", token.data);

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
  printf("-----------------------------------------------------------\n");
  printf("REG NEXT ARG\n");
  Token *token = NULL;

  CondCall(scanner, &token);

  printToken(token);

  // -> eps
  // ','
  if (token->type != t_comma) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> , <expr> <retNextArg>
  // <expr>
  CondCall(pExpr);

  // <retNextArg>
  CondCall(pRetNextArg);

  vypluj 0;
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
  printf("-----------------------------------------------------------\n");
  printf("TYPE LIST\n");
  Token *token = NULL;

  CondCall(scanner, &token);

  printToken(token);

  // eps
  if(!isDataType(token->data)) {
    CondCall(stashToken, &token);
    vypluj 0;
  }


  CondCall(pNextType);

  vypluj 0;
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
  printf("-----------------------------------------------------------\n");
  printf("NEXT TYPE\n");
  Token *token = NULL;

  CondCall(scanner, &token);

  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> , <type> <nextType>
  CondCall(pType);

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
  printf("-----------------------------------------------------------\n");
  printf("TYPE\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  if(isDataType(token->data)) {

    // Adding data type to existing variable in ST
    // else pType() is called as data type of function, do nothing
    if (element->data != NULL) {
      STSetVarDataType(symtab, element->data, getDataTypeInt(token->data));

      // TODO call gen function for fn param definition -> fn not done yet
      genVarDef(element->data, symtab->top->depth);

      cleanElement();
    }
    vypluj 0;
  }

  vypluj err(1); // TODO ADD ERR CODE
}

/**
 * @brief
 *
 * @return error code
 *
 * 58. <idList>          -> [id] <nextId> (variable declaration)
 */
int pIdList() {
  printf("-----------------------------------------------------------\n");
  printf(" ID LIST\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  if (!(token->type == t_idOrKeyword && !isKeyword(token))) {
    vypluj err(SYNTAX_ERR);
  }

  if (STFind(symtab, token->data)) {
    printf("ISDSD\n");
    if(STGetIsVariable(symtab, token->data)) { // redeclaring a variable, the user deserves a slap
      fprintf(stderr, "warning: declaring a variable again\n");
      element->data = token->data;
      genVarDef(token->data, symtab->top->depth); // already declared, shoud be just an assignment, is this correct?? TODO
    } else { // can't function call here
      vypluj err(SYNTAX_ERR);
    }
  }
  // declaring a new variable
  CondCall(STInsert, symtab, token->data);
  STSetIsVariable(symtab, token->data, true);
  STSetFnDefined(symtab, token->data, false);
  element->data = token->data;

  STElem *e = malloc(sizeof(STElem));
  e = STFind(symtab, token->data);

  if(!e) {
    printf("DOESNT EXIST\n");
  } else {
    printf("NAME : %s\n", e->name);
  }

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
  printf("-----------------------------------------------------------\n");
  printf("NEXT ID \n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> , [id] <nextId>
  CondCall(scanner, &token);
  printToken(token);

  // [id]
  if (STFind(symtab, token->data)) {
    if(STGetIsVariable(symtab, token->data)) { // redeclaring a variable, the user deserves a slap
      fprintf(stderr, "warning: declaring a variable again\n");
      genVarDef(token->data, symtab->top->depth); // already declared, shoud be just an assignment, is this correct?? TODO
    } else { // -> eps
      CondCall(stashToken, &token);
      vypluj 0;
    }
  }

  // declaring a new variable
  CondCall(STInsert, symtab, token->data);
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
  printf("-----------------------------------------------------------\n");
  printf("NEW ID ASSIGN\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> eps
  if (!(token->type == t_assignment && !strcmp(token->data, "="))) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> = <exprList>
  // '='

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
  printf("EXPR LIST \n");
  // <expr>
  // TODO semantic actions
  CondCall(pExpr);

  // <nextExpr>
  CondCall(pNextExpr);

  vypluj 0;
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
  printf("-----------------------------------------------------------\n");
  printf("NEXT EXPR\n");
  Token *token = NULL;

  CondCall(scanner, &token);
  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    CondCall(stashToken, &token);
    vypluj 0;
  }

  // -> , <expr> <nextExpr>
  // ','

  // <expr>
  // TODO semantic actions
  CondCall(pExpr);

  // <nextExpr>
  CondCall(pNextExpr);

  vypluj 0;
}

//-----------------------------------
/**
 * @brief
 *
 * @return error code
 */
int pExpr() {
  fprintf(stderr, "-----------------------------------------------------------\n");
  fprintf(stderr, "EXPR\n");
  Token *token = NULL;
  char *varName;

  CondCall(scanner, &token);
  printToken(token);

  ret = parseExpression(symtab, token, &varName);
  printf("RET = %d\n", ret);
  printf("%s\n", varName);
  if(ret == -1) {
    printf("IDDDFFDF\n");
    CondCall(scanner, &token);

    printToken(token);

    // TODO read funkcie spraviť nejak normálne nie ako imbecil :peepoGiggle: - written by Tedro
    if(strcmp(token->data, "readi") == 0) {

      CondCall(scanner, &token);

      if(token->type == t_leftParen) {

        CondCall(scanner, &token);

        if(token->type == t_rightParen) {

          //genVarAssign(element, symtab->top->depth, "readi");

          vypluj 0;
        }
      }

      vypluj err(SYNTAX_ERR);
    } else if(strcmp(token->data, "reads") == 0) {

      CondCall(scanner, &token);

      if(token->type == t_leftParen) {

        CondCall(scanner, &token);

        if(token->type == t_rightParen) {
          // asi už nie TODO GENERATE STRING READ
          //genVarAssign(element, symtab->top->depth, "reads");

          vypluj 0;
        }
      }

      vypluj err(SYNTAX_ERR);


    } else if(strcmp(token->data, "readn") == 0) {

      CondCall(scanner, &token);

      if(token->type == t_leftParen) {

        CondCall(scanner, &token);

        if(token->type == t_rightParen) {
          // asi už nie TODO GENERATE NUMBER READ
          //genVarAssign(element, symtab->top->depth, "readn");

          vypluj 0;
        }
      }

      vypluj err(SYNTAX_ERR);
    } else if(strcmp(token->data, "substr") == 0) {
      // TODO GENERATE INTEGER READ
    } else if(strcmp(token->data, "ord") == 0) {
      // TODO GENERATE INTEGER READ
    } else if(strcmp(token->data, "chr") == 0) {
      // TODO GENERATE INTEGER READ
    } else {
      printf("KOKI\n");
      vypluj err(PARAM_RET_ERR);
    }

  } else {
    CondReturn;
  }



  /*while (true) {
    ret = scanner(&token);
    CondReturn;

    printToken(token);

    if (strcmp(token->data, "readi") == 0 ||
    strcmp(token->data, "readn") == 0 ||
    strcmp(token->data, "reads") == 0) {

    ret = scanner(&token);
    CondReturn;

    printToken(token);

    if (token->type == t_leftParen) {
    ret = scanner(&token);
    CondReturn;

    printToken(token);

    if (token->type == t_rightParen) {
    vypluj 0;
    } else {
    vypluj err(SYNTAX_ERR);
    }

    } else {
    vypluj err(SYNTAX_ERR);
    }

    } else if (isExpressionParser(*token) == false) {
    printf("FALSE\n");
    printToken(token);
    stashToken(&token);
    vypluj 0;
    } else if (isExpressionParser(*token) && token->type == t_idOrKeyword) {

    ret = scanner(&token);
    CondReturn;
    printToken(token);

    if ((token->type == t_relOp || token->type == t_arithmOp) && isExpressionParser(*token)) {

    } else {
    stashToken(&token);
    vypluj 0;
    }
    }
    }*/

  // TODO BOTTOM UP PARSER call
  // semantic actions here?

  vypluj 0;
}

#endif

/* end of file parser.c */
