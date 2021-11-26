#include "../../precedence_analysis.h"

/*
 * Global variables
 */
int ret = 0;
STStack *symtable = NULL;
Token *token = NULL;


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


// Function call (should return the token)
// fn()
int test1(){
  return 0;
}

// A variable
// var
int test2(){
  return 0;
}

// An integer literal
// 1
int test3(){
  return 0;
}

// A string literal
// "abc"
int test4(){
  return 0;
}

// A simple expression with literals
// 1 + 1
int test5(){
  return 0;
}

// A simple expression with variables
// var1 + var2
int test6(){
  return 0;
}

// A simple expressions with a literal and a variable
// var1 + 1
// var1 - 1
int test7(){
  return 0;
}

// Test an integer and a number
// var1 * 2.0
// var1 / 2.0
// var1 // 1.0
int test8(){
  return 0;
}

// Test a simple concat expr
// var1 .. "abc"
int test9(){
  return 0;
}

// A simple expression with an unary operator
// #"abc"
// #var1
int test10(){
  return 0;
}

// More complicated expressions
int test11(){
  return 0;
}


int main(){
  // Init a symtable and push one frame
  symtable = STStackInit();
  STPush(symtable);

  // Get one token to pass the precedence analysis
  ret = scanner(&token);
  if(ret){
    printf("Scanner returned %d\n", ret);
    return ret;
  }


  // Run the tests:
  ret = test1();
  CondReturn;
  ret = test2();
  CondReturn;
  ret = test3();
  CondReturn;
  ret = test4();
  CondReturn;
  ret = test5();
  CondReturn;
  ret = test6();
  CondReturn;
  ret = test7();
  CondReturn;
  ret = test8();
  CondReturn;
  ret = test9();
  CondReturn;
  ret = test10();
  CondReturn;
  ret = test11();
  CondReturn;

  return 0;
}
