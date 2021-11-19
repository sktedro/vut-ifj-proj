/*
 * Parser
 */

/** IMPORTANT !!! READ FIRST, BEFORE EDITING
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

// TODO //tokenDestroy(&token) instead of just passing the token?

#ifndef PARSER_C
#define PARSER_C

#include "parser.h"

#define CondReturn \
  if (ret)         \
    vypluj err(ret);

STStack *symtab;

int ret = 0;


/**
 * @brief Check if string is a data type
 * 
 * @return if type is data type retrun 0, else 1
 */
int isDataType(char *type) {
  if (strcmp(type, "string") == 0) {
    return 0;
  } else if (strcmp(type, "integer") == 0) {
    return 0;
  } else if (strcmp(type, "number") == 0) {
    return 0;
  } else {
    return 1;
  }
}

/**
 * @brief This function is for debug print of token
 *
 * @param token
 */
void printToken(Token *token) {

  switch (token->type) {
    case t_idOrKeyword:
      printf("Token type : idOrKeyword, Token data : <%s>\n", token->data);
      break;
    case t_colon:
      printf("Token type : colon, Token data : <%s>\n", token->data);
      break;
    case t_rightParen:
      printf("Token type : rightParen, Token data : <%s>\n", token->data);
      break;
    case t_comma:
      printf("Token type : comma, Token data : <%s>\n", token->data);
      break;
    case t_arithmOp:
      printf("Token type : arithmeticOperation, Token data : <%s>\n", token->data);
      break;
    case t_assignment:
      printf("Token type : assigment, Token data : <%s> \n", token->data);
      break;
    case t_int:
      printf("Token type : integer, Token data : <%s> \n", token->data);
      break;
    case t_leftParen:
      printf("Token type : leftParen, Token data : <%s> \n", token->data);
      break;
    case t_num:
      printf("Token type : num, Token data : <%s> \n", token->data);
      break;
    case t_relOp:
      printf("Token type : relationOperation, Token data : <%s> \n", token->data);
      break;
    case t_strOp:
      printf("Token type : stringOperation, Token data : <%s> \n", token->data);
      break;
    case t_sciNum:
      printf("Token type : scientificNumber, Token data : <%s> \n", token->data);
      break;
    case t_str:
      printf("Token type : string, Token data : <%s> \n", token->data);
      break;
  }

}


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
int pStart() {
  printf("-----------------------------------------------------------\n");
  printf("PARSER START\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn

  printToken(token);

  if (token->type == t_idOrKeyword && strcmp(token->data, "require") == 0) {
    //tokenDestroy(token);
    ret = pReq();
    CondReturn;
    printf("PARSER START -> ifj21 is \n");

    ret = pCodeBody();
    CondReturn;
    vypluj 0;
  } else {
    //tokenDestroy(token);
    vypluj err(1); // TODO aky kod?
  }
  vypluj 0;
}


/**
 * @brief
 *
 * @return error code 
 *
 * 02. <req>             -> "ifj21"
 */
int pReq() {
  printf("-----------------------------------------------------------\n");
  printf("PARSER REQUIRE\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn

  printToken(token);

  if (token->type == t_str && strcmp(token->data, "ifj21") == 0) {
    //tokenDestroy(token);
    vypluj 0;
  } else {
    //tokenDestroy(token);
    vypluj err(1); // TODO aky kod?
  }
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
  printf("-----------------------------------------------------------\n");
  printf("PARSER CODE BODY\n");
  Token *token = NULL;
  ret = scanner(&token);
  CondReturn;

  printToken(token);

  // -> eps
  if (token == NULL) {
    vypluj 0;
  } else if (token->type == t_idOrKeyword) {

    //-> function [id] ( <fnArgList> ) <fnRet> <stat> <ret> end <codeBody>
    if (strcmp(token->data, "function") == 0) {

      //tokenDestroy(token);
      ret = scanner(&token);
      CondReturn;

      printToken(token);

      // [id] - function name
      if (token->type == t_idOrKeyword) {
        if (STFind(symtab, token->data) && STGetFnDefined(symtab, token->data)) {
          printf("FUNCTION ALREADY EXISTS -> ERROR\n");
          vypluj err(1); // TODO RETURN ERROR
        } else {
          printf("CREATING NEW STACK\n");
          STInsert(symtab, token->data);
        }
      } else {
        printf("NOT A ID OR KEYWORD -> ERROR\n");
        vypluj err(1); //TODO ERROR
      }

      // (
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_leftParen) {
        //tokenDestroy(token);
        vypluj err(1); // TODO what code
      } else {
        //tokenDestroy(token);
      }


      // <fnArgList>
      ret = pFnArgList();
      CondReturn;

      // )
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_rightParen) {
        //tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      //tokenDestroy(token);

      // <fnRet>
      ret = pFnRet();
      CondReturn;

      // <stat>
      // TODO new stack frame (symbol table)
      ret = pStart();
      CondReturn;

      // <ret>
      ret = pRet();
      CondReturn;

      // end
      ret = scanner(&token);
      CondReturn;

      if (!(token->type == t_idOrKeyword && strcmp(token->data, "end") == 0)) {
        STDestroy(&symtab);
        //tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      STPop(symtab);
      //tokenDestroy(token);

      // <codeBody>
      ret = pCodeBody();
      CondReturn;

      //-> global [id] : function ( <typeList> ) <fnRet> <codeBody>
    } else if (strcmp(token->data, "global") == 0) {
      // [id]
      if (token->type == t_idOrKeyword) {
        if (STFind(symtab, token->data)) {
          vypluj err(1); // TODO RETURN ERROR
        } else {
          STInsert(symtab, token->data);
        }
      } else {
        vypluj err(1); //TODO ERROR
      }
      //tokenDestroy(token);

      // :
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_colon) {
        //tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      //tokenDestroy(token);

      // function
      ret = scanner(&token);
      CondReturn;

      if (!(token->type == t_idOrKeyword && strcmp(token->data, "function") == 0)) {
        //tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      //tokenDestroy(token);

      // (
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_leftParen) {
        //tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      //tokenDestroy(token);

      // <typeList>
      ret = pTypeList();
      CondReturn;

      // )
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_rightParen) {
        //tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      //tokenDestroy(token);

      // <fnRet>
      ret = pFnRet();
      CondReturn;

      // <codeBody>
      ret = pCodeBody();
      CondReturn;

      //-> [id] <fnCall> <codeBody>
    } else if (STFind(symtab, token->data) != NULL) { // [ID]
      // [id]
      // If the id is a variable or is not defined yet, we can't call it as a
      // function...
      if (STGetIsVariable(symtab, token->data) != false || !STGetFnDefined(symtab, token->data)) {
        printf("FN NENI DEFINOVANÁ\n");
        vypluj err(1); // TODO RETURN ERROR
      }

      printf("FN JE DEFINOVANÁ\n");

      // <fnCall>
      ret = pFnCall();
      CondReturn;

      // TODO generate code?

      // <codeBody>
      ret = pCodeBody();
      CondReturn;

    } else {
      vypluj err(1); // TODO errcode
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
  printf("-----------------------------------------------------------\n");
  printf("PARSER FNCALL\n");
  Token *token = NULL;

  // (
  ret = scanner(&token);
  CondReturn

  printToken(token);

  if (token->type != t_leftParen) {
    //tokenDestroy(token);
    vypluj err(1); // TODO what code
  }
  //tokenDestroy(token);

  ret = pFnCallArgList();
  CondReturn;

  // )
  ret = scanner(&token);
  CondReturn;

  printToken(token);

  if (token->type != t_rightParen) {
    //tokenDestroy(token);
    vypluj err(1); // TODO what code
  }
  //tokenDestroy(token);

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
  CondReturn

  printToken(token);

  if(token->type != t_colon){
    stashToken(token);
    return 0;
  }else{
    // : processed by the previous if
    //tokenDestroy(token);

    // type
    ret = pType();
    CondReturn;
    
    // nextType
    ret = pNextType();
    CondReturn;
  }
  vypluj 0;
}


//TODO NOT SURE HOW TO CODE IT
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
  CondReturn

  printToken(token);

  if(token->type == t_rightParen) {
    stashToken(token);
    //tokenDestroy(token);
    return 0;
  } else {
    //tokenDestroy(token);
    if(pFnCallArg() == 0) {
      if(pNextFnCallArg() == 0) {
        return 0;
      }
    }
  }
  vypluj err(1);
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
  CondReturn

  printToken(token);

  // If the next token is not a comma, use rule 14 (else rule 15)
  // ','
  if(token->type != t_comma){
    stashToken(token);
    vypluj err(1);
  }
  //tokenDestroy(token);

  // <fnCallArg> 
  ret = pFnCallArg();
  CondReturn

  // <nextFnCallArg>
  ret = pNextFnCallArg();
  CondReturn;

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

  ret = scanner(&token);
  CondReturn

  printToken(token);

  int tokenType = token->type;

  // Is a variable?
  if(STFind(symtab, token->data) && STGetIsVariable(symtab, token->data)){
    printf("JE TO PREMENNÁ\n");
    STPush(symtab);
    STInsert(symtab, token->data);

    // TODO semantic actions (add to fn arg types or something)
    vypluj 0;
  // Is a literal?
  } else if(tokenType == t_int 
      || tokenType == t_num 
      || tokenType == t_sciNum
      || tokenType == t_str){
    printf("JE TO LITERÁL\n");
    // TODO semantic actions
    vypluj 0;
  } else {
    printf("NENI TO ANI PREMENNÁ A ANI LITERÁL\n");
    vypluj err(1); // TODO errcode
  }

  vypluj 0;
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
  CondReturn

  printToken(token);

  // If the next token is 'return', apply rule 20
  if(token->type == t_idOrKeyword && strcmp(token->data, "return") == 0){
    //tokenDestroy(token);

    // <retArgList>
    ret = pRetArgList();
    CondReturn

    vypluj 0;
  }
  // Else apply rule 19
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
  CondReturn

  printToken(token);

  if(token->type == t_idOrKeyword) {
    if(STGetIsVariable(symtab, token->data)) {
      //tokenDestroy(token);
      pStatWithId();
      vypluj 0;
    } else {

      if(strcmp(token->data, "local") == 0) {
        ret = pIdList();
        CondReturn

        //tokenDestroy(token);

        ret = scanner(&token);
        CondReturn

        printToken(token);

        if(token->type == t_colon) {
          //tokenDestroy(token);

          ret = pType();
          CondReturn

          ret = pNewIdAssign();
          CondReturn

          ret = pStat();
          CondReturn

        } else {
          vypluj err(1);
        }

      } else if(strcmp(token->data, "if") == 0) {
        ret = pExpr();
        CondReturn

        //tokenDestroy(token);
        ret = scanner(&token);

        printToken(token);

        if(token->type == t_idOrKeyword && strcmp(token->data,"then") == 0) {
          //tokenDestroy(token);

          STPush(symtab); //TODO CHECK

          ret = pStat();
          CondReturn

          STPop(symtab);

          ret = scanner(&token);
          CondReturn

          printToken(token);

          if(token->type == t_idOrKeyword && strcmp(token->data,"else") == 0) {
            //tokenDestroy(token);

            STPush(symtab);

            ret = pStat();
            CondReturn

            if(token->type == t_idOrKeyword && strcmp(token->data,"end") == 0) {
              //tokenDestroy(token);
              STPop(symtab);

              ret = pStat();
              CondReturn

            }

          }

        }

      } else if(strcmp(token->data, "while") == 0) {
        ret = pExpr(); //TODO
        CondReturn

        ret = scanner(&token);
        CondReturn

        printToken(token);

        if(token->type == t_idOrKeyword && strcmp(token->data, "do") == 0) {
          //tokenDestroy(token);

          ret = pStat();
          CondReturn

          ret = pStat();
          CondReturn

        } else {
          vypluj err(1);
        }


      } else if(strcmp(token->data, "end") == 0) {
        //tokenDestroy(token);
        pStat();
      } else {
        vypluj err(1); // TODO errcode
      }

    }


  } if(token == NULL) {
    //tokenDestroy(token);
    vypluj 0;
  } else {
    vypluj err(1); // TODO errcode
  }

  vypluj 0;
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
  CondReturn

  printToken(token);

  if(token->type == t_comma) {
    //tokenDestroy(token);

    ret = scanner(&token);
    CondReturn

    printToken(token);

    if(token->type == t_idOrKeyword) {

      if(!STGetIsVariable(symtab, token->data)) {
        vypluj err(1);
      }

      ret = pNextAssign();
      CondReturn

      ret = pExpr();
      CondReturn

      ret = scanner(&token);
      CondReturn

      printToken(token);

      if(token->type == t_comma) {
        //tokenDestroy(token);

        ret = pExpr();
        CondReturn
      }

    } else {
      vypluj err(1);
    }

  } else if(strcmp(token->data, "=") == 0) {
    pExpr();
    //TODO
  } else {
    pFnCall();
    //TODO CHECK
  }

  vypluj 0;
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
  CondReturn

  printToken(token);

  // ','
  if(token->type == t_comma) {
    //tokenDestroy(token);

    // [id]
    // TODO semantic actions???
    ret = scanner(&token);
    CondReturn

    printToken(token);

    if(token->type == t_idOrKeyword 
        && STFind(symtab, token->data) 
        && STGetIsVariable(symtab, token->data)) {
      ret = pNextAssign();
      CondReturn;
    } else {
      vypluj err(1); // TODO errcode
    }

    // <expr>
    // TODO semantic actions
    ret = pExpr();
    CondReturn
    
    // ','
    ret = scanner(&token);
    CondReturn

    printToken(token);

    if(token->type == t_comma){
      //tokenDestroy(token);
      vypluj 0;
    }

  // '='
  } else if(token->type == t_assignment) {
    vypluj 0;
  } else {
    vypluj err(1); // TODO errcode
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
  printf("-----------------------------------------------------------\n");
  printf("FN ARG LIST\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn

  printToken(token);

  if (token->type == t_rightParen) {
    printf("PRAVA ZATVORKA\n");

    stashToken(token);
    //tokenDestroy(token);
    vypluj 0; //TODO
  } else if (token->type == t_idOrKeyword) {
    printf("ID OR KEYWORD\n");
    //tokenDestroy(token);

    ret = scanner(&token);
    CondReturn

    printToken(token);

    if (STGetIsVariable(symtab, token->data) != false || !STGetFnDefined(symtab, token->data)) {
      vypluj err(1); // TODO RETURN ERROR
    }

    //tokenDestroy(token);

    ret = scanner(&token);
    CondReturn

    printToken(token);

    if (token->type != t_colon) {
      vypluj err(1);
    }

    //tokenDestroy(token);

    ret = scanner(&token);
    CondReturn

    printToken(token);

    if (token->type != t_idOrKeyword) {
      //TODO NENI TO KEYWORD
      vypluj err(1);
    }

    if (isDataType(token->data) == 1) {
      //TODO NENI TO datatype
      vypluj err(1);
    }

    //tokenDestroy(token);

    pNextFnArg();
    vypluj 0;

  } else {
    printf("ELSE\n");
    vypluj err(1);
    //RETURN ERROR
  }

  vypluj 0;
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
  printf("-----------------------------------------------------------\n");
  printf("NEXT FN ARG\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn

  printToken(token);

  if(token->type != t_comma) {
    //TODO ERROR
    vypluj err(1);
  }

  //tokenDestroy(token);

  ret = pFnArgList();
  CondReturn

  vypluj 0;
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
  Token *token = NULL;

  ret = pExpr();
  CondReturn

  vypluj 0;
}

bool isExpressionParser(Token token) {
  printf("is %s expression ?\n", token.data);

  if(token.type == t_leftParen) {
    return true;
  } else if(token.type == t_str) {
    return true;
  } else if(token.type == t_arithmOp) {
    if(strcmp(token.data, "+") == 0 || strcmp(token.data, "-") == 0) {
      return true;
    }
  } else if(token.type == t_int) {
    return true;
  } else if(token.type == t_sciNum) {
    return true;
  } else if(token.type == t_num) {
    return true;
  } else if(token.type == t_idOrKeyword) {

    if(strcmp(token.data, "nil") == 0) {
      return true;
    } else if(STGetIsVariable(symtab, token.data)) {
      return true;
    }

  }

  return false;
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
  CondReturn

  printToken(token);

  // If the next token is not a comma, use rule 41 (else rule 42)
  // ','
  if(token->type != t_comma){
    stashToken(token);
    vypluj 0;
  }

  // ','
  //tokenDestroy(token);

  // <expr>
  // TODO semantic actions
  ret = pExpr();
  CondReturn;
  
  // <retNextArg>
  ret = pRetNextArg();
  CondReturn;

  vypluj 0;
}


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
int pTypeList() {
  printf("-----------------------------------------------------------\n");
  printf("TYPE LIST\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn

  printToken(token);

  if(token->type == t_idOrKeyword) {

    if( (strcmp(token->data, "nil") == 0) && isDataType(token->data)) {
      //tokenDestroy(token);
      ret = pNextType();
      CondReturn

      vypluj 0;
    }
  }

  vypluj err(1);
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
  printf("NEXT TOKEN\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn

  printToken(token);

  if(token->type == t_comma) {

    ret = pType();

    if(ret == 0) {
      ret = pNextType();
      CondReturn

      vypluj 0;
    }
  }

  vypluj err(1);
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
  CondReturn

  printToken(token);

  if((strcmp(token->data, "nil") == 0) && isDataType(token->data)) {
    vypluj 0;
  }
  vypluj err(1);
}


/**
 * @brief
 *
 * @return error code
 *
 * 58. <idList>          -> [id] <nextId>
 */
int pIdList() {
  printf("-----------------------------------------------------------\n");
  printf(" ID LIST\n");
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn

  printToken(token);

  if(token->type == t_idOrKeyword) {

    if(STGetIsVariable(symtab, token->data)) {
      //tokenDestroy(token);
      ret = pNextId();
      CondReturn
      vypluj 0;
    }
  }

  vypluj err(1);
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
  CondReturn

  printToken(token);

  if(token->type == t_comma) {
    //tokenDestroy(token);
    ret = pIdList();
    CondReturn
    vypluj 0;
  }

  vypluj err(1);
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
  CondReturn

  printToken(token);

  // If the next token is not a ==, use rule 62 (else rule 63)
  if(token->type != t_assignment){
    stashToken(token);
    vypluj 0;
  }

  // '='
  //tokenDestroy(token);

  // <exprList>
  ret = pExprList();
  CondReturn

  vypluj 0;
}


/**
 * @brief
 *
 * @return error code
 *
 * 65. <exprList>        -> <expr> <nextExpr>
 */
int pExprList() {
  printToken("EXPR LIST \n");
  // <expr>
  // TODO semantic actions
  ret = pExpr();
  CondReturn

  // <nextExpr>
  ret = pNextExpr();
  CondReturn

  vypluj 0;
}


/**
 * @brief
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
  CondReturn

  printToken(token);

  // If the next token is not a comma, use rule 66 (else rule 67)
  if(token->type != t_comma){
    stashToken(token);
    vypluj 0;
  }

  // ','
  //tokenDestroy(token);

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
  printf("-----------------------------------------------------------\n");
  printf("EXPR\n");
  Token *token = NULL;

  while(true) {
    ret = scanner(&token);
    CondReturn;

    printToken(token);

    if(isExpressionParser(*token) == false) {
      stashToken(token);
      //tokenDestroy(token);
      return 0;
    }
  }



  // TODO BOTTOM UP PARSER call
  // semantic actions here?

  vypluj 0;
}


#endif

/* end of file parser.h */