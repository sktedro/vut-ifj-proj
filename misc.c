/*
 * Enumerators, structure definitions and miscellaneous functions
 */

#ifndef MISC_C
#define MISC_C

#include "misc.h"
#include "symtable.h"

int ret = 0;
int LOCCount = 1;
bool errMessageWritten = false;
GarbageCollector garbageCollector;

/*
 * Miscellaneous functions
 */

/**
 * @brief returns true if two strings are equal
 *
 * @param str1
 * @param str2
 *
 * @return true if strings are equal
 */
bool strEq(char *str1, char *str2){
  return !strcmp(str1, str2);
}

/**
 * @brief returns an integer representation (enum) of the string, if it
 * contains a data type keyword
 *
 * @param str
 *
 * @return data type enum or -1 if str is not a data type keyword
 */
int getDataTypeFromString(char *str){
  if(strEq(str, "integer")){
    return dt_integer;
  }else if(strEq(str, "number")){
    return dt_number;
  }else if(strEq(str, "string")){
    return dt_string;
  }else if(strEq(str, "nil")){
    return dt_nil;
  }else{
    return -1;
  }
}

/**
 * @brief Check if token is keyword in IFJ21 language
 *
 * @param token
 *
 * @return true if the string is a keyword
 */
bool isIFJ21Keyword(Token *token) {
  int keywordsCount = 15;
  char *keywords[] = {
    "do", 
    "global", 
    "number", 
    "else", 
    "if", 
    "require", 
    "end", 
    "integer", 
    "return", 
    "function", 
    "local", 
    "string", 
    "nil", 
    "then", 
    "while"
  };
  if(token->type != t_idOrKeyword){
    return false;
  }
  for(int i = 0; i < keywordsCount; i++){
    if(strEq(token->data, keywords[i])){
      return true;
    }
  }
  return false;
}

int countDigits(int value) {
  if(value == 0) {
    return 1;
  }
  int result = 0;
  while( value != 0 ) {
    value /= 10;
    result++;
  }
  return result;
}




/**
 * @brief Writes an error message to stdout and returns back the error code
 *
 * @param errCode
 *
 * @return errCode
 */
