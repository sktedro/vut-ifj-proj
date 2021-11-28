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
    //#    *    /    //   +    -    ..   rel  (    )    i    $    <- input token
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
 *
 * The precedence analysis algorithm
 *
 */

/**
 * @brief A precedence analysis algorithm. Parses an expression, checks for
 * errors and generates code
 *
 * @param symtab: symbol table containing variables and functions
 * @param token: first token of the expression
 * @param returnVarName: pointer to a string where this function writes the 
 * name of the variable where the expression result is stored (in the generated
 * code)
 *
 * @return 0 if successful, errcode otherwise 
 */
int parseExpression(STStack *symtab, Token *token, char **returnVarName) {
  // Init
  SStack *symstack = NULL;
  SStackElem *topSymbol = NULL, *inputSymbol = NULL;
  if (precedenceAnalysisInit(symtab, &symstack, &token) == -1) {
    // The token is a function call (it is not an error)
    return -1;
  }

  // An actual symbol from the precedence table will be saved here
  char precTableSymbol = '\0';

  // Will be checked every cycle of the 'while' - if true, new token is fetched
  // We already have one to process (param)
  bool getNewToken = false;

  // Will be true if the next token we fetch might not be a part of the expr
  // If this is false and we receive a token which cannot be a part of an
  // expression, we have encountered an error
  bool exprCanEnd = true;

  // True if the last scanned token was not a part of the expression. If true,
  // no tokens will be received and only the 'reduce' rule will be used
  bool exprEnd = false;

  /*
   * The algorithm:
   */

  // End when we parsed all tokens of the expr and there is just $E in the stack
  while (!exprEnd || !isExprAtomic(symstack)) {

    // Get the top symbol
    topSymbol = SStackTopTerminal(symstack);

    // Get the input symbol
    CondCall(getNewSymbol, symtab, &token, &inputSymbol, getNewToken, 
        &exprCanEnd, &exprEnd);

    // Get the character from the precedence table
    precTableSymbol = precTab[topSymbol->op][inputSymbol->op];

    // Shift (the top terminal)
    if (precTableSymbol == '=') {
      CondCall(shiftStep, symstack, inputSymbol, &token);
      getNewToken = true;

      // Shift and push '<' (after (above) the top terminal)
    } else if (precTableSymbol == '<') {
      SStackElem *newSymbol = NULL;
      CondCall(allocateSymbol, &newSymbol, st_push);
      CondCall(SStackPushAfterTopTerminal, symstack, newSymbol);
      CondCall(shiftStep, symstack, inputSymbol, &token);
      getNewToken = true;

      // Reduce (the top terminals to an expression)
    } else if (precTableSymbol == '>') {
      CondCall(reduceStep, symstack);
      getNewToken = false;

      // Syntax error
    } else if (precTableSymbol == '_') {
      vypluj err(SYNTAX_ERR);
    }
  }

  // Return the name of the variable where the result of the expression is 
  // stored
  *returnVarName = symstack->top->data;

  vypluj 0;
}

/**
 * @brief Shift step of the precedence analysis - just push the input symbol
 * to the symbol stack
 *
 * @param symstack: symbol stack
 * @param inputSymbol to be pushed to the symbol stack
 * @param token 
 *
 * @returns 0 if successful, errcode otherwise 
 */
int shiftStep(SStack *symstack, SStackElem *inputSymbol, Token **token) {
  // Push the input symbol to the stack
  CondCall(SStackPush, symstack, inputSymbol);
  return 0;
}

/**
 * @brief Reduce step of the precedence analysis - reduce an atomic expression
 * that is on top of the stack - checks if a rule for the expression exists and
 * generates code if it does
 *
 * @param symstack: symbol stack
 *
 * @returns 0 if successful, errcode otherwise 
 */
int reduceStep(SStack *symstack) {
  // Count symbols that are to be reduced (eg. 2 for "#str")
  int opSymbols = 0;
  SStackElem *tmp = SStackTop(symstack);
  while (tmp && tmp->next && tmp->type != st_push) {
    opSymbols++;
    tmp = tmp->next;
  }
  // Call rule functions - if a rule is found, expr will be reduced, code
  // generated and 0 returned. Returns -1 if no rule was found
  return checkRules(symstack, opSymbols);
}

