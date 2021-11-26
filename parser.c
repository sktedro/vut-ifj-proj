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
 * 1. code generating not implemented yet, first parser testing
 * 2. code in testing
 * 3. change error codes
 *
 * --------------------------------------------------
 * Function             State
 * --------------------------------------------------
 * pStart()             Change error codes
 * pReq()               Change error codes
 * pCodeBody()          DONE
 * pFnCall()            Change error codes
 * pFnRet()             Change error codes
 * pFnCallArgList()     DONE
 * pNextFnCallArg()     Change error codes
 * pFnCallArg()         Change error codes
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

int ret = 0;

STStack *symtab;

//----------------------------------
// SStackElem stuff, just ignore

SStackElem *element;

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

void destroyElement() {
  free(element->next);
  free(element);
  cleanElement();
}

//------------------------------------------------------------
 
/**
 * @brief Check if string is a data type
 * 
 * @return if type is data type return 0, else 1
 */
bool isDataType(char *data) {
  if (strcmp(data, "string") == 0) {
    vypluj true;
  } else if (strcmp(data, "integer") == 0) {
    vypluj true;
  } else if (strcmp(data, "number") == 0) {
    vypluj true;
  } else {
    vypluj false;
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  if (token->type == t_idOrKeyword && strcmp(token->data, "require") == 0) {
    tokenDestroy(&token);
    ret = pReq();
    CondReturn;
    fprintf(stderr, "PARSER START -> ifj21 is \n");

    vypluj pCodeBody();
  } else {
    tokenDestroy(&token);
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

  ret = scanner(&token);
  CondReturn;
  printToken(token); 

  if (token->type == t_str && strcmp(token->data, "ifj21") == 0) {
    tokenDestroy(&token);
    vypluj 0;
  } else {
    tokenDestroy(&token);
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
      STInsert(symtab, token->data);

      STSetIsVariable(symtab, token->data, false);
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
  ret = scanner(&token);
  CondReturn;

  // -> eps
  if (token == NULL) { // EOF
    vypluj 0;
  } else if (token->type == t_idOrKeyword) {
    //-> function [id] ( <fnArgList> ) <fnRet> <stat> <ret> end <codeBody>
    if (strcmp(token->data, "function") == 0) {
      tokenDestroy(&token);
      ret = scanner(&token);
      CondReturn;

      
      // [id] - function name
      ret = pNewFunId(token);
      CondReturn;
      STSetFnDefined(symtab, token->data, true);

      genFnDef(token->data);
      // (
      ret = scanner(&token);
      CondReturn;
      printToken(token); 

      if (token->type != t_leftParen) {
        destroyElement();
        tokenDestroy(&token);
        vypluj err(SYNTAX_ERR);
      }
      tokenDestroy(&token);
      //TODO TU POKRAČUJ 

      // <fnArgList>
      ret = pFnArgList();

      CondReturn;

      // )
      ret = scanner(&token);

      CondReturn;
      printToken(token); 
      if (token->type != t_rightParen) {
        tokenDestroy(&token);
        vypluj err(SYNTAX_ERR);
      }
      tokenDestroy(&token);

      // <fnRet>
      ret = pFnRet();
      CondReturn;
      // <stat>
      // TODO new stack frame (symbol table)
      ret = pStat();
      CondReturn;
      // <ret>
      ret = pRet();
      CondReturn;
      // end
      ret = scanner(&token);
      CondReturn;
      if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
        STDestroy(&symtab);
        tokenDestroy(&token);
        vypluj err(SYNTAX_ERR);
      }
      STPop(symtab);
      tokenDestroy(&token);

      // <codeBody>
      ret = pCodeBody();
      CondReturn;
      //-> global [id] : function ( <typeList> ) <fnRet> <codeBody>
    } else if (strcmp(token->data, "global") == 0) {
      // [id]
      ret = scanner(&token);
      CondReturn;

      printToken(token);

      ret = pNewFunId(token);
      CondReturn;
      
      STSetFnDefined(symtab, token->data, false);
      tokenDestroy(&token);

      // :
      ret = scanner(&token);
      CondReturn;

      printToken(token);

      if (token->type != t_colon) {
        tokenDestroy(&token);
        vypluj err(SYNTAX_ERR);
      }
      tokenDestroy(&token);

      // function
      ret = scanner(&token);
      CondReturn;
      if (!(token->type == t_idOrKeyword && strcmp(token->data, "function") == 0)) {
        tokenDestroy(&token);
        vypluj err(SYNTAX_ERR);
      }
      tokenDestroy(&token);

      // (
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_leftParen) {
        tokenDestroy(&token);
        vypluj err(SYNTAX_ERR);
      }
      tokenDestroy(&token);

      // <typeList>
      ret = pTypeList();
      CondReturn;

      // )
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_rightParen) {
        tokenDestroy(&token);
        vypluj err(SYNTAX_ERR);
      }
      tokenDestroy(&token);

      // <fnRet>
      ret = pFnRet();
      CondReturn;

      // <codeBody>
      ret = pCodeBody();
      CondReturn;
    }

    //-> [id] <fnCall> <codeBody> - calling a build in function
    else if (isBuiltInFunction(token)) {
      if (strcmp(token->data, "write") == 0) {

        ret = pFnCall();
        CondReturn;
        ret = pCodeBody();
        CondReturn;
        vypluj 0;
      }

    //-> [id] <fnCall> <codeBody> - calling a user function
    } else if (STFind(symtab, token->data) != NULL) {
      fprintf(stderr, "user function call\n");
      // [id]
      // If the id is a variable or is not defined yet, we can't call it as a function...
      
      STElem *element = STFind(symtab, token->data);
      fprintf(stderr, "ELEMENT %s\n",element->name);

      if(element->fnDefined) {
        fprintf(stderr, "TRUE\n");
      } else {
        fprintf(stderr, "FALSE\n");
      }
      

      if (STGetIsVariable(symtab, token->data) || !STGetFnDefined(symtab, token->data)) {
        fprintf(stderr, "FN NENI DEFINOVANÁ\n");
        vypluj err(ID_DEF_ERR);
      }
      fprintf(stderr, "FN JE DEFINOVANÁ\n");

      // <fnCall>
      ret = pFnCall();
      CondReturn;
      // TODO generate code?

      // <codeBody>
      ret = pCodeBody();
      CondReturn;
    } else {
      vypluj err(ID_DEF_ERR); // TODO errcode asi good?
    }
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
  ret = scanner(&token);
  CondReturn;
  printToken(token);

  if (token->type != t_leftParen) {
    tokenDestroy(&token);
    vypluj err(SYNTAX_ERR);
  }
  tokenDestroy(&token);

  // <fnCallArgList>
  ret = pFnCallArgList();
  CondReturn;

  // )
  fprintf(stderr, "back in fn call \n");
  ret = scanner(&token);
  CondReturn;

  printToken(token); 

  if (token->type != t_rightParen) {
    tokenDestroy(&token);
    vypluj err(SYNTAX_ERR);
  }
  tokenDestroy(&token);
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // <fnRet>           -> eps
  if (token->type != t_colon) { // :
    printf("STASH TOKEN FNRET\n");
    stashToken(token);
    vypluj 0;

  // <fnRet>           -> : <type> <nextType>
  } else {
    // : processed by the previous if
    tokenDestroy(&token);

    // <type>
    ret = pType();
    CondReturn;

    // <nextType>
    ret = pNextType();
    CondReturn;
  }
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // -> eps
  if(token->type == t_rightParen) {
    stashToken(token);
    vypluj 0;
  }

  // -> <fnCallArg> <nextFnCallArg>
  stashToken(token);
  ret = pFnCallArg();
  CondReturn;
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // -> , <fnCallArg> <nextFnCallArg>
  if (token->type == t_comma) {
    // <fnCallArg>
    ret = pFnCallArg();
    CondReturn;
    // <nextFnCallArg>
    ret = pNextFnCallArg();
    CondReturn;
    vypluj 0;

  // -> eps
  } else {
    stashToken(token);
    vypluj 0;
  }
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  int tokenType = token->type;

  // -> [id] (must be a variable)
  if (STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)) {
    fprintf(stderr, "JE TO PREMENNÁ\n");
    STPush(symtab);
    STInsert(symtab, token->data);

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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // -> return <retArgList>
  if (token->type == t_idOrKeyword && strcmp(token->data, "return") == 0) {
    tokenDestroy(&token);

    // <retArgList>
    ret = pRetArgList();
    CondReturn;
        vypluj 0;
  }
  // -> eps
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // -> eps
  if (token->type != t_idOrKeyword) {
    stashToken(token);
    token = NULL;
    vypluj 0;
  }

  // -> local <idList> : <type> <newIdAssign> <stat>
  if (strcmp(token->data, "local") == 0) {
    // <idList>
    
    ret = pIdList();
    CondReturn;
    tokenDestroy(&token);

    // :
    ret = scanner(&token);
    CondReturn;
    printToken(token);
    if (token->type != t_colon) {
      tokenDestroy(&token);
      vypluj err(SYNTAX_ERR);
    }
    tokenDestroy(&token);

    // <type>
    
    ret = pType();
    CondReturn;
    // <newIdAssign>
    ret = pNewIdAssign();
    CondReturn;
    // <stat>
    ret = pStat();
    CondReturn;

    vypluj 0;
  }

  // -> if <expr> then <stat> else <stat> end <stat>
  else if (strcmp(token->data, "if") == 0) {
    // <expr>
    ret = pExpr();
    CondReturn;
    tokenDestroy(&token);

    // then
    ret = scanner(&token);
    CondReturn;
    printToken(token);
    printf("TU\n");

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "then") == 0)) {
      tokenDestroy(&token);
      vypluj err(SYNTAX_ERR);
    }
    tokenDestroy(&token);

    STPush(symtab); //TODO CHECK

    // <stat>
    ret = pStat();
    CondReturn;
    STPop(symtab);

    // else
    ret = scanner(&token);
    CondReturn;
    printToken(token);

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "else") == 0)) {
      tokenDestroy(&token);
      vypluj err(SYNTAX_ERR);
    }
    tokenDestroy(&token);
    STPush(symtab);

    // <stat>
    ret = pStat();
    CondReturn;
    fprintf(stderr, "WE ARE BACK \n");

    ret = scanner(&token);
    CondReturn;

    // end
    if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
      tokenDestroy(&token);
      vypluj err(SYNTAX_ERR);
    }
    fprintf(stderr, "DSDSDADASDASD\n");
    tokenDestroy(&token);
    fprintf(stderr, "sDSDSDADAS\n");
    STPop(symtab);

    // <stat>
    ret = pStat();
    CondReturn;

    vypluj 0;
  }

  // -> while <expr> do <stat> end <stat>
  else if (strcmp(token->data, "while") == 0) {
    // <expr>
    ret = pExpr(); //TODO
    CondReturn;
    tokenDestroy(&token);

    // do
    ret = scanner(&token);
    CondReturn;
    printToken(token);

    if (!(token->type == t_idOrKeyword && strcmp(token->data, "do") == 0)) {
      tokenDestroy(&token);
      vypluj err(SYNTAX_ERR);
    }
    tokenDestroy(&token);

    // <stat>
    ret = pStat();
    CondReturn;
    // end
    ret = scanner(&token);
    if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
      tokenDestroy(&token);
      vypluj err(SYNTAX_ERR);
    }
    stashToken(token);
    vypluj 0;
    
  } else if(strcmp(token->data, "return") == 0) {
    tokenDestroy(&token);
    vypluj 0;
  } else if(strcmp(token->data, "end") == 0) {
    printToken(token);
    fprintf(stderr, "SME V ENDE\n");
  
    stashToken(token);
    vypluj 0;
  }

  // -> [id] <statWithId> <stat>
  if (STFind(symtab, token->data)) {
    // TODO generate something
    tokenDestroy(&token);
    fprintf(stderr, "NOOOOOOOOOOOOOOOOOOOOOOOOOO\n");
    // <statWithId>
    ret = pStatWithId();
    CondReturn;

    // <stat>
    vypluj pStat();

  // built in write function call
  } else if (strcmp(token->data, "write") == 0) {
    ret = pFnCall();
    CondReturn;
    fprintf(stderr, "BACK IN STAT\n");
    ret = pStat();
    CondReturn;
    vypluj 0;
    // TODO other built in functions call?
  }

  // -> eps
  else {
    stashToken(token);
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // -> , [id] <nextAssign> <expr> , <expr>
  if (token->type == t_comma) {
    tokenDestroy(&token);

    // [id]
    ret = scanner(&token);
    CondReturn;
    printToken(token);

    if (token->type != t_idOrKeyword) {
      vypluj err(SYNTAX_ERR);
    }
    if (!STGetIsVariable(symtab, token->data)) {
      vypluj err(SYNTAX_ERR);
    }

    genVarDef(token->data,symtab->top->depth);
    // <nextAssign>
    ret = pNextAssign();
    CondReturn;

    // <expr>
    ret = pExpr();
    CondReturn;

    // ,
    ret = scanner(&token);
    CondReturn;
    printToken(token);

    if (token->type != t_comma) {
      vypluj err(SYNTAX_ERR);
    }
    
    tokenDestroy(&token);
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);


  if (token->type == t_assignment) {
    vypluj 0;
  }
  
  // -> , [id] <nextAssign> <expr> ,
  if(token->type != t_comma) {
    vypluj err(ASS_ERR); 
  }

  tokenDestroy(&token);
  // [id]
  // TODO semantic actions???
  ret = scanner(&token);
  CondReturn;
   printToken(token);

  if (!(token->type == t_idOrKeyword && STFind(symtab, token->data) && STGetIsVariable(symtab, token->data))) {
    vypluj err(SYNTAX_ERR); 
  }  

  genVarDef(token->data, symtab->top->depth);
  
  // <nextAssign>
  ret = pNextAssign();
  CondReturn;
  
  // <expr>
  // TODO semantic actions
  ret = pExpr();
  CondReturn;

  // ','
  ret = scanner(&token);
  CondReturn;
  printToken(token);

  if (token->type != t_comma) {
    tokenDestroy(&token);
    vypluj err(SYNTAX_ERR); // TODO maybe ass_err my brain can't compreheend errors right now
  }
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

  ret = scanner(&token);
  CondReturn;
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

  ret = scanner(&token);
  CondReturn;
  
  printToken(token);

  // -> eps
  if(!(token->type == t_idOrKeyword && token->type != t_idOrKeyword)) {
    stashToken(token);
    vypluj 0;
  }

  // -> [id] : <type> <nextFnArg>
  // [id]
  genVarDef(token->data, symtab->top->depth);
  STInsert(symtab, token->data);
  tokenDestroy(&token);

  // :
  ret = scanner(&token);
  CondReturn;
  printToken(token);

  if(token->type != t_colon) { 
    tokenDestroy(&token); 
    vypluj err(SYNTAX_ERR);
  }
  tokenDestroy(&token);

  // <type>
  ret = pType();
  CondReturn;

  // <nextFnArg>
  vypluj pNextFnArg();
  

// OLD CODE, DELETE IF THE NEW CODE WORKS
/*
  if (token->type == t_rightParen) { // ) // tady nemáš co hledat závorku jeď podle gramatiky
    fprintf(stderr, "PRAVA ZATVORKA\n");
    stashToken(token);
    token = NULL;
    vypluj 0; //TODO
  } else if (token->type == t_idOrKeyword) {
    
    if(isKeyword(token) == false) {
      genVarDef(token->data, symtab->top->depth);
      STInsert(symtab, token->data);
      tokenDestroy(&token);
    } else{
      vypluj err(-1); // TODO ADD ERR
    }
    
    ret = scanner(&token);
    CondReturn;

    printToken(token);

    if(token->type != t_colon) { // proč je celá tahle funkce totálně divná
      tokenDestroy(&token); 
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    tokenDestroy(&token);
    stashToken(token);
    vypluj 0;
  }
  tokenDestroy(&token);

  // -> , [id] : <type> <nextFnArg>
  // [id]
  ret = scanner(&token);
  CondReturn;
  printToken(token);
  // TODO
  tokenDestroy(&token);

  // :
  ret = scanner(&token);
  CondReturn;
  printToken(token);

  if(token->type != t_colon) {
    tokenDestroy(&token);
    vypluj err(SYNTAX_ERR); 
  }
  tokenDestroy(&token);

  // <type>
  ret = pType(&token);
  CondReturn;
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
  //Token *token = NULL;

  // -> eps
  //TODO

  // -> <expr> <retNextArg>
  ret = pExpr();
  CondReturn;

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

  ret = scanner(&token);
  CondReturn;

      printToken(token);

  // -> eps
  // ','
  if (token->type != t_comma) {
    stashToken(token);
    vypluj 0;
  }
  tokenDestroy(&token);

  // -> , <expr> <retNextArg>
  // <expr>
  // TODO semantic actions
  ret = pExpr();
  CondReturn;

  // <retNextArg>
  ret = pRetNextArg();
  CondReturn;

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

  ret = scanner(&token);
  CondReturn;

  printToken(token);

  if (token->type == t_idOrKeyword && isDataType(token->data)) {

      //TODO SAVE TOKEN TYPE FOR GENERATING VARIABLE IN IFJ21CODE

      vypluj 0;

    // Huh?
    /*if (strcmp(token->data, "nil") == 0) {
      tokenDestroy(&token);
      ret = pNextType();
      CondReturn;
    }*/
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
 * 50. <nextType>        -> eps
 * 51. <nextType>        -> , <type> <nextType>
 */
int pNextType() {
  printf("-----------------------------------------------------------\n");
  printf("NEXT TYPE\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn;

  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    stashToken(token);
    vypluj 0;
  }

  // -> , <type> <nextType>
  ret = pType();
  CondReturn;

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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  if ((strcmp(token->data, "nil") == 0) || isDataType(token->data)) {
    printf("IF\n");
    if(strcmp(token->data, "integer") == 0) {
      STSetVarDataType(symtab, element->data,dt_integer);
    } else if(strcmp(token->data, "number") == 0) {
      STSetVarDataType(symtab, element->data,dt_number);
    } else if(strcmp(token->data, "string") == 0) {
      STSetVarDataType(symtab, element->data,dt_string);
    } else if(strcmp(token->data, "nil") == 0) {
      STSetVarDataType(symtab, element->data,dt_nil);
    } else {
      vypluj err(0);
    }

    STElem *e = malloc(sizeof(STElem));
    e = STFind(symtab, element->data);
    if(!e){
      printf("is null since it does not exist in the symbol table (yet)\n");
    }

    printf("DATA : %s\n", e->name);
    printf("TYPE : %d\n", e->varDataType);
    free(e);
    cleanElement();
    fprintf(stderr, "IS DATA TYPE\n");
    vypluj 0;
  } else {
    vypluj err(SYNTAX_ERR);
  }
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

  ret = scanner(&token);
  CondReturn;
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
      tokenDestroy(&token);
    } else { // can't function call here
      vypluj err(SYNTAX_ERR);
    }
  }
  // declaring a new variable
  STInsert(symtab, token->data);
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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  // -> eps
  if (token->type != t_comma) {
    stashToken(token);
    vypluj 0;
  }
  tokenDestroy(&token);

  // -> , [id] <nextId>
  ret = scanner(&token);
  CondReturn;
  printToken(token);
  
  // [id]
  if (STFind(symtab, token->data)) {
    if(STGetIsVariable(symtab, token->data)) { // redeclaring a variable, the user deserves a slap
      fprintf(stderr, "warning: declaring a variable again\n");
      genVarDef(token->data, symtab->top->depth); // already declared, shoud be just an assignment, is this correct?? TODO
      tokenDestroy(&token);
    } else { // -> eps
      stashToken(token);
      vypluj 0;
    }
  }
  
  // declaring a new variable
  STInsert(symtab, token->data);
  STSetIsVariable(symtab, token->data, true);
  STSetFnDefined(symtab, token->data, false);
  
  // <nextId>
  tokenDestroy(&token);
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

  ret = scanner(&token);
  CondReturn;

      printToken(token);

  // -> eps
  if (!(token->type == t_assignment && !strcmp(token->data, "="))) {
    stashToken(token);
    vypluj 0;
  }

  // -> = <exprList>
  // '='
  tokenDestroy(&token);

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
  ret = pExpr();
  CondReturn;

      // <nextExpr>
      ret = pNextExpr();
  CondReturn;

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

  ret = scanner(&token);
  CondReturn;

      printToken(token);

  // -> eps
  if (token->type != t_comma) {
    stashToken(token);
    vypluj 0;
  }

  // -> , <expr> <nextExpr>
  // ','
  tokenDestroy(&token);

  // <expr>
  // TODO semantic actions
  ret = pExpr();
  CondReturn;

  // <nextExpr>
  ret = pNextExpr();
  CondReturn;

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

  ret = scanner(&token);
  CondReturn;
  printToken(token);

  ret = parseExpression(symtab, token, &varName);
  printf("RET = %d\n", ret);
  printf("%s\n", varName);
  if(ret == -1) {
    printf("IDDDFFDF\n");
    ret = scanner(&token);
    CondReturn;

    printToken(token);

    // TODO read funkcie spraviť nejak normálne nie ako imbecil :peepoGiggle: - written by Tedro
    if(strcmp(token->data, "readi") == 0) {
      tokenDestroy(&token);

      ret = scanner(&token);
      CondReturn;
      
      if(token->type == t_leftParen) {
        tokenDestroy(&token);

        ret = scanner(&token);
        CondReturn;

        if(token->type == t_rightParen) {
          
          //genVarAssign(element, symtab->top->depth, "readi");
  
          tokenDestroy(&token);
          vypluj 0;
        }
      }
      
      vypluj err(SYNTAX_ERR); 
    } else if(strcmp(token->data, "reads") == 0) {
      tokenDestroy(&token);

      ret = scanner(&token);
      CondReturn;
      
      if(token->type == t_leftParen) {
        tokenDestroy(&token);

        ret = scanner(&token);
        CondReturn;

        if(token->type == t_rightParen) {
          tokenDestroy(&token);
          // asi už nie TODO GENERATE STRING READ
           //genVarAssign(element, symtab->top->depth, "reads");
          
          vypluj 0;
        }
      }
      
      vypluj err(SYNTAX_ERR); 
      
      
    } else if(strcmp(token->data, "readn") == 0) {
      tokenDestroy(&token);

      ret = scanner(&token);
      CondReturn;
      
      if(token->type == t_leftParen) {
        tokenDestroy(&token);

        ret = scanner(&token);
        CondReturn;

        if(token->type == t_rightParen) {
          tokenDestroy(&token);
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
      stashToken(token);
      //tokenDestroy(&token);
      vypluj 0;
    } else if (isExpressionParser(*token) && token->type == t_idOrKeyword) {

      ret = scanner(&token);
      CondReturn;
      printToken(token);

      if ((token->type == t_relOp || token->type == t_arithmOp) && isExpressionParser(*token)) {
      
      } else {
        stashToken(token);
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
