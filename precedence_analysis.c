/*
 * Precedence analysis
 */

#ifndef PRECEDENCE_ANALYSIS_C
#define PRECEDENCE_ANALYSIS_C

#define TRYRULE(FN, ...)                  \
    rulesRet = FN(symstack, __VA_ARGS__); \
    if(rulesRet != -1){                   \
      return rulesRet;                    \
    }

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


/**
 * @brief Simple function to print all elements on the symbol stack
 *
 * @param stack to print out
 */
void debugPrint(SStack *stack) {
  return;
  int len = 0;
  SStackElem *element = SStackTop(stack);;
  fprintf(stderr, "--------STACK------------\n");
  while (element != NULL) {
    fprintf(stderr,"Element type: %d\n",element->type);
    fprintf(stderr,"Element op: %d\n",element->op);
    if(element->data != NULL) {
      fprintf(stderr,"Element data: %s\n",element->data);
    }else{
      fprintf(stderr,"Element data is NULL\n");
    }
    fprintf(stderr,"Element isId: %d\n",element->isId);
    fprintf(stderr,"Element dataType: %d\n",element->dataType);
    fprintf(stderr, "-------------------------\n");

    if(element->next == NULL) {
      fprintf(stderr,"Next element is NULL\n");
      fprintf(stderr,"Total length is: %d\n",len + 1);
      fprintf(stderr, "-------------------------\n");
      return;
    } else {
      element = element->next;
      len++;
    }
  }
  fprintf(stderr, "Stack is empty\n");
}

/**
 * @brief Allocates a new symbol and initializes it (a symbol to be inserted to 
 * the symbol stack)
 *
 * @param symbol: enum value of symbol stack type 
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
 * @param elem to be destroyed
 */
void destroySymbol(SStackElem **elem){
  if(elem && *elem){
    if((*elem)->data){
      free((*elem)->data);
    }
    free(*elem);
  }
  *elem = NULL;
}


/**
 * @brief Check if a string represents a zero (as a number)
 *
 * @param str containing the number (can be an integer, a floating point number
 * or in scientific format)
 *
 * TODO test this somehow please
 */
bool isZero(SStackElem *operand){
  if(operand && operand->isId == false){
    char *todptr = NULL;
    double res = strtod(operand->data, &todptr);
    if(!todptr || todptr[0] == 'e' || todptr[0] == 'E' || todptr[0] == '\0'){
      if(res == 0.0){
        return true;
      }
    }
  }
  return false;
}


/*
 * Rule functions
 */


/**
 * @brief Pop 'amount' of symbols from the stack and pass it down by
 * parameters. Also pop the '<' beneath them
 *
 * TODO
 */
int getSymbolsForReduction(SStack *symstack, SStackElem **op1, 
    SStackElem **op2, SStackElem **op3, int amount){

  if(amount == 1) {
    // Get the first symbol from the stack (and pop it)
    *op1 = SStackPop(symstack);
  } else if(amount == 2){
    // Get first two symbols
    *op1 = SStackPop(symstack);
    *op2 = SStackPop(symstack);
  }else if(amount == 3){
    // Get three symbols 
    *op1 = SStackPop(symstack);
    *op2 = SStackPop(symstack);
    *op3 = SStackPop(symstack);
  }else{
    // In case there is no op symbol or there are too many. Shouldn't happen
    fprintf(stderr, "This is odd. Wrong amount of symbols to reduce.\n");
    vypluj err(SYNTAX_ERR); // TODO errcode
  }

  // Pop the '<'
  SStackPop(symstack);

  return 0;
}

int checkRules(SStack *symstack, int opSymbols){
  // Will be 0 if a rule was found and applied, -1 if not, error code otherwise
  int rulesRet = -1; 
        
  // Get the symbols that are to be reduced
  SStackElem *op1 = NULL, *op2 = NULL, *op3 = NULL;
  CondCall(getSymbolsForReduction, symstack, &op1, &op2, &op3, opSymbols);

  // If we only have one symbol, call iRule (which reduces i to E)
  if(opSymbols == 1) {
    TRYRULE(iRule, op1);

  // Unary operator
  } else if(opSymbols == 2) {
    TRYRULE(strLenRule, op2, op1);

  // Binary operator
  } else if (opSymbols == 3) {
    TRYRULE(bracketsRule, op3, op2, op1);
    TRYRULE(arithmeticOperatorsRule, op3, op2, op1);
    TRYRULE(relationalOperatorsRule, op3, op2, op1);
  }

  vypluj rulesRet;
}