/*
 * Rule functions
 */

/**
 * @brief Check rules for symbols on the stack that need reducing
 *
 * @param symstack: symbol stack
 * @param opSymbols: amount of symbols that are to be reduces (eg. 2 for #str)
 *
 * @return 0 if successful, errcode otherwise 
 */
int checkRules(SStack *symstack, int opSymbols) {
  // Will be 0 if a rule was found and applied, -1 if not, error code otherwise
  int rulesRet = -1;

  // Get the symbols that are to be reduced
  SStackElem *op1 = NULL, *op2 = NULL, *op3 = NULL;
  CondCall(getSymbolsForReduction, symstack, &op1, &op2, &op3, opSymbols);

  // If we only have one symbol, call iRule (which reduces i to E)
  if (opSymbols == 1) {
    TRYRULE(iRule, op1);

    // Unary operator
  } else if (opSymbols == 2) {
    TRYRULE(strLenRule, op2, op1);

    // Binary operator
  } else if (opSymbols == 3) {
    TRYRULE(bracketsRule, op3, op2, op1);
    TRYRULE(arithmeticOperatorsRule, op3, op2, op1);
    TRYRULE(relationalOperatorsRule, op3, op2, op1);
  }

  // If no rule was found and applied -> syntax err
  if (rulesRet == -1) {
    rulesRet = SYNTAX_ERR;
  }

  vypluj rulesRet;
}

/**
 * @brief "i rule" - reduce i to E -just convert the i to E and push it back 
 * to the stack
 *
 * @param symstack: symbol stack
 * @param op: the symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 */
int iRule(SStack *symstack, SStackElem *op) {
  // The symbol needs to be an 'i'
  if (op->type == st_idOrLiteral) {
    op->type = st_expr;
    // If it is literal, save it into a variable and assign it a value
    if (!op->isId) {
      char *newName = genTmpVarDef();
      CondCall(genVarAssign, newName, op->dataType, op->data);
      op->data = newName;
      op->isId = true;
    }
    // And push it back to the stack as E
    CondCall(SStackPush, symstack, op);
    vypluj 0;
  }
  vypluj - 1;
}

/**
 * @brief "#E rule" - reduce "#E" to "E" - generate code for strlen
 *
 * @param symstack: symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 */
int strLenRule(SStack *symstack, SStackElem *op1, SStackElem *op2) {
  char *newSymbolName = NULL;

  if (op1->type == st_operator && op1->op == pt_strlen) {

    // We received a '#' operator but no i after it
    if (op2->type != st_expr) {
      vypluj err(SYNTAX_ERR);
    }

    // The data type of the operand must be a string
    if (op2->dataType != dt_string) {
      vypluj err(TYPE_EXPR_ERR);
    }

    // Generate code
    newSymbolName = genUnaryOperation(op2);

    // Create a new sym stack element (E) and push it
    SStackElem *newSymbol = NULL;
    CondCall(createSymbol, &newSymbol, st_expr, pt_id, true, dt_integer, 
        newSymbolName);
    CondCall(SStackPush, symstack, newSymbol);

  } else {
    // No rule
    return -1;
  }
  return 0;
}

/**
 * @brief "(E) rule" - destroy the parantheses and push the E back
 *
 * @param symstack: symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 * @param op3: third symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 */
int bracketsRule(SStack *symstack, SStackElem *op1,
                 SStackElem *op2, SStackElem *op3) {
  if (op1->op == pt_lParen && op3->op == pt_rParen && op2->type == st_expr) {
    // Push the E back
    CondCall(SStackPush, symstack, op2);

  } else {
    // No rule
    return -1;
  }

  vypluj 0;
}

/**
 * @brief "arithmetic operator rule" - reduces a arithmetic operation - 
 * addition, subtraction, multiplication, division, integer division and 
 * concatenation (+ - * / // ..) - check the data types and everything and 
 * generate code
 *
 * @param symstack - symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 * @param op3: third symbol to be reduced
 *
 * @return 0 if sucessful, -1 if no rule was applied, errcode otherwise
 *
 * pt_concat only works with two strings
 * pt_{mult, add, sub} work with integers and numbers
 * pt_div works with numbers and returns a number
 * pt_intDiv works with integers and returns an integer
 */
