#include "../../precedence_analysis.h"

/*
 * Global variables
 */

extern int ret;

STStack *symtable = NULL;

Token *token = NULL;

char *retVarName = NULL;

/*
 * Helper functions
 */

void consumeTokens(int amount){
  for(int i = 0; i < amount; i++){
    scanner(&token);
    if(token->data){
      fprintf(stderr, "Testing: Consumed token, type is %d and data %s\n", token->type, token->data);
    }else{
      fprintf(stderr, "Testing: Consumed token, type is %d and data NULL\n", token->type);
    }
  }
}

void printTokenData(){
  printf("Testing: Token type is \'%d\' and data \'%s\'\n", token->type, token->data);
}

void pushVarToSymtab(char *name, int dataType){
  STInsert(symtable, name);
  STSetIsVariable(symtable, name, true);
  STSetVarDataType(symtable, name, dataType);
}

void pushFnToSymtab(char *name){
  STInsert(symtable, name);
  STSetIsVariable(symtable, name, false);
  STSetFnDefined(symtable, name, true);
}

/*
 * Tests
 */

// A variable
// var
int test1(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// An integer literal
// 1
int test2(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A string literal
// "abc"
int test3(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A simple expression with literals
// 1 + 1
int test4(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A simple expression with variables
// varInt1 + varInt2
int test5(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A simple expressions with a literal and a variable
// varInt1 + 1
// varInt1 - 1
int test6(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  ret = scanner(&token);
  CondReturn;

  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// Test an integer and a number
// varInt1 * 2.0
// varInt1 / 2.0
// 1.1E2 // varInt1
int test7(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  ret = scanner(&token);
  CondReturn;

  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  ret = scanner(&token);
  CondReturn;

  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// Test a simple concat expr
// varStr1 .. "abc"
int test8(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// Function call (should return the token)
// fn()
int test9(){
  parseExpression(symtable, token, &retVarName);

  // Precedence analysis should have returned the token (consume it)
  ret = scanner(&token);
  CondReturn;
  if(strcmp(token->data, "fn")){
    fprintf(stderr, "Function call token was not returned by the precedence analysis.\n");
    return 1;
  }

  // Consume the tokens: ( and )
  consumeTokens(2);

  return 0;
}

// A simple expression with an unary operator
// #"abc"
int test10(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// More complicated expressions
int test11(){
  // TODO not done yet
  return 0;
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}


int main(){
  // Init a symtable and push one frame
  symtable = STStackInit();
  STPush(symtable);

  // Push needed variables and functions to the symbol table
  pushFnToSymtab("fn");
  pushVarToSymtab("varInt1", dt_integer);
  pushVarToSymtab("varInt2", dt_integer);
  pushVarToSymtab("varStr1", dt_string);


  /*
   * Run the tests: get a token and call the test function
   */

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'varInt1\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test1();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'1\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test2();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'\"abc\"\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test3();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'1 + 1\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test4();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'varInt1 + varInt2\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test5();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'varInt1 + 1\' and \'varInt1 - 1\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test6();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'varInt1 * 2.0\' and \'varInt1 / 2.0\' and \'1.1E2 // varInt1\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test7();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'varStr1 .. \"abc\"\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test8();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'fn()\'\n");
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test9();
  CondReturn;

  printf("====================NEXT TEST=====================\n");
  printf("Testing: \'#\"abc\"\'\n");
  consumeTokens(2); // consume i and = 
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test10();
  CondReturn;

  return 0;

  // TODO Advanced expressions
  ret = scanner(&token);
  CondReturn;
  printTokenData();
  ret = test11();
  CondReturn;

  return 0;
}