/**
 * @brief "i rule" - just convert the i to E and push it back to the stack
 *
 * TODO
 */
int iRule(SStack *symstack, SStackElem *op) {
  // The symbol needs to be an 'i'
  if(op->type == st_idOrLiteral){
    op->type = st_expr;
    SStackPush(symstack, op);
    vypluj 0;
  }
  vypluj -1;
}


SStackElem *newSymbol(int type, int op, bool isId, int dataType, char *data){
  SStackElem *newElem = allocateSymbol(type);
  if(!newElem){
    return NULL;
  }
  newElem->op = op;
  newElem->isId = isId;
  newElem->dataType = dataType;
  newElem->data = data;
  return newElem;
}

/**
 * @brief "#E rule" - generate code for strlen
 *
 * TODO
 */
int strLenRule(SStack *symstack, SStackElem *op1, SStackElem *op2){
  if(op1->type == st_operator && op1->op == pt_strlen){

    // We received a '#' operator but no i after it
    if(op2->type != st_expr){
      vypluj err(SYNTAX_ERR); // TODO errcode
    }
     
    // The data type of the operand must be a string
    if(op2->dataType != dt_string){
      vypluj err(TYPE_EXPR_ERR);
    }

    // Generate code
    char *name = genUnaryOperation(op2);

    // Create a new sym stack element (E) and push it
    SStackElem *newElem = newSymbol(st_expr, pt_id, true, dt_integer, name);
    SStackPush(symstack, newElem);

    // Destroy both old symbols
    destroySymbol(&op1);
    destroySymbol(&op2);

  }else{
    // No rule
    return -1;
  }
  return 0;
}

/**
 * @brief "(E) rule" - destroy the parantheses and push the E back
 *
 * TODO
 */
int bracketsRule(SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) {
  if(op1->op == pt_lParen && op3->op == pt_rParen && op2->type == st_idOrLiteral){

    // Push the E and destroy the parentheses
    SStackPush(symstack, op2);
    destroySymbol(&op1);
    destroySymbol(&op3);

  }else{
    // No rule
    return -1;
  }

  vypluj 0;
}

int ensureInteger(SStackElem *op){
  // If it is a number, convert it to an integer
  if(op->dataType == dt_number){
    op->data = genConvertFloatToInt(op);
    op->dataType = dt_integer;
  // If it not a num and neither an integer, throw an error
  }else if(op->dataType != dt_integer){
    return err(TYPE_EXPR_ERR); // TODO errcode
  }
  return 0;
}

int ensureNumber(SStackElem *op){
  // If it is an integer, convert it to a number
  if(op->dataType == dt_integer){
    op->data = genConvertIntToFloat(op);
    op->dataType = dt_number;
  // If it not an int and neither a number, throw an error
  }else if(op->dataType != dt_number){
    return err(TYPE_EXPR_ERR); // TODO errcode
  }
  return 0;
}

int checkDataTypesOfBinaryOperations(SStackElem *op1, SStackElem *op2, 
    SStackElem *op3){
  // Concatenation operation needs two strings
  if(op2->op == pt_concat && (op1->dataType != dt_string || op3->dataType != dt_string)){
    return err(TYPE_EXPR_ERR);

  // If we're dividing with //, convert both operands to integers
  }else if(op2->op == pt_intDiv){
    CondCall(ensureInteger, op1);
    CondCall(ensureInteger, op3);

  // If we're dividing floats, convert both to floats
  }else if(op2->op == pt_div){
    CondCall(ensureNumber, op1);
    CondCall(ensureNumber, op3);

  // Otherwise, if we have an integer and a number -> convert the int to num
  }else if(op1->dataType == dt_integer && op3->dataType == dt_number){
    CondCall(ensureNumber, op1);

  // ... and vice versa
  }else if(op3->dataType == dt_integer && op1->dataType == dt_number){
    CondCall(ensureNumber, op3);

  // Else, throw an error if types don't match
  }else if(op1->dataType != op3->dataType){
    return err(TYPE_EXPR_ERR);
  }
  return 0;
}