int arithmeticOperatorsRule(SStack *symstack, SStackElem *op1,
                            SStackElem *op2, SStackElem *op3) {

  // op2 needs to be an arithmetic operator, op1 and op3 need to be E
  if (isBinArithmOp(op2) && op1->type == st_expr && op3->type == st_expr) {

    // Check the data types
    CondCall(checkDataTypesOfBinOps, op1, op2, op3);

    // Division by zero check (op3 can't be zero)
    if ((op2->op == pt_div || op2->op == pt_intDiv) && op3->isZero) {
      return err(DIV_BY_ZERO_ERR);
    }

    // Result will have the same data type as op1 (and op3)
    int resultDataType = op1->dataType;
    char *newSymbolName = NULL;
    bool newSymbolIsZero = false;

    // Generate the operator code:
    if (op2->op == pt_add) {
      newSymbolName = genBinaryOperationAdd(op1, op3);
      newSymbolIsZero = op1->isZero && op3->isZero;
    } else if (op2->op == pt_sub) {
      newSymbolName = genBinaryOperationSub(op1, op3);
      newSymbolIsZero = op1->isZero && op3->isZero;
    } else if (op2->op == pt_mult) {
      newSymbolName = genBinaryOperationMul(op1, op3);
      newSymbolIsZero = op1->isZero || op3->isZero;
    } else if (op2->op == pt_div) {
      newSymbolName = genBinaryOperationDiv(op1, op3);
      newSymbolIsZero = op1->isZero;
    } else if (op2->op == pt_intDiv) {
      newSymbolName = genBinaryOperationIDiv(op1, op3);
      newSymbolIsZero = op1->isZero;
    } else if (op2->op == pt_concat) {
      newSymbolName = genBinaryOperationConcat(op1, op3);
    } else {
      return err(SYNTAX_ERR);
    }

    // Create a new symbol and push it to the symstack
    SStackElem *newSymbol = NULL;
    CondCall(createSymbol, &newSymbol, st_expr, pt_id, true, resultDataType, 
        newSymbolName);
    newSymbol->isZero = newSymbolIsZero;
    CondCall(SStackPush, symstack, newSymbol);

  } else {
    // No rule
    return -1;
  }

  vypluj 0;
}

/**
 * @brief "relational operator rule" - reducing relational expressions - 
 * == ~= < > <= >= - check the data types and everything and generate code
 *
 * @param symstack - symbol stack
 * @param op1: first symbol to be reduced
 * @param op2: second symbol to be reduced
 * @param op3: third symbol to be reduced
 *
 * @return 0 if successful, -1 if no rule was found, errcode otherwise 
 */
int relationalOperatorsRule(SStack *symstack, SStackElem *op1,
                            SStackElem *op2, SStackElem *op3) {

  // Format needs to be "E relOp E"
  if (op2->op == pt_relOp && op1->type == st_expr && op3->type == st_expr) {

    char *newSymbolName = NULL;

    // If one of the types is a boolean, it is a result of another rel op
    if (op1->dataType == dt_boolean || op3->dataType == dt_boolean) {
      return err(SYNTAX_ERR);
    }

    // If we have an integer and a number -> convert the int to num
    if (op1->dataType == dt_integer && op3->dataType == dt_number) {
      CondCall(ensureNumber, op1);
    } else if (op3->dataType == dt_integer && op1->dataType == dt_number) {
      CondCall(ensureNumber, op3);
    }

    // If an operand is a nil
    if (op1->dataType == dt_nil || op3->dataType == dt_nil) {
      // We can only compare by '==' or '~='
      if (!strEq(op2->data, "==") && !strEq(op2->data, "~=")) {
        return err(NIL_ERR);
      }

      // If no operand is a nil
    } else {
      // The data types of operands must match now (we made sure they do)
      if (op1->dataType != op3->dataType) {
        return err(TYPE_EXPR_ERR);
      }
    }

    // Generate code
    if (strEq(op2->data, "<") || strEq(op2->data, ">=")) {
      newSymbolName = genLower(op1, op3);
    } else if (strEq(op2->data, ">") || strEq(op2->data, "<=")) {
      newSymbolName = genGreater(op1, op3);
    } else if (strEq(op2->data, "==") || strEq(op2->data, "~=")) {
      newSymbolName = genEqual(op1, op3);
    }

    // Create a new symbol
    SStackElem *newSymbol = NULL;
    CondCall(createSymbol, &newSymbol, st_expr, pt_id, true, dt_boolean, 
        newSymbolName);

    // Generate code for NOT (negate the result of the expression)
    // eg. for '<=', we'll generate '>' and negate it
    if (strEq(op2->data, "<=") 
        || strEq(op2->data, ">=") 
        || strEq(op2->data, "~=")) {
      newSymbol->data = genNot(newSymbol);
    }

    // Push the new symbol to the symstack (E)
    CondCall(SStackPush, symstack, newSymbol);

    return 0;
  } else {
    // No rule
    return -1;
  }
}

