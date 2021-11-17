/*
 * Parser
 */

#ifndef PARSER
#define PARSER

#include "misc.h"
#include "precedence_analysis.h"
#include "scanner.h"
#include "symtable.h"

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
 * @brief Rule for require
 *
 * 01. <start>           -> require <req> <codeBody>
 * @vypluj int - 0 if success, else on error
 */
int pStart() {
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn;

  if (token->type == t_idOrKeyword && strcmp(token->data, "require") == 0) {
    tokenDestroy(token);
    ret = pReq();
    CondReturn;
    ret = pCodeBody();
    CondReturn;
    vypluj 0;
  } else {
    tokenDestroy(token);
    vypluj err(1); // TODO aky kod?
  }
  vypluj 0;
}

int pReq() {
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn;

  if (token->type == t_str && strcmp(token->data, "ifj21") == 0) {
    tokenDestroy(token);
    vypluj 0;
  } else {
    tokenDestroy(token);
    vypluj err(1); // TODO aky kod?
  }
}

/**
 * @brief Rule for <codebody> string
 *
 * 04. <codeBody>  -> eps
 * 05. <codeBody>  -> function [id] ( <fnArgList> ) <fnRet> <stat> <ret> end <codeBody>
 * 06. <codeBody>  -> global [id] : function ( <typeList> ) <fnRet> <codeBody>
 * 07. <codeBody>  -> [id] <fnCall> <codeBody>
 * @vypluj int - 0 if success, else on error
 */