bool isOperand(SStackElem *op){
  return true;
}
bool isUnaryOperator(SStackElem *op){
  return true;
}
bool isBinaryArithmeticOperator(SStackElem *op){
  return op->type == st_operator && 
      (op->op == pt_mult || op->op == pt_div || op->op == pt_intDiv
      || op->op == pt_add || op->op == pt_sub || op->op == pt_concat);
}

// i OPERATOR i or E OPERATOR E ?
// "+ - * / // .."
//
// pt_concat only works with two strings
// pt_{mult, div, add, sub} work with integers and numbers
// pt_intDiv works with integers and numbers, always returns an integer
int arithmeticOperatorsRule(SStack *symstack, SStackElem *op1, 
    SStackElem *op2, SStackElem *op3) {

  // op2 needs to be an arithmetic operator, op1 and op3 need to be E
  if(isBinaryArithmeticOperator(op2) && op1->type == st_expr && op3->type == st_expr){

    // Check the data types
    CondCall(checkDataTypesOfBinaryOperations, op1, op2, op3);

    // Division by zero check (op3 can't be zero)
    if((op2->op == pt_div || op2->op == pt_intDiv) && isZero(op3)){
      return err(DIV_BY_ZERO_ERR);
    }

    char *newName = NULL;
    int dataType = -1;

    // Generate the operator code:
    if(op2->op == pt_add){
      newName = genBinaryOperationAdd(op1, op3);
    }else if(op2->op == pt_sub){
      newName = genBinaryOperationSub(op1, op3);
    }else if(op2->op == pt_mult){
      newName = genBinaryOperationMul(op1, op3);
    }else if(op2->op == pt_div){
      newName = genBinaryOperationDiv(op1, op3);
      dataType = dt_number;
    }else if(op2->op == pt_intDiv){
      newName = genBinaryOperationIDiv(op1, op3);
      dataType = dt_integer;
    }else if(op2->op == pt_concat){
      newName = genBinaryOperationConcat(op1, op3);
    }else{
      return err(SYNTAX_ERR);
    }

    // Destroy all old symbols
    destroySymbol(&op1);
    destroySymbol(&op2);
    destroySymbol(&op3);

    // Create a new symbol and push it to the symstack
    SStackElem *newElem = newSymbol(st_expr, pt_id, false, dataType, newName);
    SStackPush(symstack, newElem);

  }else{
    // No rule
    return -1;
  }

  vypluj 0;
}