/*
 * Rule helper functions
 */

/**
 * @brief Pop 'amount' of symbols from the stack and pass it down by
 * parameters. Also pop the '<' beneath them
 *
 * @param symstack: symbol stack
 * @param op1: first operand
 * @param op2: operator
 * @param op3: second operand
 * 
 * @return 0 if successful, errcode otherwise
 */
int getSymbolsForReduction(SStack *symstack, SStackElem **op1,
                           SStackElem **op2, SStackElem **op3, int amount) {

  if (amount == 1) {
    // Get the first symbol from the stack (and pop it)
    *op1 = SStackPop(symstack);
  } else if (amount == 2) {
    // Get first two symbols
    *op1 = SStackPop(symstack);
    *op2 = SStackPop(symstack);
  } else if (amount == 3) {
    // Get three symbols
    *op1 = SStackPop(symstack);
    *op2 = SStackPop(symstack);
    *op3 = SStackPop(symstack);
  } else {
    // In case there is no op symbol or there are too many. Shouldn't happen
    vypluj err(SYNTAX_ERR);
  }

  // Pop the '<'
  SStackPop(symstack);
  return 0;
}

/**
 * @brief If an operand is not an integer, convert it (generate code)
 *
 * @param op that should be an integer
 *
 * @return 0 if successful, errcode otherwise 
 */
int ensureInteger(SStackElem *op) {
  // If it is a number, convert it to an integer
  if (op->dataType == dt_number) {
    op->data = genConvertFloatToInt(op);
    op->dataType = dt_integer;

    // If it not a num and neither an integer, throw an error
  } else if (op->dataType != dt_integer) {
    return err(TYPE_EXPR_ERR);
  }

  return 0;
}

/**
 * @brief If an operand is not a number, convert it (generate code)
 *
 * @param op that should be a number
 *
 * @return 0 if successful, errcode otherwise 
 */
int ensureNumber(SStackElem *op) {
  // If it is an integer, convert it to a number
  if (op->dataType == dt_integer) {
    op->data = genConvertIntToFloat(op);
    op->dataType = dt_number;

    // If it not an int and neither a number, throw an error
  } else if (op->dataType != dt_number) {
    return err(TYPE_EXPR_ERR);
  }

  return 0;
}

/**
 * @param Check data types of operators of a binary operation (and convert them
 * if necessary)
 *
 * @param op1: first operand
 * @param op2: operator
 * @param op3: second operand
 *
 * @return 0 if successful, errcode otherwise 
 */
int checkDataTypesOfBinOps(SStackElem *op1, SStackElem *op2,
                           SStackElem *op3) {
  // Concatenation operation needs two strings
  if (op2->op == pt_concat && 
      (op1->dataType != dt_string || op3->dataType != dt_string)) {
    return err(TYPE_EXPR_ERR);

    // If we're dividing with //, convert both operands to integers
  } else if (op2->op == pt_intDiv) {
    CondCall(ensureInteger, op1);
    CondCall(ensureInteger, op3);

    // If we're dividing floats, convert both to floats
  } else if (op2->op == pt_div) {
    CondCall(ensureNumber, op1);
    CondCall(ensureNumber, op3);

    // Otherwise, if we have an integer and a number -> convert the int to num
  } else if (op1->dataType == dt_integer && op3->dataType == dt_number) {
    CondCall(ensureNumber, op1);
  } else if (op3->dataType == dt_integer && op1->dataType == dt_number) {
    CondCall(ensureNumber, op3);

    // Else, throw an error if types don't match
  } else if (op1->dataType != op3->dataType) {
    return err(TYPE_EXPR_ERR);
  }

  return 0;
}

