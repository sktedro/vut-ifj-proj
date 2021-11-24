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




/*
 * TODO
 * we'll need to return (receive it returned by code gen??) a variable name (in
 * ifjcode) where the expression result is stored by the precedence analysis
 * TODO
 *
 * Pro chybné kombinace datových typů ve výrazech vracejte chybu 6.
 */




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

/**
 * @brief Destroys (frees data allocated by) a symbol of the symbol stack
 *
 * @param elem - symbol to be destroyed
 */
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
int checkRules(STStack *symtab, SStack *symstack, int opSymbols){
  int rulesRet = -1; // Will be changed to 0 if a rule was found and applied or
  // to > 0 if an error occured
        
  // Get the first symbol from the stack and pop it
  SStackElem *op1 = SStackTop(symstack);
  SStackPop(symstack);

  // If we only have one symbol, no rule function needed as this expression
  // should just be a constant or a variable (without any operator)
  if(opSymbols == 1) {
    // Pop the '<'
    SStackPop(symstack);
    // i rule
    rulesRet = iRule(symtab, symstack, op1);
    if(rulesRet != -1){
      return rulesRet;
    }
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
      rulesRet = strLenRule(symtab, symstack, op1, op2);
      if(rulesRet != -1){
        return rulesRet;
      }

    // Binary operator
    } else if (opSymbols == 3) {
      // Get the third symbol and pop the '<'
      SStackElem *op3 = SStackTop(symstack);
      SStackPop(symstack);
      SStackPop(symstack);

      // (i) or (E)
      rulesRet = bracketsRule(symtab, symstack, op1, op2, op3);
      if(rulesRet != -1){
        return rulesRet;
      }
      rulesRet = aritmeticOperatorsRule(symtab, symstack, op1, op2, op3);
      if(rulesRet != -1){
        return rulesRet;
      }
      rulesRet = relationalOperatorsRule(symtab, symstack, op1, op2, op3);
      if(rulesRet != -1){
        return rulesRet;
      }

      // TODO Call rule functions of binary operators

    }
  }
  return 0;
}

// i
int iRule(STStack *symtab, SStack *symstack, SStackElem *op) {

  // TODO ak nevhodné pravidlo -> vypluj -1; (nie je err len prejdeme na ďalšiu funkciu)
  // TODO check if the symbol is a literal or a variable
  // TODO Generate code

  vypluj 0;
}


// #i or #E
int strLenRule(STStack *symtab, SStack *symstack, SStackElem *op1, SStackElem *op2){
  // Rule of # i/E (string length)
  if(op1->type == st_operator && op1->op == pt_strlen){
    // If the op2 is a variable or a literal
    if(op2->type == st_idOrLiteral){
      // If it is a variable
      if(op2->isId == true){
        // Check if the data type is a string
        if(op2->dataType == dt_string){

          // TODO code gen: STRLEN
          
        }else{
          // Calling strlen on a non string data type
          vypluj err(1); // TODO errcode
        }

      // If it is a literal
      }else {
        if(op2->dataType == dt_string){

          // TODO code gen: STRLEN

        }else{
          // Calling strlen on a non string data type
          vypluj err(1); // TODO errcode
        }
        
      }
    // If the op2 is an expression
    }else if(op2->type == st_expr){
      if(op2->dataType == dt_string){

        // TODO code gen: STRLEN

      }else{
        // Calling strlen on a non string data type
        vypluj err(1); // TODO errcode
      }

    // We received a '#' operator but no literal, ID or expression following it
    }else{
      return err(1); // TODO errcode
    }

    // Push a new element to symstack (E)
    SStackElem *newElem = allocateSymbol(st_expr);
    newElem->op = pt_id;
    newElem->isId = true;
    newElem->dataType = dt_integer;
    // TODO newElem->data = new variable name (where the strlen result is
    // stored in ifjcode) - guess we should get this from the code generator
    SStackPush(symstack, newElem);

  }else{
    // No rule
    return -1;
  }

  destroySymbol(&op1);
  destroySymbol(&op2);
  return 0;
}

