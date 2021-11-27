/*
 * Code generator
 */

#include "generator.h"

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
    sprintf(frameNum, "%d", frame);
    char *newName = malloc(sizeof(char) * (strlen(name) + strlen(frameNum) + 1));
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
  char *ret = malloc(sizeof(char) * 10);
  sprintf(ret, "%s%d", "%tmp", tmpCounter);
  tmpCounter++;

  printf("DEFVAR TF@%s\n", ret);
  return ret;
}

char *genLabelName() {
  char *ret = malloc(sizeof(char) * 10);
  sprintf(ret, "%s%d", "%label", labelCounter);
  labelCounter++;
  
  return ret;
}

/**
 * identifikátor, frame number
 * a = 1
 * a je v globále
 * v ifjcode bude názov a0
 */
void genVarDef(char *name, int frame) {
  printf("DEFVAR LF@%s\n", genName(name, frame));
  printf("MOVE LF@%s nil@nil\n", genName(name, frame));
}

/**
 * identifikátor, frame number, priradzovaná hodnota
 */
int genVarAssign(Token *token, int frameNumber, char *assignValue) {

  if(token->type == t_int) {
    printf("MOVE TF@%s int@%s\n", genName(token->data, frameNumber), assignValue);
  
  } else if(token->type == t_num) {
    printf("MOVE TF@%s float@%s\n", genName(token->data, frameNumber), assignValue);
  
  } else if(token->type == t_sciNum) {
    printf("MOVE TF@%s float@%a\n", genName(token->data, frameNumber), atof(assignValue));
  
  } else if(token->type == t_str) {
    printf("MOVE TF@%s int@%s\n", genName(token->data, frameNumber), assignValue);
  
  } else if(token->type == t_idOrKeyword && strcmp(assignValue, "nil") == 0) {
    printf("MOVE TF@%s nil@nil\n", genName(token->data, frameNumber));
  
  } else if(token->type == t_idOrKeyword && strcmp(assignValue, "readi") == 0) {
    printf("READ TF@%s int\n", genName(token->data, frameNumber));
  
  } else if(token->type == t_idOrKeyword && strcmp(assignValue, "readn") == 0) {
    printf("READ TF@%s float\n", genName(token->data, frameNumber));
  
  } else if(token->type == t_idOrKeyword && strcmp(assignValue, "reads") == 0) {
    printf("READ TF@%s string\n", genName(token->data, frameNumber));
  
  } else {
    // TODO add error code - vypisujú sa tu normálne code errors alebo nejaké špeciálne pre ifjcode21? 
    return 1;
  }

  return 0;
}


/**
 * volane bude z precedenčnej analýzy (iba)
 * potrebujeme asi tri symbol stack elementy - ľavý op, pravý op a operátor
 */
char *genBinaryOperationAdd(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("ADD %s %s %s\n", dest, src1->data, src2->data);
  return dest;
}


char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("SUB %s %s %s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("MUL %s %s %s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("DIV %s %s %s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  printf("DIV %s %s %s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVar();
  char *op1 = src1->data, *op2 = src2->data;

  // If it is not an ID, put " around it
  if(src1 && !src1->isId){
    op1 = malloc(sizeof(char) * (strlen(src1->data) + 2 + 1));
    op1 = strcat(op1, "\"");
    op1 = strcat(op1, src1->data);
    op1 = strcat(op1, "\"");
  }
  if(src2 && !src2->isId){
    op2 = malloc(sizeof(char) * (strlen(src2->data) + 2 + 1));
    op2 = strcat(op2, "\"");
    op2 = strcat(op2, src2->data);
    op2 = strcat(op2, "\"");
  }
  printf("CONCAT %s %s %s\n", dest, op1, op2);
  return dest;
}

char *genConvertFloatToInt(SStackElem *src) {
  char *dest = genTmpVar();
  printf("FLOAT2INT %s %s\n", dest, src->data);
  return dest;
}

char *genConvertIntToFloat(SStackElem *src) {
  char *dest = genTmpVar();
  printf("INT2FLOAT %s %s\n", dest, src->data);
  return dest;
}

char *genUnaryOperation(SStackElem *src) {
  char *dest = genTmpVar();
  char *op = src->data;
  // If it is not an ID, put " around it
  // TODO make it a function?
  if(src && !src->isId){
    op = malloc(sizeof(char) * (strlen(src->data) + 2 + 1));
    op = strcat(op, "\"");
    op = strcat(op, src->data);
    op = strcat(op, "\"");
  }
  printf("STRLEN %s %s\n", dest, op);
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
  printf("LT %s %s %s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genGreater(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVar();
  printf("GT %s %s %s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genEqual(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVar();
  printf("EQ %s %s %s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genNot(SStackElem *src) {
  char *tmp = genTmpVar();
  printf("NOT %s %s\n", tmp, src->data);
  return tmp;
}

char *genJumpIfNeq(char *tmp) {
  char *label = genLabelName();
  printf("JUMPIFNEQ label%s %s bool@true\n", label ,tmp);
  return label;
}

char *genJumpIfEq(char *tmp) {
  char *label = genLabelName();
  printf("JUMPIFEQ label%s %s bool@true\n", label ,tmp);
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
