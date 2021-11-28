/*
 * Enumerators, structure definitions and miscellaneous functions
 */

#ifndef MISC_C
#define MISC_C

#include "misc.h"

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
      fprintf(stderr, "Unknown error ");
    }
    fprintf(stderr, "at (around) line %d.\n", LOCCount);
    errMessageWritten = true;
  }
  vypluj errCode;
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
  if(garbageCollector.ptrsAllocated == garbageCollector.ptrsUsed){
    int newLen = 2 * sizeof(void*) * garbageCollector.ptrsAllocated;
    garbageCollector.pointers = realloc(garbageCollector.pointers, newLen);
    if(!garbageCollector.pointers){
      return err(INTERN_ERR);
    }
    garbageCollector.ptrsAllocated = newLen;
  }
  garbageCollector.pointers[garbageCollector.ptrsUsed] = ptr;
  (garbageCollector.ptrsUsed)++;
  return 0;
}

void GCDelete(void *ptr) {
  for(int i = 0; i < garbageCollector.ptrsUsed; i++){
    if(ptr == garbageCollector.pointers[i]){
      garbageCollector.pointers[i] = NULL;
      return;
    }
  }
}

void GCCollect(){
  for(int i = 0; i < garbageCollector.ptrsUsed; i++){
    free(garbageCollector.pointers[i]);
  }
}

#endif
/* end of file misc.c */
