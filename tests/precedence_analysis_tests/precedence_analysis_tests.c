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
  }
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

// Function call (should return the token)
// fn()
int test1(){
  parseExpression(symtable, token, &retVarName);

  // Precedence analysis should have returned the token
  ret = scanner(&token);
  CondReturn;
  if(strcmp(token->data, "fn")){
    fprintf(stderr, "Function call token was not returned by the precedence analysis.\n");
    return 1;
  }

  // Consume the tokens: fn, (, )
  consumeTokens(3);

  return 0;
}

// A variable
// var
int test2(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// An integer literal
// 1
int test3(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A string literal
// "abc"
int test4(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A simple expression with literals
// 1 + 1
int test5(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A simple expression with variables
// varInt1 + varInt2
int test6(){
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
int test7(){
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
int test8(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// Test a simple concat expr
// varStr1 .. "abc"
int test9(){
  parseExpression(symtable, token, &retVarName);

  if(!retVarName){
    fprintf(stderr, "No variable name was returned by the precedence analysis\n");
    return 1;
  }

  return 0;
}

// A simple expression with an unary operator
// #"abc"
// #varStr1
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
  pushVarToSymtab("varStr1", dt_integer);


  /*
   * Run the tests: get a token and call the test function
   */

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'fn()\'\n");
  ret = test1();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'varInt1\'\n");
  ret = test2();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'1\'\n");
  ret = test3();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'\"abc\"\'\n");
  ret = test4();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'1 + 1\'\n");
  ret = test5();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'varInt1 + varInt2\'\n");
  ret = test6();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'varInt1 + 1\' and \'varInt1 - 1\'\n");
  ret = test7();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'varInt1 * 2.0\' and \'varInt1 / 2.0\' and \'1.1E2 // varInt1\'\n");
  ret = test8();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'varStr1 .. \"abc\"\'\n");
  ret = test9();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  printf("Testing: \'#\"abc\"\' and \'#varStr1\'\n");
  ret = test10();
  CondReturn;

  ret = scanner(&token);
  CondReturn;
  ret = test11();
  CondReturn;

  return 0;
}
