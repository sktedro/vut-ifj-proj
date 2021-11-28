/*
 * Code generator
 */

#ifndef GENERATOR_C
#define GENERATOR_C

#include "generator.h"

extern int ret;

int tmpCounter = 0;
int labelCounter = 0;

int digits(int value) {

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

// TODO %s ain't enough to reference a variable. Don't we need '?F@'
// everywhere?

/**
 * @brief Generates unique name for variables in ifj21code
 *
 *
 *
 * */
char *genName(char *name, int frame) {

  if(name[0] != '%') {
    char *frameNum = malloc(sizeof(char) * (digits(frame) + 1));
    CondGCInsert(frameNum);
    sprintf(frameNum, "%d", frame);
    char *newName = malloc(sizeof(char) * (strlen(name) + strlen(frameNum) + 1));
    CondGCInsert(newName);
    memcpy(newName, name, strlen(name));
    memcpy(&newName[strlen(name)], frameNum, strlen(frameNum) + 1);

    return newName;
  }

  return name;
}

/**
 * @brief Converts ifj21 string to ifj21code string
 * 
 * @param ifj21 string 
 * @return ifj21code string
 */
char *stringConvert(char *string) {
  char *newString = malloc(sizeof(char) * (strlen(string)*3));
  CondGCInsert(newString);
  int asciiValue = 0;
  int digitsTmp = 0;
  int k = 0;
  
  for(int i=0; i<(int) strlen(string); i++) {

    if((*string >= 'a' && *string <= 'z') || (*string >= 'A' && *string <= 'Z') || (*string >= '0' && *string <= '9')) {

        newString[k] = string[i];
    } else {
        char arr1[5];
        char arr2[5];

        arr1[0] = '\0';
        arr2[0] = '\0';
        asciiValue = (int) string[i];
        digitsTmp = digits(asciiValue);
        
        arr1[0] = '\\';
        if(digitsTmp == 2) {
          arr1[1] = 0;
        }

        sprintf(arr2, "%d", asciiValue);
        strcat(arr1, arr2);
        strcat(newString, arr1);
        k +=4;
    }
  }

  return newString;
}


/**
 * @brief Generates temporary variable for expression
 * 
 * @return generated temporary variable 
 */
char *genTmpVar() {
  char *varName = malloc(sizeof(char) * 10);
  CondGCInsert(varName);
  sprintf(varName, "%s%d", "%tmp", tmpCounter);
  tmpCounter++;

  printf("DEFVAR LF@%s\n", varName);
  return varName;
}

char *genLabelName() {
  char *varName = malloc(sizeof(char) * 10);
  CondGCInsert(varName);
  sprintf(varName, "%s%d", "%label", labelCounter);
  labelCounter++;
  
  return varName;
}

char *genTmpVarDef() {
  return genTmpVar();
}
/**
 * identifikátor, frame number
 * a = 1
 * a je v globále
 * v ifjcode bude názov a0
 */

void genVarDef(char *name, int frame) {
  printf("DEFVAR LF@%s\n", genName(name, frame));
}

/**
 * identifikátor, dátový typ, priradzovaná hodnota
 */
int genVarAssign(char *name, int dataType, char *assignValue) {
  if(dataType == dt_integer) {
    // Convert to int and check if the conversion was successful
    char *tolptr = NULL;
    int val = (int)strtol(assignValue, &tolptr, 10);
    // If the assignValue contains an invalid number (shouldn't happen since we
    // are checking it in lexical analysis)
    if(tolptr[0]){
      return err(SYNTAX_ERR); // TODO spravny errcode?
    }
    printf("MOVE LF@%s int@%d\n", name, val);
  
  } else if(dataType == dt_number) {
    // Convert to double and check if the conversion was successful
    char *todptr = NULL;
    double val = strtod(assignValue, &todptr);
    // If the assignValue contains an invalid number (shouldn't happen)
    if(todptr[0]){
      return err(SYNTAX_ERR); // TODO spravny errcode?
    }
    printf("MOVE LF@%s float@%a\n", name, val);
  
  } else if(dataType == dt_string) {
    printf("MOVE LF@%s string@%s\n", name, assignValue);
  
  } else if(strcmp(assignValue, "nil") == 0) {
    printf("MOVE LF@%s nil@nil\n", name);
  
  } else if(strcmp(assignValue, "readi") == 0) {
    printf("READ LF@%s int\n", name);
  
  } else if(strcmp(assignValue, "readn") == 0) {
    printf("READ LF@%s float\n", name);
  
  } else if(strcmp(assignValue, "reads") == 0) {
    printf("READ LF@%s string\n", name);
  
  } else {
    return 1; // TODO errcode?
  }

  return 0;
}


/**
 * volane bude z precedenčnej analýzy (iba)
 * potrebujeme asi tri symbol stack elementy - ľavý op, pravý op a operátor
 */
char *genBinaryOperationAdd(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("ADD LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}


char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("SUB LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("MUL LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("DIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("IDIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("CONCAT LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genConvertFloatToInt(SStackElem *src) {
  char *dest = genTmpVar();
  printf("FLOAT2INT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genConvertIntToFloat(SStackElem *src) {
  char *dest = genTmpVar();
  printf("INT2FLOAT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genUnaryOperation(SStackElem *src) {
  char *dest = genTmpVar();
  printf("STRLEN LF@%s LF@%s\n", dest, src->data);
  return dest;
}

void genFnCall(char *name) {
  printf("CALL %s0\n",name);
}

void genFnDef(char *name) {
  printf("LABEL %s0\n", name);
  printf("PUSHFRAME\n");
}

void genFnDefRet() {
  printf("POPFRAME\n");
  printf("RETURN\n");
}


void genWrite(char *name, int frame) {
  if(frame == 0) {
    printf("WRITE GF@%s\n", genName(name, frame));
  } else {
    printf("WRITE TF@%s\n", genName(name, frame));
  }
}

char *genLower(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVar();
  printf("LT LF@%s LF@%s LF@%s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genGreater(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVar();
  printf("GT LF@%s LF@%s LF@%s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genEqual(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVar();
  printf("EQ LF@%s LF@%s LF@%s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genNot(SStackElem *src) {
  char *tmp = genTmpVar();
  printf("NOT LF@%s LF@%s\n", tmp, src->data);
  return tmp;
}

char *genJumpIfNeq(char *tmp) {
  char *label = genLabelName();
  printf("JUMPIFNEQ label%s LF@%s bool@true\n", label ,tmp);
  return label;
}

char *genJumpIfEq(char *tmp) {
  char *label = genLabelName();
  printf("JUMPIFEQ label%s LF@%s bool@true\n", label ,tmp);
  return label;
}

void genLabel(char *labelName) {
  printf("LABEL %s\n", labelName);
}

void genStart() {
  printf(".IFJcode21\n\n");
}

/*
 * MOV
 * CREATEFRAME
 * PUSHFRAME
 * POPFRAME
 * DEFVAR
 * CALL
 * RETURN
 *
 * PUSHS
 * POPS
 * CLEARS
 *
 * ADD y
 * SUB y
 * MUL y
 * DIV y
 * IDIV y
 * LT y
 * GT y
 * EQ y
 * LTS x
 * GTS x
 * EQS x
 * AND
 * OR
 * NOT
 * ANDS
 * ORS
 * NOTS
 * INT2FLOAT y
 * FLOAT2INT y
 * INT2CHAR y
 * STR2INT y
 *
 * READ
 * WRITE
 *
 * CONCAT y
 * STRLEN
 * GETCHAR
 * SETCHAR
 *
 * TYPE
 *
 * LABEL
 * JUMP
 * JUMIFEQ y
 * JUMPIFNEQ y
 * JUMPIFEQS y
 * JUMPIFNEQS y
 * EXIT
 *
 * BREAK x
 * DPRINT
 */

#endif
/* end of file generator.c */