int pCodeBody() {

  Token *token = NULL;
  ret = scanner(&token);
  CondReturn;

  // -> eps
  if (token == NULL) {

    vypluj 0;
  } else if (token->type == t_idOrKeyword) {

    //-> function [id] ( <fnArgList> ) <fnRet> <stat> <ret> end <codeBody>
    if (strcmp(token->data, "function") == 0) {

      tokenDestroy(token);
      ret = scanner(&token);
      CondReturn;

      // [id] - function name
      if (token->type == t_idOrKeyword) {
        if (STFind(symtab, token->data) && STGetFnDefined(symtab, token->data)) {
          vypluj err(1); // TODO RETURN ERROR
        } else {
          STInsert(symtab, token->data);
        }
      } else {
        vypluj err(1); //TODO ERROR
      }

      // (
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_leftParen) {
        tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      tokenDestroy(token);

      // <fnArgList>
      ret = pFnArgList();
      CondReturn;

      // )
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_rightParen) {
        tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      tokenDestroy(token);

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
        tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      tokenDestroy(token);

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
      tokenDestroy(token);

      // :
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_colon) {
        tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      tokenDestroy(token);

      // function
      ret = scanner(&token);
      CondReturn;

      if (!(token->type == t_idOrKeyword && strcmp(token->data, "function") == 0)) {
        tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      tokenDestroy(token);

      // (
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_leftParen) {
        tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      tokenDestroy(token);

      // <typeList>
      ret = pTypeList();
      CondReturn;

      // )
      ret = scanner(&token);
      CondReturn;

      if (token->type != t_rightParen) {
        tokenDestroy(token);
        vypluj err(1); // TODO what code
      }
      tokenDestroy(token);

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
        vypluj err(1); // TODO RETURN ERROR
      }

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

int pFnCall() {
  Token *token = NULL;

  // (
  ret = scanner(&token);
  CondReturn;

  if (token->type != t_leftParen) {
    tokenDestroy(token);
    vypluj err(1); // TODO what code
  }
  tokenDestroy(token);

  ret = pFnCallArgList();
  CondReturn;

  // )
  ret = scanner(&token);
  CondReturn;

  if (token->type != t_rightParen) {
    tokenDestroy(token);
    vypluj err(1); // TODO what code
  }
  tokenDestroy(token);

  vypluj 0;
}

int pFnRet() {
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn;

  if(token->type != t_colon){
    stashToken(token);
    return 0;
  }else{
    // : processed by the previous if
    tokenDestroy(token);

    // type
    ret = pType();
    CondReturn;
    
    // nextType
    ret = pNextType();
    CondReturn;
  }
  vypluj 0;
}

int pFnCallArgList() {
  vypluj 0;
}

int pNextFnCallArg() {
  vypluj 0;
}

int pFnCallArg() {
  vypluj 0;
}

int pRet() {
  vypluj 0;
}

int pStat() {
  vypluj 0;
}

int pStatWithId() {
  vypluj 0;
}

int pNextAssign() {
  vypluj 0;
}

int pFnArgList() {
  Token *token = NULL;

  ret = scanner(&token);
  CondReturn;

  if (token->type == t_rightParen) {

    stashToken(token);
    tokenDestroy(token);
    vypluj 0; //TODO
  } else if (token->type == t_idOrKeyword) {
    tokenDestroy(token);

    ret = scanner(&token);
    CondReturn;

    if (STGetIsVariable(symtab, token->data) != false || !STGetFnDefined(symtab, token->data)) {
      vypluj err(1); // TODO RETURN ERROR
    }

    tokenDestroy(token);

    ret = scanner(&token);
    CondReturn;

    if (token->type != t_colon) {
      vypluj err(1);
    }

    tokenDestroy(token);

    ret = scanner(&token);
    CondReturn;

    if (token->type != t_idOrKeyword) {
      //TODO NENI TO KEYWORD
      vypluj err(1);
    }

    if (isDataType(token->data) == 1) {
      //TODO NENI TO datatype
      vypluj err(1);
    }

    tokenDestroy(token);

    pNextFnArg();
    vypluj 0;

  } else {
    vypluj err(1);
    //RETURN ERROR
  }

  vypluj 0;
}

int pNextFnArg() {
  Token *token = NULL;

  scanner(&token);

  if(token->type != t_comma) {
    //TODO ERROR
    vypluj err(1);
  }

  tokenDestroy(token);

  pFnArgList();
  vypluj 0;
}

int pRetArgList() {

  ret = pExpr();

  vypluj 0;
}

int pRetNextArg() {
  vypluj 0;
}

int pTypeList() {
  vypluj 0;
}

int pNextType() {
  vypluj 0;
}

int pType() {
  vypluj 0;
}

int pIdList() {
  vypluj 0;
}

int pNextId() {
  vypluj 0;
}

int pNewIdAssign() {
  vypluj 0;
}

int pExprList() {
  vypluj 0;
}

int pNextExpr() {
  vypluj 0;
}

//-----------------------------------
int pExpr() {

  //TODO BOTTOM UP PARSER call

  vypluj 0;
}

// int rule06() {
// Token *token = NULL;
// STElem *stelement = NULL;

// ret = scanner(&token);
// CondReturn;

// if (token->type == t_idOrKeyword) {
// stelement = STFind(symtab, token->data);

// if (stelement) {
// vypluj err(1); // TODO RETURN ERROR
// } else {
// }
// // TODO NECO S ID
// } else {
// vypluj err(1); //TODO ERROR
// }
// }

/**
 * @brief Rule for <fnCallArgList>
 *
 * <fnCall>    -> ( <fnCallArgList> )
 * @vypluj int - 0 if success, else on error
 */
// int rule07(Token *token) {

// Token *leftParen = NULL;
// Token *rightParen = NULL;
// Token *fnArgList = NULL;
// }

// int rule09() {
// //vedro
// Token *token = NULL;

// ret = scanner(&token);
// CondReturn;

// if (token->type != t_leftParen) {
// tokenDestroy(token);
// vypluj err(1); // TODO what code
// }

// ret = scanner(&token);
// CondReturn;

// if (token->type == t_rightParen) {
// // TODO call nejakej funkcie
// vypluj 0;
// } else {
// // TODO call nejakej funkcie
// }

// ret = scanner(&token);
// CondReturn;
// if (token->type == t_rightParen) {
// vypluj 0;
// } else {
// vypluj err(1); //TODO code
// }
// }

// int rule11() {
// //mine
// // typelist

// // <type>

// Token *token = NULL;
// ret = scanner(&token);
// CondReturn;

// // token integer
// if (token->type == t_idOrKeyword && strcmp(token->data, "integer") == 0) {
// // TODO call nejakej funkcie
// } else if (token->type == t_idOrKeyword && strcmp(token->data, "number") == 0) {
// // TODO call nejakej funkcie
// } else if (token->type == t_idOrKeyword && strcmp(token->data, "string") == 0) {
// // TODO call nejakej funkcie
// } else if (token->type == t_idOrKeyword && strcmp(token->data, "nil") == 0) {
// // TODO call nejakej funkcie
// } else {
// // TODO
// }
// }

#endif

/* end of file parser.h */