/*
 *
 * Creating and destroying symbols
 *
 */

/**
 * @brief Receives a token and return a new symbol of the symbol stack
 *
 * @param symtab: the symbol table 
 * @param token to be converted to SStackElem
 * @param newSymbol: destination pointer
 *
 * @return 0 if successful, errcode otherwise 
 */
int parseToken(STStack *symtab, Token *token, SStackElem **newSymbol) {
  switch (token->type) {

  // Variable, or a nil literal
  case t_idOrKeyword:
    CondCall(createSymbol, newSymbol, st_idOrLiteral, pt_id, false, -1, NULL);

    // If it is a nil literal
    if (strEq(token->data, "nil")) {
      (*newSymbol)->isId = false;
      (*newSymbol)->dataType = dt_nil;

      // If it is an ID (and not a nil)
    } else if (STFind(symtab, token->data)) {
      (*newSymbol)->isId = true;
      (*newSymbol)->dataType = STGetVarDataType(symtab, token->data);

      // The ID does not exist!
    } else {
      return err(ID_DEF_ERR);
    }

    GCMalloc((*newSymbol)->data, sizeof(char) * (strlen(token->data) + 1));
    memcpy((*newSymbol)->data, token->data, strlen(token->data) + 1);
    break;

  // A literal (integer, number, string)
  case t_int:
  case t_num:
  case t_sciNum:
  case t_str:
    CondCall(createSymbol, newSymbol, st_idOrLiteral, pt_id, false, -1, NULL);

    // Copy the data
    if (token->type == t_int) {
      (*newSymbol)->dataType = dt_integer;
    } else if (token->type == t_num || token->type == t_sciNum) {
      (*newSymbol)->dataType = dt_number;
    } else if (token->type == t_str) {
      (*newSymbol)->dataType = dt_string;
    }

    GCMalloc((*newSymbol)->data, sizeof(char) * (strlen(token->data) + 1));
    memcpy((*newSymbol)->data, token->data, strlen(token->data) + 1);
    (*newSymbol)->isZero = isZero(*newSymbol);
    break;

  // (
  case t_leftParen:
    CondCall(createSymbol, newSymbol, st_operator, pt_lParen, false, -1, NULL);
    break;

  // )
  case t_rightParen:
    CondCall(createSymbol, newSymbol, st_operator, pt_rParen, false, -1, NULL);
    break;

  // Arithmetic and string operators (+ - * / // .. #)
  case t_arithmOp:
  case t_strOp:
    CondCall(createSymbol, newSymbol, st_operator, -1, false, -1, NULL);

    if (strEq(token->data, "+")) {
      (*newSymbol)->op = pt_add;
    } else if (strEq(token->data, "-")) {
      (*newSymbol)->op = pt_sub;
    } else if (strEq(token->data, "#")) {
      (*newSymbol)->op = pt_strlen;
    } else if (strEq(token->data, "*")) {
      (*newSymbol)->op = pt_mult;
    } else if (strEq(token->data, "/")) {
      (*newSymbol)->op = pt_div;
    } else if (strEq(token->data, "//")) {
      (*newSymbol)->op = pt_intDiv;
    } else if (strEq(token->data, "..")) {
      (*newSymbol)->op = pt_concat;
    }
    break;

  // Relational operators (== ~= < > <= >=)
  case t_relOp:
    CondCall(createSymbol, newSymbol, st_operator, pt_relOp, false, -1, NULL);
    GCMalloc((*newSymbol)->data, sizeof(char) * (strlen(token->data) + 1));
    memcpy((*newSymbol)->data, token->data, strlen(token->data) + 1);
    break;
  }

  return 0;
}

/**
 * @brief Allocates a new symbol and initializes it (a symbol to be inserted to 
 * the symbol stack)
 *
 * @param newSymbol: destination pointer
 * @param symbol: enum value of symbol stack type 
 *
 * @return 0 if successful, errcode otherwise
 */
