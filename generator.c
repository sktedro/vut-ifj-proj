/*
 * Code generator
 */

#ifndef GENERATOR_C
#define GENERATOR_C

#include "generator.h"

// TODO use this instead of hardcoding it in every printf
char labelPrefix = '$';
char varPrefix = '%';

extern int ret;
extern GarbageCollector garbageCollector;

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
    char *frameNum;
    GCMalloc(frameNum, sizeof(char) * (digits(frame) + 1));
    sprintf(frameNum, "%d", frame);
    char *newName;
    GCMalloc(newName, sizeof(char) * (strlen(name) + strlen(frameNum) + 1));
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
  char *newString;
  GCMalloc(newString, sizeof(char) * (strlen(string)*3));
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
  char *varName;
  GCMalloc(varName, sizeof(char) * 10);
  sprintf(varName, "%s%d", "%tmp", tmpCounter);
  tmpCounter++;

  printf("DEFVAR LF@%s\n", varName);
  return varName;
}

char *genLabelName() {
  char *varName;
  GCMalloc(varName, sizeof(char) * 10);
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

void genJumpIfFalse(char *labelName, char *varName){
  // TODO JUMPIFNEQ to bool literal
}

void genUnconditionalJump(char *labelName){
  printf("JUMP %s\n", labelName);
}

void genVarDef(char *name, int frame) {
  name = genName(name, frame);
  printf("DEFVAR LF@%s\n", name);
}

int genPassParam(char *varInLF, char *varInTF, int frameOfLF){
  varInTF = genName(varInTF, frameOfLF + 1);
  varInLF = genName(varInLF, frameOfLF);
  printf("MOVE TF@%s LF@%s\n", varInTF, varInLF);
  return 0;
}

int genReturn(char *varInTF, char *varInLF, int frameOfTF){
  varInTF = genName(varInTF, frameOfTF);
  varInLF = genName(varInLF, frameOfTF - 1);
  printf("MOVE LF@%s TF@%s\n", varInLF, varInTF);
  return 0;
}

int genMove(char *dest, char *src, int frame){
  dest = genName(dest, frame);
  src = genName(src, frame);
  printf("MOVE LF@%s LF@%s\n", dest, src);
  return 0;
}

/**
 * identifikátor, dátový typ, priradzovaná hodnota
 */
int genVarAssign(char *name, int frame, int dataType, char *assignValue) {
  name = genName(name, frame);
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
  
  } else if(dataType == -1) {
    // TODO change TF to LF if expression variable is in local frame
    printf("MOVE LF@%s TF@%s", name, assignValue);
  } else {
    return 1; // TODO errcode?
  }

  return 0;
}

int genReadFunction(char *varName, char *builtInFnName, int frame) {
  
  if(strcmp(builtInFnName, "readi") == 0) {
      printf("READ LF@%s int\n", genName(varName, frame));
  } else if(strcmp(builtInFnName, "readn") == 0) {
      printf("READ LF@%s float\n", genName(varName, frame));
  } else if(strcmp(builtInFnName, "reads") == 0) {
      printf("READ LF@%s string\n", genName(varName, frame));
  } else {
    vypluj err(1); // TODO INTERNAL ERROR
  }

  vypluj 0;
}

int genSubstrFunction(char *target, Token *string, double start, int end, int frame) {

  printf("LABEL: $substr");

  printf("PUSHFRAME");

  printf("DEFVAR LF@$substrRET");
  printf("MOVE LF@$substrRET nil@nil");

  printf("DEFVAR LF@$in1");
  printf("MOVE LF@$in1 $STRING");

  printf("DEFVAR LF@$in2");
  printf("MOVE LF@$in2 $START");

  printf("DEFVAR LF@$in3");
  printf("MOVE LF@$in3 $END");

  //TODO porovnanie ak nil => err 8, ak start>end alebo 1 > s a e > strlen => prázdny string
  //in2 je start in3 je end
  printf("LT $nejakyBool $in2 $in3"); //ak start<end tak je dobre a ideme na GETCHAR, inak prázdny reťazec

  printf("JUMPIFEQ LOOP $nejakybool TRUE"); //start<end a v náveští LOOP vypisujem GETCHAR

  printf(""); //prázdny retazec lebo end < start

  printf("LT $nejakyBoo $in2 1"); //ak start<1


  printf("EQ $nejakyBool $in2 $in3"); // ak sú rovnaké tak jeden char

  printf("JUMPIFEQ LOOP $nejakybool TRUE"); 




  //TODO ak je $in2 alebo $in3 nil tak err 8

  //TODO loop from start to end for getchar
  
  printf("GETCHAR $tmpPremenna $in1 $in2");
  

  return 0;
}

int genSubstrFunstionCall(Token *string, Token *start, Token *end) {
  
}

// Create a TF
void genFnCallInit(){
  printf("CREATEFRAME\n");
}

// After TF is created and all parameters are moved, push the TF to the stack
// and call the function
void genFnCall(char *fnName) {
  printf("PUSHFRAME\n");
  printf("CALL %s0\n", fnName);
}

void genFnDef(char *name) {
  printf("LABEL %s0\n", name);
}

void genFnDefRet() {
  printf("RETURN\n");
}


void genWrite(char *name, int frame) {
  if(frame == 0) {
    printf("WRITE GF@%s\n", genName(name, frame));
  } else {
    printf("WRITE TF@%s\n", genName(name, frame));
  }
}

// Jump if the var is nil or false
void genJumpIfFalse(char *label, char *varName) {
  // TODO
}

// Jump if the var is true or not nil
void genJumpIfTrue(char *label, char *varName) {
  // TODO
}

void genLabel(char *labelName) {
  printf("LABEL %s\n", labelName);
}

void genStart() {
  printf(".IFJcode21\n\n");
}

/*
 * Functions that are to be called by the shift-reduce parser only
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

char *genStrlen(SStackElem *src) {
  char *dest = genTmpVar();
  printf("STRLEN LF@%s LF@%s\n", dest, src->data);
  return dest;
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