// (E) or (i)?
int bracketsRule(STStack *symtab, SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) {
  if(op1->type == st_operator && op3->type == st_operator
      && op1->op == pt_lParen && op3->op == pt_rParen){

    // If it was (E), just push it back
    if(op2->type == st_idOrLiteral){
      SStackPush(symstack, op2);
    // If it was (i), change i to E and push it
    }else if(op2->type == st_expr){
      op2->type = st_expr;
      SStackPush(symstack, op2);
    }else{
      // No rule
      return -1;
    }
    // Destroy the parentheses
    destroySymbol(&op1);
    destroySymbol(&op3);

  }else{
    // No rule
    return -1;
  }

  vypluj 0;
}


// i OPERATOR i or E OPERATOR E ?
// "+ - * / // .."
int aritmeticOperatorsRule(STStack *symtab, SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) {

  // op2 needs to be st_operator and op == pt_relOp
  // op1 and op3 both need to be i or E
  if(op2->type == st_operator 
      && (op2->op == pt_mult || op2->op == pt_div || op2->op == pt_intDiv
        || op2->op == pt_add || op2->op == pt_sub || op2->op == pt_concat)
      && (op1->type == st_idOrLiteral || op1->type == st_expr)
      && (op3->type == st_idOrLiteral || op3->type == st_expr)){

    // TODO dalenie nulou - error DIV_BY_ZERO_ERR
    
    // Rule of i/E .. (string concatenation)
    /**
      * if(op2->type == st_operator && op2->op == pt_concat){
      *   if(op1->type == st_idOrLiteral){
      *   }else if(op1->type == st_expr){
      *   }
      * }
      */
  }else{
    // No rule
    return -1;
  }

  vypluj 0;
}


// i RELOP i or E RELOP E ?
int relationalOperatorsRule(STStack *symtab, SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) {

  
  // op2 needs to be st_operator and op == pt_relOp
  // op1 and op3 both need to be i or E
  if(op2->type == st_operator && op2->op == pt_relOp
      && (op1->type == st_idOrLiteral || op1->type == st_expr)
      && (op3->type == st_idOrLiteral || op3->type == st_expr)){


    // If all or one of the operands is a nil
    if(op1->dataType == dt_nil || op3->dataType == dt_nil){
      // If the op2 is not "==" and neither "~=" we throw an error, since nil
      // can only be compared by these two operators
      if(strcmp(op2->data, "==") && strcmp(op2->data, "~=")){
        return err(NIL_ERR);
      }
    // If none of the operands are a nil
    }else{
      // If the data types of op1 and op3 don't match
      if(op1->dataType != op3->dataType){

        // If we have an integer and a number -> convert the int to num
        if(op1->dataType == dt_integer && op3->dataType == dt_number){
          // TODO code gen op1 to num

        }else if(op3->dataType == dt_integer && op1->dataType == dt_number){
          // TODO code gen op3 to num

        // Else, throw an error
        else{
          return err(TYPE_EXPR_ERR);
        }
      }

    }

    }

    // TODO code gen push operands to stack
    
    if(!strcmp(op2->data, "<") || !strcmp(op2->data, ">=")){
      // TODO code gen LT
      
    }else if(!strcmp(op2->data, ">") || !strcmp(op2->data, "<=")){
      // TODO code gen GT
      
    }else if(!strcmp(op2->data, "==") || !strcmp(op2->data, "~=")){
      // TODO code gen EQ
      
    }else{
      fprintf(stderr, "This is awkward. The operator should be relational.\n");
      return err(1); // todo? This should not be possible
    }

    // Generate code for NOT - to negate the result of the expression
    // eg. if we have '~=', we'll generate code for '==' but need to negate it,
    // for '<=', we'll generate '>',...
    if(!strcmp(op2->data, "<=") || !strcmp(op2->data, ">=") || !strcmp(op2->data, "~=")){
      // TODO code gen NOTS
    }


  }else{
    // No rule
    return -1;
  }

  vypluj 0;
}



/**
 * This is called from the parser TODO
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
        ret = checkRules(symtab, symstack, opSymbols);

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