int allocateSymbol(SStackElem **newSymbol, int symbol) {
  GCMalloc(*newSymbol, sizeof(SStackElem));
  (*newSymbol)->type = symbol;
  (*newSymbol)->op = -1;
  (*newSymbol)->isId = false;
  (*newSymbol)->dataType = -1;
  (*newSymbol)->data = NULL;
  (*newSymbol)->next = NULL;
  return 0;
}

/**
 * @brief Allocates a new symbol and sets its data based on parameters
 *
 * @param newSymbol: destination pointer
 * @param type
 * @param op: precedence table enum
 * @param isId: bool
 * @param dataType: data type enum
 * @param data: string representing the symbol
 *
 * @return 0 if successful, errcode otherwise
 */
int createSymbol(SStackElem **newSymbol, int type, int op, bool isId, 
    int dataType, char *data) {
  CondCall(allocateSymbol, newSymbol, type);
  if (!(*newSymbol)) {
    return err(INTERN_ERR);
  }
  (*newSymbol)->op = op;
  (*newSymbol)->isId = isId;
  (*newSymbol)->dataType = dataType;
  (*newSymbol)->data = data;
  (*newSymbol)->isZero = false;
  return 0;
}

/*
 *
 * Helper functions
 *
 */

/**
 * @brief Init before the precedence analysis begins. Init the symbol stack,
 * push '$' to it, check if we received a token from the parser and check if
 * the token is a function (we don't parse functions here)
 *
 * @param symtab: symbol table
 * @param symstack: where to save the initialized symbol stack
 * @param token: the token received from the parser
 *
 * @return 0 if successful, -1 if the token is a fn call, errcode otherwise 
 */
int precedenceAnalysisInit(STStack *symtab, SStack **symstack, Token **token) {

  // Initialize a symbol stack
  CondCall(SStackInit, symstack);

  // Push a $ to the stack
  SStackElem *initialSymbol = NULL;

  CondCall(createSymbol, &initialSymbol, st_dollar, pt_dollar, false, -1, NULL);
  CondCall(SStackPush, *symstack, initialSymbol);

  // Get a new token if it was not provided by the parser
  if (!(*token)) {
    CondCall(scanner, token);
  }

  // Check if the next token is a function - if so, stash the token and let the
  // parser know by returning -1
  if (STFind(symtab, (*token)->data) 
      && !STGetIsVariable(symtab, (*token)->data)) {
    CondCall(stashToken, token);
    return -1;
  }

  // If the token is not allowed to be a part of the expression => syntax err
  if (!isTokenAllowedInExpr(*token)) {
    return err(SYNTAX_ERR);
  }

  return 0;
}

/**
 * @brief If we're still receiving tokens that are a part of the expression, we
 * fetch it in this function and convert it into a symbol. Otherwise the new
 * symbol is just a 'reduce' symbol. Also sets the exprCanEnd to false if the
 * last symbol was an operator (and vice versa)
 *
 * @param symtab: symbol table
 * @param token: destination pointer for the token
 * @param inputSymbol: destination pointer for the new symbol
 * @param getNewToken: boolean representing if we're (not) done fetching tokens
 * @param exprCanEnd
 * @param exprEnd
 *
 * @returns 0 if successful, errcode otherwise 
 */
int getNewSymbol(STStack *symtab, Token **token, SStackElem **inputSymbol,
                 bool getNewToken, bool *exprCanEnd, bool *exprEnd) {

  // Get a new token (check if it is a part of the expression)
  if (getNewToken) {
    CondCall(fetchNewToken, token, *exprCanEnd, exprEnd);
  }

  // Parse the token into a symbol (if we're still fetching new tokens)
  if (!(*exprEnd)) {
    CondCall(parseToken, symtab, *token, inputSymbol);
    // If we fetched all tokens belonging to the expression, input should be '>'
  } else {
    CondCall(createSymbol, inputSymbol, st_reduce, pt_dollar, false, -1, NULL);
  }

  // If the symbol is not an operator, the expr can end by the next token
  // eg. 1 + 1 <expr can end here>; 1 + 1 + <expr cannot end here>
  if ((*inputSymbol)->type == st_operator) {
    *exprCanEnd = false;
  } else {
    *exprCanEnd = true;
  }

  return 0;
}

/**
 * @brief Get a new token and set exprEnd to true if the new token is not a 
 * part of the expression
 *
 * @param token: address where the new token should be saved
 * @param exprCanEnd
 * @param exprEnd
 *
 * @return 0 if successful, errcode otherwise 
 */
