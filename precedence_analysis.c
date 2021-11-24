/*
 * Precedence analysis
 */

#ifndef PRECEDENCE_ANALYSIS_C
#define PRECEDENCE_ANALYSIS_C

#include "precedence_analysis.h"
#include "symtable.h"
#include "parser.h"

extern int ret;

// Precedence table
// Could be simpler since rows (columns) repeat
char precTab[12][12] = {
  //#   *   /   //  +   -   .. rel  (   )   i   $
  {'_', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // pt_strlen
  {'<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // pt_mult
  {'<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // pt_div
  {'<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // pt_intDiv
  {'<', '<', '<', '<', '>', '>', '>', '>', '<', '>', '<', '>'}, // pt_add
  {'<', '<', '<', '<', '>', '>', '>', '>', '<', '>', '<', '>'}, // pt_sub
  {'<', '<', '<', '<', '<', '<', '<', '>', '<', '>', '<', '>'}, // pt_concat
  {'<', '<', '<', '<', '<', '<', '<', '>', '<', '>', '<', '>'}, // pt_relOp
  {'<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '_'}, // pt_lParen
  {'>', '>', '>', '>', '>', '>', '>', '>', '_', '>', '_', '>'}, // pt_rParen
  {'>', '>', '>', '>', '>', '>', '>', '>', '_', '>', '_', '>'}, // pt_id
  {'<', '<', '<', '<', '<', '<', '<', '<', '<', '_', '<', '_'}  // pt_dollar
};


/**
 * @brief Allocates a new symbol (to be inserted to the symbol stack)
 *
 * @param symbol: enumeration of symbol stack type 
 *
 * @return SStackElem as a new, allocated symbol stack element
 */
SStackElem *allocateSymbol(int symbol) {
  SStackElem *newElem = malloc(sizeof(SStackElem));
  if (!newElem) {
    exit(err(INTERN_ERR));
    vypluj NULL;
  }
  newElem->type = symbol;
  newElem->op = -1;
  newElem->isId = false;
  newElem->dataType = -1;
  newElem->data = NULL;
  newElem->next = NULL;
  return newElem;
}

void destroySymbol(SStackElem **elem){
  free((*elem)->data);
  free(*elem);
  *elem = NULL;
}


/*
 * Rule functions
 */


/**
 * Call rule functions based on how many symbols we are reducing
 */
int checkRules(STStack *symtab, SStack *symstack, int opSymbols, int *rulesRet){
  // Get the first symbol from the stack and pop it
  SStackElem *op1 = SStackTop(symstack);
  SStackPop(symstack);

  // If we only have one symbol, no rule function needed as this expression
  // should just be a constant or a variable (without any operator)
  if(opSymbols == 1) {
    // Pop the '<'
    SStackPop(symstack);

    // TODO check if the symbol is a literal or a variable
    // TODO Generate code

  // We have a unary or a binary operator:
  } else {
    // Get the second symbol and pop it
    SStackElem *op2 = SStackTop(symstack);
    SStackPop(symstack);

    // Unary operator
    if (opSymbols == 2) {
      // Pop the '<'
      SStackPop(symstack);

      // Call rule functions of unary operators
      *rulesRet = strLenRule(symtab, symstack, op1, op2);
      if(*rulesRet){
        return 0;
      }

    // Binary operator
    } else if (opSymbols == 3) {
      // Get the third symbol and pop the '<'
      SStackElem *op3 = SStackTop(symstack);
      SStackPop(symstack);
      SStackPop(symstack);

      // TODO Call rule functions of binary operators

    }
  }
  return 0;
}

int iRule(SStack *stack, STStack *symtab) { // i

  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}


int strLenRule(STStack *symtab, SStack *symstack, SStackElem *op1, SStackElem *op2){
  // Rule of # i/E (string length)
  if(op1->type == st_operator && op1->op == pt_strlen){
    // If the op2 is a variable or a literal
    if(op2->type == st_idOrLiteral){
      // If it is a variable
      if(op2->isId == true){
        // Check if the data type is a string
        if(op2->dataType == dt_string){

          // TODO generate code calling STRLEN
          
        }else{
          // Calling strlen on a non string data type
          vypluj err(1); // TODO errcode
        }

      // If it is a literal
      }else {
        if(op2->dataType == dt_string){

          // TODO generate code calling STRLEN

        }else{
          // Calling strlen on a non string data type
          vypluj err(1); // TODO errcode
        }
        
      }
    // If the op2 is an expression
    }else if(op2->type == st_expr){
      if(op2->dataType == dt_string){

        // TODO generate code calling STRLEN

      }else{
        // Calling strlen on a non string data type
        vypluj err(1); // TODO errcode
      }

    }else{
      // TODO err??
    }

    // Push a new element to symstack (E)
    SStackElem *newElem = allocateSymbol(st_expr);
    // TODO newElem->op = ? netreba??
    // TODO newElem->isId = ?? je to ID alebo nie? Asi jo
    newElem->dataType = dt_integer;
    // TODO newElem->data = new variable name (where the strlen result is
    // stored in ifjcode
    SStackPush(symstack, newElem);

  }else{
    // no rule
  }

  destroySymbol(&op1);
  destroySymbol(&op2);
  return 0;
}

// (E) or (i)?
int bracketsRule(SStack *stack, STStack *symtab) {

  //TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}


// i OPERATOR i or E OPERATOR E ?
// "+ - * / //"
int aritmeticOperatorsRule(SStack *stack, STStack *symtab) {

  // TODO dalenie nulou - error DIV_BY_ZERO_ERR
  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)

  vypluj 0;
}


// ".."
int concatenanceRule(SStack *stack, STStack *symtab) {

  // Rule of i/E .. (string concatenation)
  /**
    * if(op2->type == st_operator && op2->op == pt_concat){
    *   if(op1->type == st_idOrLiteral){
    *   }else if(op1->type == st_expr){
    *   }
    * }
    */
  // TODO zistiť či pracujeme len so stringami, inak nemôžeme robiť operáciu
  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}

// i RELOP i or E RELOP E ?
int relationalOperatorsRule(SStack *stack, STStack *symtab) {

  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}


/**
 * This is called from the parser
 */
int parseExpression(STStack *symtab, Token *token) {
  precedenceAnalysis(symtab, token);
  return 0;
}

/**
 * @brief Receives a token and return a new element of the symbol stack
 *
 * @param token to be converted to SStackElem
 * @return SStackElem* converted from token
 */
SStackElem *parseToken(STStack *symtab, Token *token) {
  // Allocate and initialize new symbol stack element
  SStackElem *newElem = allocateSymbol(-1);

  switch (token->type) {
    case t_idOrKeyword:
      // Check if it is a keyword - if it is, that's bad
      // If the token is not a nil literal, check if it is a keyword
      // (it can be a nil (value), but nil is also a keyword)
      if(strcmp(token->data, "nil")){
        if(isKeyword(token)){
          // The token is a keyword (not a nil)
          exit(err(1)); // TODO errcode
          return NULL;
        }
      }
      // Check it the ID exists (we now know it is an ID)
      if(!STFind(symtab, token->data)){
        // THE ID DOES NOT EXIST
        exit(err(1)); // TODO errcode
        return NULL;
      }
      newElem->type = st_idOrLiteral;
      newElem->isId = true;
      newElem->op = pt_id;
      // Read the data type from the symbol table
      newElem->dataType = STGetVarDataType(symtab, token->data);
      newElem->data = malloc(sizeof(char) * (strlen(token->data) + 1));
      memcpy(newElem->data, token->data, strlen(token->data) + 1);
      break;

    case t_int:
    case t_num:
    case t_sciNum:
    case t_str:
      newElem->type = st_idOrLiteral;
      newElem->op = pt_id;
      if(token->type == t_int){
        newElem->dataType = dt_integer;
      }else if(token->type == t_num || token->type == t_sciNum){
        newElem->dataType = dt_number;
      }else if(token->type == t_str){
        newElem->dataType = dt_string;
      }
      newElem->data = malloc(sizeof(char) * (strlen(token->data) + 1));
      memcpy(newElem->data, token->data, strlen(token->data) + 1);
      break;

    case t_colon:
    case t_comma:
    case t_assignment:
      // TODO stash token?
      free(newElem);
      return NULL;
      break;

    case t_rightParen:
      newElem->type = st_operator;
      newElem->op = pt_rParen;
      break;

    case t_leftParen:
      newElem->type = st_operator;
      newElem->op = pt_lParen;
      break;

    case t_arithmOp:
    case t_strOp:
      newElem->type = st_operator;
      if (strcmp(token->data, "+") == 0) {
        newElem->op = pt_add;
      } else if (strcmp(token->data, "-") == 0) {
        newElem->op = pt_sub;
      } else if (strcmp(token->data, "#") == 0) {
        newElem->op = pt_strlen;
      } else if (strcmp(token->data, "*") == 0) {
        newElem->op = pt_mult;
      } else if (strcmp(token->data, "/") == 0) {
        newElem->op = pt_div;
      } else if (strcmp(token->data, "//") == 0) {
        newElem->op = pt_intDiv;
      } else if (strcmp(token->data, "..") == 0) {
        newElem->op = pt_concat;
      }
      break;

    case t_relOp:
      newElem->type = st_operator;
      newElem->op = pt_relOp;
      newElem->data = malloc(sizeof(char) * (strlen(token->data) + 1));
      memcpy(newElem->data, token->data, strlen(token->data) + 1);
      break;
  }

  return newElem;
}

/**
 * The precedence analysis algorithm
 */
int precedenceAnalysis(STStack *symtab, Token *token) {
  SStack *symstack = SStackInit();

  // Push a $ to the stack
  ret = SStackPush(symstack, allocateSymbol(st_dollar));
  CondReturn;

  bool exprEnd = false;

  while (1) {

    // Check if the next token is a part of the expression
    if (!exprEnd) {
      // TODO par dalsich podmienok asi. Ocheckovat keywordy a take
      if (token->type == t_colon
          || token->type == t_comma
          || token->type == t_assignment) {
        stashToken(token);
        exprEnd = true;
        token = NULL;
      }
    }

    SStackElem *topSymbol = SStackTopTerminal(symstack);
    SStackElem *inputSymbol = parseToken(symtab, token);

    if (precTab[topSymbol->op][inputSymbol->op] == st_nop) {
      // Push the input symbol to the stack
      SStackPush(symstack, inputSymbol);
      // Destroy the old token
      tokenDestroy(&token);
      // Get a new token
      ret = scanner(&token);
      condVypluj

    } else if (precTab[topSymbol->op][inputSymbol->op] == st_push) {
      // Allocate a new symbol ('<') and push it after the top terminal
      SStackPushAfterTopTerminal(symstack, allocateSymbol(st_push));
      // Push the input symbol
      SStackPush(symstack, inputSymbol);
      // Destroy the old token
      tokenDestroy(&token);
      // Get a new token
      ret = scanner(&token);
      condVypluj

    } else if (precTab[topSymbol->op][inputSymbol->op] == st_reduce) {
      // TODO toto asi ne: // Check if '<' 'y' is on top of the stack

      // Call rule functions - if one of them has a rule that reduces the
      // expression, it returns 0 and we're done reducing for now

      // Count symbols that are after '<' on the stack
      int opSymbols = 0;
      SStackElem *tmp = SStackTop(symstack);
      while(tmp && tmp->next){
        // '<' found
        if(tmp->type == st_push){
          // Set opSymbols to a positive value
          opSymbols = - opSymbols;
          break;
        }
        // Keep opSymbols negative and invert the value when '<' is found
        opSymbols--;
        tmp = tmp->next;
      }

      // In case opSymbols is negative / there is no op symbol / there are more
      // than three op symbols -> error
      if(opSymbols < 0 || opSymbols == 0 || opSymbols > 3){
        vypluj err(1); // TODO errcode
      }else{
        int rulesRet;
        ret = checkRules(symtab, symstack, opSymbols, &rulesRet);
      }


      // None of the rule functions returned 0 - there's no rule able to reduce
      // this expression, which means that there is an error

    } else {
      vypluj err(SYNTAX_ERR);
    }
  }

  vypluj 0;
}

#endif
/* end of file precedence_analysis.h*/
