#include "../../precedence_analysis.h"

/*
 * Global variables
 */

extern int ret;

STStack *symtable = NULL;

Token *token = NULL;

char *retVarName = NULL;
int retDataType = -1;

extern DynamicStringArray *varDefBuf;

/*
 * Helper functions
 */

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

void initPushToSymtab(){
  // Push needed variables and functions to the symbol table
  pushFnToSymtab("fn1");
  pushFnToSymtab("fn2");
  pushFnToSymtab("fn3");
  pushFnToSymtab("fn4");
  pushFnToSymtab("fn5");

  pushVarToSymtab("varInt1", dt_integer);
  pushVarToSymtab("varInt2", dt_integer);
  pushVarToSymtab("varInt3", dt_integer);
  pushVarToSymtab("varInt4", dt_integer);
  pushVarToSymtab("varInt5", dt_integer);

  pushVarToSymtab("varNum1", dt_number);
  pushVarToSymtab("varNum2", dt_number);
  pushVarToSymtab("varNum3", dt_number);
  pushVarToSymtab("varNum4", dt_number);
  pushVarToSymtab("varNum5", dt_number);

  pushVarToSymtab("varStr1", dt_string);
  pushVarToSymtab("varStr2", dt_string);
  pushVarToSymtab("varStr3", dt_string);
  pushVarToSymtab("varStr4", dt_string);
  pushVarToSymtab("varStr5", dt_string);
}

void printTokenData(){
  printf("Testing: Token type is \'%d\' and data \'%s\'\n", token->type, token->data);
}

/*
 * Main
 */

int main(){
  // Init a symtable and push one frame
  STStackInit(&symtable);
  STPush(symtable);

  // Init the vardef buffer
  TryCall(dynStrArrInit, &varDefBuf);

  // Push 5 IDs to the symtab for every data type: fn1, fn2...fn3, varInt1, ...
  initPushToSymtab();

  // Get the first token and print it's details
  scanner(&token);
  printTokenData();

  // Run the test
  int ret = parseExpression(symtable, token, &retVarName, &retDataType);
  printf("Testing: Expression result is in: %s and has data type of %d\n", retVarName, retDataType);
  printf("Testing: Precedence analysis returned %d\n", ret);

  return 0;
}