int fetchNewToken(Token **token, bool exprCanEnd, bool *exprEnd) {
  // Call the scanner
  CondCall(scanner, token);

  // ',', ':', '=' and keywords (not including nil) can't be a part of expr
  if (!(*exprEnd) && !isTokenAllowedInExpr(*token)) {
    CondCall(stashToken, token);
    *exprEnd = true;

    // If the last token was an operand and the next token is too, expr ends
  } else if (exprCanEnd && isTokenIdOrLiteral(*token)) {
    CondCall(stashToken, token);
    *exprEnd = true;
  }
  return 0;
}

/**
 * @brief Check if a a symbol represents a zero literal (an int or a num)
 *
 * @param operand as a symbol from the symbol stack
 *
 * @return true if the number is a zero
 */
bool isZero(SStackElem *operand) {
  if (operand->type == st_idOrLiteral 
      && operand->op == pt_id 
      && operand->isId == false 
      && operand->data 
      && (operand->dataType == dt_integer || operand->dataType == dt_number)) {
    char *todptr = NULL;
    double res = strtod(operand->data, &todptr);
    // If conversion was successful (todptr[0] == '\0')
    if (!todptr[0]) {
      // If the number equals 0
      if (res == 0.0) {
        return true;
      }
    }
  }
  return false;
}

/**
 * @brief returns true if a symbol is an binary arithmetic operator
 *
 * @param op: symbol
 *
 * @return true if a symbol is an binary arithmetic operator
 */
bool isBinArithmOp(SStackElem *op) {
  return op->type == st_operator &&
         (op->op == pt_mult 
          || op->op == pt_div 
          || op->op == pt_intDiv 
          || op->op == pt_add 
          || op->op == pt_sub 
          || op->op == pt_concat);
}

/**
 * @brief check if the token is allowed to be a part of the expression
 *
 * @param token
 *
 * @returns true if token is allowed in the expression
 */
bool isTokenAllowedInExpr(Token *token) {
  if (token->type == t_colon 
      || token->type == t_comma 
      || token->type == t_assignment 
      || (!strEq(token->data, "nil") && isKeyword(token))) {
    return false;
  }
  return true;
}

/**
 * @brief checks if the token is an ID or a literal
 *
 * @param token
 *
 * @return true if the token is an ID or a literal
 */
bool isTokenIdOrLiteral(Token *token) {
  return token->type == t_idOrKeyword 
    || token->type == t_int 
    || token->type == t_num 
    || token->type == t_sciNum 
    || token->type == t_str;
}

/**
 * @brief Checks if there is only $E in the stack
 *
 * @param symstack: symbol stack
 *
 * @returns true if there is only $E in the stack
 */
bool isExprAtomic(SStack *symstack) {
  // We need exactly two elements in the symstack
  if (symstack->top && symstack->top->next && !symstack->top->next->next) {
    return symstack->top->type == st_expr 
      && symstack->top->next->type == st_dollar;
  }
  return false;
}

/**
 * @brief Simple debugging function to print all elements on the symbol stack
 *
 * @param stack to print out
 */
void debugPrint(SStack *stack) {
  return;
  int len = 0;
  SStackElem *element = SStackTop(stack);
  ;
  fprintf(stderr, "--------STACK------------\n");
  while (element != NULL) {
    fprintf(stderr, "Element type: %d\n", element->type);
    fprintf(stderr, "Element op: %d\n", element->op);
    if (element->data != NULL) {
      fprintf(stderr, "Element data: %s\n", element->data);
    } else {
      fprintf(stderr, "Element data is NULL\n");
    }
    fprintf(stderr, "Element isId: %d\n", element->isId);
    fprintf(stderr, "Element dataType: %d\n", element->dataType);
    fprintf(stderr, "-------------------------\n");

    if (element->next == NULL) {
      fprintf(stderr, "Next element is NULL\n");
      fprintf(stderr, "Total length is: %d\n", len + 1);
      fprintf(stderr, "-------------------------\n");
      return;
    } else {
      element = element->next;
      len++;
    }
  }
  fprintf(stderr, "Stack is empty\n");
}


#endif
/* end of file precedence_analysis.c*/
