/*
 * Precedence analysis
 */

#ifndef PRECEDENCE_ANALYSIS_C
#define PRECEDENCE_ANALYSIS_C

#include "precedence_analysis.h"

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

/*
* Rule functions
*/

//TO DO voláme funkcie za radom, ak neakceptujem pravidlo dám vedieť že ho nemám

int strLenRule(SStack *stack, STStack *tableStack) { // #

  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)

  vypluj 0;
}

int aritmeticOperatorsRule(SStack *stack, STStack *tableStack) { // + - * / //

  // TODO dalenie nulou - error DIV_BY_ZERO_ERR
  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)

  vypluj 0;
}

int concatenanceRule(SStack *stack, STStack *tableStack) { // ..

  // TODO zistiť či pracujeme len so stringami, inak nemôžeme robiť operáciu
  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}

<<<<<<< Updated upstream
int relationalOperatorsRule(SStack *stack, STStack *tableStack){ // relačné operátory (vynímky)
  
  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}

int iRule(SStack *stack, STStack *tableStack){ // i
  
  // TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
=======
int relationalOperatorsRule(SStack *stack, STStack *tableStack) { // relačné operátory (vynímky)

  //TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}

int iRule(SStack *stack, STStack *tableStack) { // i

  //TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
>>>>>>> Stashed changes
  vypluj 0;
}

int bracketsRule(SStack *stack, STStack *tableStack) { // ()

  //TODO ak nevhodné pravidlo -> vypluj 1; (nie je err len prejdeme na ďalšiu funkciu)
  vypluj 0;
}

/**
 * This is called from the parser
 */
int parseExpression(STStack *symtable, Token *token) {
  precedenceAnalysis(symtable, token);
  return 0;
}

/**
 * @brief Receives a token and return a new element of the symbol stack
 * 
 * @param token to be converted to SStackElem
 * @return SStackElem* converted from token
 */
SStackElem *parseToken(Token *token) {
  // Allocate and initialize new symbol stack element
<<<<<<< Updated upstream
  SStackElem *newElem = (SStackElem*) malloc(sizeof(SStackElem));
  if(newElem == NULL) {
    exit(err(INTERN_ERR));
  }
=======
  SStackElem *newElem = (SStackElem *)malloc(sizeof(SStackElem));
>>>>>>> Stashed changes
  newElem->type = newElem->op = -1;
  newElem->isId = false;
  newElem->data = NULL;
  newElem->next = NULL;

<<<<<<< Updated upstream

  switch (token->type){
    case t_idOrKeyword:
      newElem->type = st_idOrLiteral;
      newElem->isId = true;
      newElem->op = pt_id;
      newElem->data = malloc(sizeof(char) * (strlen(token->data) + 1));
      memcpy(newElem->data, token->data, strlen(token->data) + 1);
      break;
    case t_int:
    case t_num:
    case t_sciNum:
    case t_str:
      newElem->type = st_idOrLiteral;
      newElem->op = pt_id;
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
      newElem->type = st_op;
      newElem->op = pt_rParen;
      break;
    case t_leftParen:
      newElem->type = st_op;
      newElem->op = pt_lParen;
      break;
    case t_arithmOp:
    case t_strOp:
      newElem->type = st_op;
      if(strcmp(token->data, "+") == 0){
        newElem->op = pt_add;
      }else if(strcmp(token->data, "-") == 0){
        newElem->op = pt_sub;
      }else if(strcmp(token->data, "#") == 0){
        newElem->op = pt_strlen;
      }else if(strcmp(token->data, "*") == 0){
        newElem->op = pt_mult;
      }else if(strcmp(token->data, "/") == 0){
        newElem->op = pt_div;
      }else if(strcmp(token->data, "//") == 0){
        newElem->op = pt_intDiv;
      }else if(strcmp(token->data, "..") == 0){
        newElem->op = pt_concat;
      }
      break;
    case t_relOp:
      newElem->type = st_op;
      newElem->op = pt_relOp;
      newElem->data = malloc(sizeof(char) * (strlen(token->data) + 1));
      memcpy(newElem->data, token->data, strlen(token->data) + 1);
      break;
=======
  switch (token->type) {
  case t_idOrKeyword:
    newElem->type = st_idOrLiteral;
    newElem->isId = true;
    newElem->op = pt_id;
    newElem->data = malloc(sizeof(char) * (strlen(token->data) + 1));
    memcpy(newElem->data, token->data, strlen(token->data) + 1);
    break;
  case t_int:
  case t_num:
  case t_sciNum:
  case t_str:
    newElem->type = st_idOrLiteral;
    newElem->op = pt_id;
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
    newElem->type = st_op;
    newElem->op = pt_rParen;
    break;
  case t_leftParen:
    newElem->type = st_op;
    newElem->op = pt_lParen;
    break;
  case t_arithmOp:
  case t_strOp:
    newElem->type = st_op;
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
    newElem->type = st_op;
    newElem->op = pt_relOp;
    newElem->data = malloc(sizeof(char) * (strlen(token->data) + 1));
    memcpy(newElem->data, token->data, strlen(token->data) + 1);
    break;
>>>>>>> Stashed changes
  }

  return newElem;
}

SStackElem *allocateSymbol(int st_symbol) {
  SStackElem *newElem = malloc(sizeof(SStackElem));
  if (!newElem) {
    vypluj NULL;
  }
  newElem->type = st_symbol;
  newElem->op = -1;
  newElem->isId = false;
  newElem->data = NULL;
  newElem->next = NULL;
  return newElem;
}

/**
 * The precedence analysis algorithm
 */
int precedenceAnalysis(STStack *symtable, Token *token) {
  SStack *symstack = SStackInit();


  // Push a $ to the stack
  ret = SStackPush(symstack, allocateSymbol(st_dollar));
  CondReturn;

  bool exprEnd = false;

  while (1) {

    // Check if the next token is a part of the expression
    if (!exprEnd) {
      // TODO par dalsich podmienok asi. Ocheckovat keywordy a take
<<<<<<< Updated upstream
      if(token->type == t_colon 
          || token->type == t_comma 
          || token->type == t_assignment) {
=======
      if (token->type == t_colon || token->type == t_comma || token->type == t_assignment) {
>>>>>>> Stashed changes
        stashToken(token);
        exprEnd = true;
        token = NULL;
      }
    }

    SStackElem *topSymbol = SStackTopTerminal(symstack);
    SStackElem *inputSymbol = parseToken(token);

<<<<<<< Updated upstream
    if(precTab[topSymbol->op][inputSymbol->op] == st_nop) {
=======
    if (precTab[topSymbol->op][inputSymbol->op] == st_nop) {
>>>>>>> Stashed changes
      // Push the input symbol to the stack
      SStackPush(symstack, inputSymbol);
      // Destroy the old token
      tokenDestroy(token);
      // Get a new token
      ret = scanner(&token);
      condVypluj

<<<<<<< Updated upstream
    } else if(precTab[topSymbol->op][inputSymbol->op] == st_push) {
=======
    } else if (precTab[topSymbol->op][inputSymbol->op] == st_push) {
>>>>>>> Stashed changes
      // Allocate a new symbol ('<') and push it after the top terminal
      SStackPushAfterTopTerminal(symstack, allocateSymbol(st_push));
      // Push the input symbol
      SStackPush(symstack, inputSymbol);
      // Destroy the old token
      tokenDestroy(token);
      // Get a new token
      ret = scanner(&token);
      condVypluj

<<<<<<< Updated upstream
    } else if(precTab[topSymbol->op][inputSymbol->op] == st_reduce){
=======
    } else if (precTab[topSymbol->op][inputSymbol->op] == st_reduce) {
>>>>>>> Stashed changes
      // TODO toto asi ne: // Check if '<' 'y' is on top of the stack

      // Call rule functions - if one of them has a rule that reduces the
      // expression, it returns 0 and we're done reducing for now

      // None of the rule functions returned 0 - there's no rule able to reduce
      // this expression, which means that there is an error

<<<<<<< Updated upstream

    } else {
      vypluj err(SYNTAX_ERR);
=======
    } else {
      vypluj err(1); // TODO errcode
>>>>>>> Stashed changes
    }
  }

  vypluj 0;
}

#endif
/* end of file precedence_analysis.h*/