int err(int errCode) {
  if(!errMessageWritten){
    fprintf(stderr, "ERROR: ");
    if (errCode == LEX_ERR) {
      fprintf(stderr, "Lexical analysis error ");
    } else if (errCode == SYNTAX_ERR) {
      fprintf(stderr, "Syntax error ");
    } else if (errCode == ID_DEF_ERR) {
      fprintf(stderr, "Function/variable definition error "); //TODO
    } else if (errCode == ASS_ERR) {
      fprintf(stderr, "Assignment error (types might be uncompatible) "); //TODO
    } else if (errCode == PARAM_RET_ERR) {
      fprintf(stderr, "Function parameters or return values error. Check types "); //TODO
    } else if (errCode == TYPE_EXPR_ERR) {
      fprintf(stderr, "Data types of operands in an expression are not compatible "); //TODO
    } else if (errCode == OTHER_SEM_ERR) {
      fprintf(stderr, "Semantic error "); //TODO
    } else if (errCode == NIL_ERR) {
      fprintf(stderr, "Unexpected nil "); //TODO
    } else if (errCode == DIV_BY_ZERO_ERR) {
      fprintf(stderr, "Division by zero error "); //TODO
    } else if (errCode == INTERN_ERR) {
      fprintf(stderr, "Internal error "); //TODO
    }else{
      fprintf(stderr, "Unknown error (code %d) ", errCode);
    }
    fprintf(stderr, "at (around) line %d.\n", LOCCount);
    errMessageWritten = true;
  }
  vypluj errCode;
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

int GCInit(){
  garbageCollector.pointers = malloc(sizeof(void*) * GCINITLEN);
  if(!garbageCollector.pointers){
    return err(INTERN_ERR);
  }
  garbageCollector.ptrsAllocated = GCINITLEN;
  garbageCollector.ptrsUsed = 0;
  return 0;
}

int GCInsert(void *ptr) {
  // We need to test if we have some allocated or the tests won't run well
  if(garbageCollector.ptrsAllocated != 0){
    if(garbageCollector.ptrsAllocated == garbageCollector.ptrsUsed){
      int newLen = sizeof(void*) * 2 * garbageCollector.ptrsAllocated;
      garbageCollector.pointers = realloc(garbageCollector.pointers, newLen);
      if(!(garbageCollector.pointers)){
        return err(INTERN_ERR);
      }
      garbageCollector.ptrsAllocated = newLen;
    }
    garbageCollector.pointers[garbageCollector.ptrsUsed] = ptr;
    (garbageCollector.ptrsUsed)++;
  }
  return 0;
}

void GCDelete(void *ptr) {
  for(int i = 0; i < garbageCollector.ptrsUsed; i++){
    if(ptr == garbageCollector.pointers[i]){
      garbageCollector.pointers[i] = NULL;
    }
  }
}

void GCCollect(){
  for(int i = 0; i < garbageCollector.ptrsUsed; i++){
    free(garbageCollector.pointers[i]);
  }
}

/**
 * @brief Initialize all built in functions (add them to the symtable)
 *
 * @param symtab
 *
 * @return 0 if successful, errcode otherwise
 */
int initBuiltInFunctions(STStack *symtab){
  // write
  TryCall(STInsert, symtab, "write");
  TryCall(STSetIsVariable, symtab, "write", false);
  TryCall(STSetFnDefined, symtab, "write", true);

  // reads
  TryCall(STInsert, symtab, "reads");
  TryCall(STSetName, symtab, "reads", "reads0");
  TryCall(STSetIsVariable, symtab, "reads", false);
  TryCall(STSetFnDefined, symtab, "reads", true);
  TryCall(STAppendRetType, symtab, "reads", dt_string);

  // readi
  TryCall(STInsert, symtab, "readi");
  TryCall(STSetName, symtab, "readi", "readi0");
  TryCall(STSetIsVariable, symtab, "readi", false);
  TryCall(STSetFnDefined, symtab, "readi", true);
  TryCall(STAppendRetType, symtab, "readi", dt_integer);

  // readn
  TryCall(STInsert, symtab, "readn");
  TryCall(STSetName, symtab, "readn", "readn0");
  TryCall(STSetIsVariable, symtab, "readn", false);
  TryCall(STSetFnDefined, symtab, "readn", true);
  TryCall(STAppendRetType, symtab, "readn", dt_number);

  // tointeger
  TryCall(STInsert, symtab, "tointeger");
  TryCall(STSetName, symtab, "tointeger", "tointeger0");
  TryCall(STSetIsVariable, symtab, "tointeger", false);
  TryCall(STSetFnDefined, symtab, "tointeger", true);
  TryCall(STAppendParamType, symtab, "tointeger", dt_number);
  TryCall(STAppendParamName, symtab, "tointeger", "f");
  TryCall(STAppendRetType, symtab, "tointeger", dt_integer);

  // substr
  TryCall(STInsert, symtab, "substr");
  TryCall(STSetName, symtab, "substr", "substr0");
  TryCall(STSetIsVariable, symtab, "substr", false);
  TryCall(STSetFnDefined, symtab, "substr", true);
  TryCall(STAppendParamType, symtab, "substr", dt_string);
  TryCall(STAppendParamType, symtab, "substr", dt_integer);
  TryCall(STAppendParamType, symtab, "substr", dt_integer);
  TryCall(STAppendParamName, symtab, "substr", "s");
  TryCall(STAppendParamName, symtab, "substr", "i");
  TryCall(STAppendParamName, symtab, "substr", "j");
  TryCall(STAppendRetType, symtab, "substr", dt_string);

  // ord
  TryCall(STInsert, symtab, "ord");
  TryCall(STSetName, symtab, "ord", "ord0");
  TryCall(STSetIsVariable, symtab, "ord", false);
  TryCall(STSetFnDefined, symtab, "ord", true);
  TryCall(STAppendParamType, symtab, "ord", dt_string);
  TryCall(STAppendParamType, symtab, "ord", dt_integer);
  TryCall(STAppendParamName, symtab, "ord", "s");
  TryCall(STAppendParamName, symtab, "ord", "i");
  TryCall(STAppendRetType, symtab, "ord", dt_integer);

  // chr
  TryCall(STInsert, symtab, "chr");
  TryCall(STSetName, symtab, "chr", "chr0");
  TryCall(STSetIsVariable, symtab, "chr", false);
  TryCall(STSetFnDefined, symtab, "chr", true);
  TryCall(STAppendParamType, symtab, "chr", dt_integer);
  TryCall(STAppendParamName, symtab, "chr", "i");
  TryCall(STAppendRetType, symtab, "chr", dt_string);

  return 0;
}

#endif
/* end of file misc.c */