// i RELOP i or E RELOP E ?
int relationalOperatorsRule(SStack *symstack, SStackElem *op1, 
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
          // Convert op1 from int to number 
          op1->data = genConvertIntToFloat(op1);
          op1->dataType = dt_number;


        }else if(op3->dataType == dt_integer && op1->dataType == dt_number){
          // Convert op3 from int to number 
          op3->data = genConvertIntToFloat(op3);
          op3->dataType = dt_number;

        // Else, throw an error
        }else{
          return err(TYPE_EXPR_ERR);
        }
      }
    }

    SStackElem *newElem = allocateSymbol(st_expr);
    newElem->op = pt_id;
    newElem->isId = true;
    newElem->dataType = -1; // It's gonna be a boolean.. So.. What now?

    if(!strcmp(op2->data, "<") || !strcmp(op2->data, ">=")){
      newElem->data = genLower(op1, op3);
    }else if(!strcmp(op2->data, ">") || !strcmp(op2->data, "<=")){
      newElem->data = genGreater(op1, op3);
    }else if(!strcmp(op2->data, "==") || !strcmp(op2->data, "~=")){
      newElem->data = genEqual(op1, op3);
    }else{
      fprintf(stderr, "This is awkward. The operator should be relational.\n");
      return err(-1); // todo? This should not be possible
    }

    // Generate code for NOT - to negate the result of the expression
    // eg. if we have '~=', we'll generate code for '==' but need to negate it,
    // for '<=', we'll generate '>',...
    if(!strcmp(op2->data, "<=") 
        || !strcmp(op2->data, ">=") 
        || !strcmp(op2->data, "~=")){
      newElem->data = genUnaryOperation(op1);
      // newElem->data = gen...
      newElem->data = genNot(op1, op3);
    }

    // Push the new element to the symstack (E)
    SStackPush(symstack, newElem);

  }else{
    // No rule
    return -1;
  }

  // Destroy all old symbols
  destroySymbol(&op1);
  destroySymbol(&op2);
  destroySymbol(&op3);


  vypluj 0;
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
      // Check if it is a nil (cause it might be)
      if(!strcmp(token->data, "nil")){
        newElem->isId = false;
        newElem->dataType = dt_nil;

      // Check it the ID exists (we now know it is an ID)
      }else if(!STFind(symtab, token->data)){
        fprintf(stderr, "This ID does not exist in the symbol table: %s\n", token->data);
        // THE ID DOES NOT EXIST
        exit(ID_DEF_ERR);
        return NULL;
      // It is an ID (and not a nil)
      }else{
        newElem->isId = true;
        // Read the data type from the symbol table
        newElem->dataType = STGetVarDataType(symtab, token->data);

      }
      newElem->type = st_idOrLiteral;
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

    // TODO remove these three cases when PA is tested
    case t_colon:
    case t_comma:
    case t_assignment:
      // We're not calling this function if we encounter one of these types
      fprintf(stderr, "This is awkward. How did we get this in parse token function?\n");
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
 * @brief Init before the precedence analysis begins. Init the symbol stack,
 * push '$' to it, check if we received a token from the parser and check if
 * the token is a function (we don't parse functions here)
 *
 * @param symtab: symbol table
 * @param symstack: where to save the initialized symbol stack
 * @param token: the token received from the parser
 *
 * @return 0 if successful, -1 if the token is a function call
 */
int precedenceAnalysisInit(STStack *symtab, SStack **symstack, Token **token){
  // Initialize a symbol stack
  *symstack = SStackInit();

  // Push a $ to the stack
  SStackElem *initialSymbol = allocateSymbol(st_dollar);
  initialSymbol->type = st_dollar;
  initialSymbol->op = pt_dollar;
  CondCall(SStackPush, *symstack, initialSymbol);

  // Get a new token if it was not provided by the parser
  if(!(*token)){
    CondCall(scanner, token);
  }

  // Check if the next token is a function - if so, stash the token and let the
  // parser know by returning -1
  if(STFind(symtab, (*token)->data) && !STGetIsVariable(symtab, (*token)->data)){
    stashToken(*token);
    return -1;
  }
  return 0;
}

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
 * @return 0 if successful
 */
int parseExpression(STStack *symtab, Token *token, char **returnVarName) {

  // Init
  SStack *symstack = NULL;
  SStackElem *topSymbol = NULL, *inputSymbol = NULL;
  ret = precedenceAnalysisInit(symtab, &symstack, &token);

  // If init returned -1, the token is a function call and it is not an error
  if(ret == -1){
    return ret;
  }

  // Will be checked every cycle of the 'while' - if true, new token is fetched
  // We already have one to process (param)
  bool getNewToken = false; 

  // Will be true if the next token we fetch might not be a part of the expression
  // If this is false and we receive a token which cannot be a part of an 
  // expression, we have encountered an error
  bool exprCanEnd = true;

  // True if the last scanned token was not a part of the expression. If true,
  // no tokens will be received and only the 'reduce' rule will be used
  bool exprEnd = false;
  
  inputSymbol = parseToken(symtab, token);

  while (1) {
    debugPrint(symstack);

    if(getNewToken){
      // Get a new token
      CondCall(scanner, &token);

      // Check if the next token is a part of the expression

      if(exprEnd){
        inputSymbol = allocateSymbol(st_reduce);
        inputSymbol->op = pt_dollar;

      // If it is a ',', ':' or '=', it means the end of the expression
      // If it is a keyword, this is the end of the expr (unless it's a nil)
      }else if(token->type == t_colon
          || token->type == t_comma
          || token->type == t_assignment
          || (strcmp(token->data, "nil") && isKeyword(token))){
        /** fprintf(stderr, "received a ,/:/= or a keyword\n"); */
        stashToken(token);
        token = NULL;
        exprEnd = true;
        inputSymbol = allocateSymbol(st_reduce);
        inputSymbol->op = pt_dollar;
      

      // If the last token was not an operator, but an ID or a literal and the 
      // next token is an ID or a literal, it is not a part of the expression
      }else if(exprCanEnd && (
          token->type == t_idOrKeyword 
          || token->type == t_int 
          || token->type == t_num 
          || token->type == t_sciNum
          || token->type == t_str)){
        /** fprintf(stderr, "received an id or a literal after exprCanEnd\n"); */
        stashToken(token);
        token = NULL;
        exprEnd = true;
        inputSymbol = allocateSymbol(st_reduce);
        inputSymbol->op = pt_dollar;
      }


      // Parse the new token into a symbol
      // If exprEnd is true, it means that the token was stashed and does not
      // exist any more
      if(!exprEnd){
        inputSymbol = parseToken(symtab, token);
      }

    }
    // If the symbol is not an operator, the expr can end by the next token
    // eg. 1 + 1 <expr can end here>; 1 + 1 + <expr cannot end here>
    if(inputSymbol->type == st_operator){
      exprCanEnd = false;
    }else{
      exprCanEnd = true;
    }


    // Update the top symbol since it might have changed
    topSymbol = SStackTopTerminal(symstack);
    if(!topSymbol){
      fprintf(stderr, "top symbol is NULL\n");
      return -1;
    }
    char precTableSymbol = precTab[topSymbol->op][inputSymbol->op];

    if (precTableSymbol == '=') {
      // Push the input symbol to the stack
      SStackPush(symstack, inputSymbol);
      // Destroy the old token
      tokenDestroy(&token);
      // Get a new token
      getNewToken = true;

    } else if (precTableSymbol == '<') {
      // Allocate a new symbol ('<') and push it after the top terminal
      ret = SStackPushAfterTopTerminal(symstack, allocateSymbol(st_push));
      if(ret == -1){
        printf("no terminal on stack. Hmm\n");
        // no terminal on the stack
        ret = 0;
      }
      // Push the input symbol
      SStackPush(symstack, inputSymbol);
      // Destroy the old token
      tokenDestroy(&token);
      // Get a new token
      getNewToken = true;

    // Reduce ('Convert') top terminals to an expression
    } else if (precTableSymbol == '>') {

      // Call rule functions - if one of them has a rule that reduces the
      // expression, it returns 0 and we're done reducing for now

      // Count symbols that are to be reduced (eg. 2 for "#str")
      int opSymbols = 0;
      SStackElem *tmp = SStackTop(symstack);
      while(tmp && tmp->next && tmp->type != st_push){
        opSymbols++;
        tmp = tmp->next;
      }

      ret = checkRules(symstack, opSymbols);

      // A rule function returned -1 <=> there's no rule able to reduce
      // this expression, which means that there is an error
      if(ret == -1){
        ret = 15; // TODO errcode if no rule was found!
      }
      CondReturn;

      getNewToken = false;

    } else {
      vypluj err(SYNTAX_ERR);
    }

    if(exprEnd
        && symstack && symstack->top && symstack->top->next
        && symstack->top->type == st_expr
        && symstack->top->next->type == st_dollar){
      break;
    }else{
    }

  }

  // If there is only "$E" at the top of the stack, we're done here
  // TODO but we need to be done reading tokens, too
  if(symstack && symstack->top && symstack->top->next
      && symstack->top->type == st_expr
      && symstack->top->next->type == st_dollar){
    printf("Done with precedence analysis\n");
  }else{
    debugPrint(symstack);
    return 15; // TODO errcode
  }
  // Return the name of the variable where the result of the expression is stored
  *returnVarName = symstack->top->data;

  // TODO anything else here to finish up??

  vypluj 0;
}

#endif
/* end of file precedence_analysis.h*/
