/*
 * Code generator
 */

#ifndef GENERATOR_C
#define GENERATOR_C

#include "generator.h"

extern int ret;
extern GarbageCollector garbageCollector;

char *labelPrefix = "_label_";
char *varPrefix = "%var_";
char *tmpvarPrefix = "?tmpvar_";
char *paramPrefix = "$param_";
char *retPrefix = "!ret_";
char *fnPrefix = "&fn_";

int tmpCounter = 0;
int labelCounter = 0;
int paramCnt = 0;
int retCnt = 0;
int writeCount = 0;

// ----------------------------------------------------------------------------
// VARIABLES FOR MULTIPLE ASSIGMENT

int exprLabelCnt = 0;
int exprEndCnt = 0;

// ----------------------------------------------------------------------------

/*
 * Helper functions
 */

/**
 * @brief Generates unique name for variables in ifj21code
 *
 */
// TODO rename to getVarName
char *genName(char *name, int frame) {

  if(name[0] != '%') {
    char *frameNum;
    GCMalloc(frameNum, sizeof(char) * (countDigits(frame) + 2));
    sprintf(frameNum, "_%d", frame);
    char *newName;
    GCMalloc(newName, sizeof(char) * (strlen(name) + strlen(frameNum) + 1));
    memcpy(newName, name, strlen(name));
    memcpy(&newName[strlen(name)], frameNum, strlen(frameNum) + 1);
    return newName;
  }
  return name;
}

/**
 * @brief Generates temporary variable for expression
 * 
 * @return generated temporary variable 
 */
char *genTmpVarName() {
  char *varName;
  GCMalloc(varName, sizeof(char) * 10);
  sprintf(varName, "%s%d", tmpvarPrefix, tmpCounter);
  tmpCounter++;
  return varName;
}

char *genRetName() {
  char *retName;
  GCMalloc(retName, sizeof(char) * 10);
  sprintf(retName, "%s%d", retPrefix, retCnt);
  retCnt++;
  return retName;
}
char *genLabelName() {
  char *varName;
  GCMalloc(varName, sizeof(char) * 10);
  sprintf(varName, "%s%d", labelPrefix, labelCounter);
  labelCounter++;
  return varName;
}

char *genParamName() {
  char *tmp;
  GCMalloc(tmp, sizeof(char) * 100);
  sprintf(tmp, "%s%d", paramPrefix, paramCnt);
  /** paramCnt++; */ // TODO niekedy to nechceme inkrementovať...
  // Teda možno hej, ale v parseri po argumentoch fn call aj po parametroch vo
  // funkcii treba asi paramCnt vynulovať
  return tmp;
}

// TODO use this instead of hardcoding it in every printf
// TODO toto sa nikde nepoužíva
/*char *genVarName() {
  //genVarDefLF(name); zakomentované lebo nešlo preloži
  char *tmp;
  GCMalloc(tmp, sizeof(char) * 100);
  sprintf(tmp, "%s%d", varPrefix, retCnt);
  retCnt++;
  return tmp;
} Alex vyrieši či treba alebo nie*/ 

/*
 * Other helper functions
 */

char *getDataTypeFromInt(Token *token) {
  char *tmp;

  GCMalloc(tmp, sizeof(char) * 50);

  if(token->type == t_str) {
    strcpy(tmp, "string");
  } else if(token->type == t_num) {
    strcpy(tmp, "float");
  } else if(token->type == t_int) {
    strcpy(tmp, "int");
  }  else if(strcmp(token->data, "nil") == 0) {
    strcpy(tmp, "nil");
  } else {
    vypluj NULL;
  }

  vypluj tmp;
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
        digitsTmp = countDigits(asciiValue);
        
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

/*
 * Code generation
 */

void genComment(char *comment){
  printf("\n# %s\n", comment);
}

void genComment2(char *comment){
  printf("# %s\n\n", comment);
}

void genStart() {
  printf(".IFJcode21\n\n");
}

/*
 * Variable definition and assignment
 */

void genVarDefLF(char *name) {
  printf("DEFVAR LF@%s\n", name);
}

void genVarDefTF(char *name) {
  printf("DEFVAR TF@%s\n", name);
}

char *genTmpVarDef() {
  char *name = genTmpVarName();
  genVarDefLF(name);
  return name;
}

// TODO rename to genAssignLiteral
int genVarAssign(char *name, int dataType, char *assignValue, char *frame) {

  if(dataType == dt_integer) {
    // Convert to int and check if the conversion was successful
    char *tolptr = NULL;
    int val = (int)strtol(assignValue, &tolptr, 10);
    // If the assignValue contains an invalid number (shouldn't happen since we
    // are checking it in lexical analysis)
    if(tolptr[0]){
      return ERR(INTERN_ERR);
    }
    printf("MOVE %s@%s int@%d\n",frame, name, val);
  
  } else if(dataType == dt_number) {
    // Convert to double and check if the conversion was successful
    char *todptr = NULL;
    double val = strtod(assignValue, &todptr);
    // If the assignValue contains an invalid number (shouldn't happen)
    if(todptr[0]){
      return ERR(INTERN_ERR);
    }
    printf("MOVE %s@%s float@%a\n",frame, name, val);
  
  } else if(dataType == dt_string) {
    printf("MOVE %s@%s string@%s\n",frame, name, assignValue);
  
  } else if(strcmp(assignValue, "nil") == 0) {
    printf("MOVE %s@%s nil@nil\n",frame, name);
  
  } else if(strcmp(assignValue, "readi") == 0) {
    printf("READ %s@%s int\n",frame, name);
  
  } else if(strcmp(assignValue, "readn") == 0) {
    printf("READ %s@%s float\n",frame, name);
  
  } else if(strcmp(assignValue, "reads") == 0) {
    printf("READ %s@%s string\n",frame, name);
  
  } else if(dataType == -1) {
    // TODO change TF to LF if expression variable is in local frame
    printf("MOVE %s@%s TF@%s",frame, name, assignValue);
  } else {
    return ERR(-1); // TODO errcode??? huh
  }

  return 0;
}


// TODO swap the parameters here (next line)
int genPassParam(char *varInLF, char *varInTF){
  printf("MOVE TF@%s LF@%s\n", varInTF, varInLF);
  return 0;
}

int genReturn(char *varInLF, char *varInTF){
  printf("MOVE LF@%s TF@%s\n", varInLF, varInTF);
  return 0;
}

int genMove(char *dest, char *src){
  printf("MOVE LF@%s LF@%s\n", dest, src);
  return 0;
}

/*
 * Functions and frame operations
 */

// After TF is created and all parameters are moved, push the TF to the stack
// and call the function
void genFnCall(char *fnName) {
  printf("CALL %s0\n", fnName);
}

void genFnDef(char *name) {
  printf("LABEL %s0\n\n", name);
}

// Create a TF
void genFnCallInit(){
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");
}

void genPopframe() {
  printf("POPFRAME\n");
}

void genReturnInstruction() {
  printf("RETURN\n");
}



void genWriteLiteral(Token *token, char *frame) {
  char *string;
  char *dataType = getDataTypeFromInt(token);

  if(token->type == t_str) {
    string = stringConvert(token->data);

    printf("\n");
    printf("DEFVAR %s@$W%d\n",frame, writeCount);
    printf("MOVE %s@$W%d %s@%s\n",frame, writeCount, dataType, string);
    printf("WRITE %s@$W%d\n", frame, writeCount);
    writeCount++;
  } else {
    printf("\n");
    printf("DEFVAR %s@W%d\n",frame, writeCount);
    printf("MOVE %s@W%d %s@%s\n",frame, writeCount, dataType, token->data);
    printf("WRITE %s@W%d\n", frame, writeCount);
    writeCount++;
  }

}

void genWrite(char *name) {
/** void genWriteVariable(char *name, char *frame) { */
  /**
    * LOG("FRAME : %s", frame);
    * printf("\n");
    * printf("DEFVAR %s@$W%d\n", frame, writeCount);
    * printf("MOVE %s@$W%d %s@%s\n", frame, writeCount, frame, name);
    * printf("WRITE %s@$W%d\n", frame, writeCount);
    * writeCount++;
    */
  // tedro: nestačí to takto?
  printf("WRITE %s\n", name);
}

char *genType(char *varName){
  char *newVarName = genTmpVarDef();
  printf("TYPE %s %s\n", newVarName, varName);
  return newVarName;
}




/*
 * Jumps and labels
 */

void genUnconditionalJump(char *labelName){
  printf("JUMP %s\n", labelName);
}

// Jump if the var is nil or false
void genJumpIfFalse(char *label, char *varName) {
  genConditionalJump(label, varName, false);
}

// Jump if the var is true or not nil
void genJumpIfTrue(char *label, char *varName) {
  genConditionalJump(label, varName, true);
}

void genLabel(char *labelName) {
  printf("LABEL %s\n", labelName);
}

// Basically an IF condition
void genConditionalJump(char *label, char *varName, bool condition){
  /*
   * The code (to jump conditionally):
   *
   * %type = type(%result)
   *
   * if(%type == "nil") jump _exprIsFalse
   * if(%type != "bool") jump _exprIsTrue
   * # We know it is a boolean:
   * if(%result == true) jump _exprIsTrue
   * jump _exprIsFalse
   *
   * The rest depends on the 'condition' parameter in this function
   */

  genComment("If condition start");
  char *falseLabelName = "exprIsFalse";
  char *trueLabelName = "exprIsTrue";
  char *noJumpLabelName = "noJump";

  // typeVarName = type(varName)
  char *typeVarName = genType(varName);

  // if(typeVarName == "nil") jump falseLabelName
  printf("JUMPIFEQ %s %s \"nil\"\n", falseLabelName, typeVarName);

  // if(typeVarName != "bool") jump trueLabelName
  printf("JUMPIFNEQ %s %s \"bool\"\n", trueLabelName, typeVarName);

  // if(varName == false) jump falseLabelName
  printf("JUMPIFEQ %s %s false\n", falseLabelName, varName);
  
  // trueLabelName:
  genLabel(trueLabelName);

  if(condition == true){
    // jump label
    genUnconditionalJump(label);
  }else{
    // jump noJumpLabelName:
    genUnconditionalJump(noJumpLabelName);
  }

  // falseLabelName:
  genLabel(falseLabelName);
  
  if(condition == false){
    genUnconditionalJump(label);
  }else{
    genUnconditionalJump(noJumpLabelName);
  }
  
  genLabel(noJumpLabelName);

  genComment2("If condition done");
}

/*
 * Operations to be called by the shift-reduce parser
 */

char *genBinaryOperationAdd(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarDef();
  printf("ADD LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarDef();
  printf("SUB LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarDef();
  printf("MUL LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarDef();
  printf("DIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarDef();
  printf("IDIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarDef();
  printf("CONCAT LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genConvertFloatToInt(SStackElem *src) {
  char *dest = genTmpVarDef();
  printf("FLOAT2INT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genConvertIntToFloat(SStackElem *src) {
  char *dest = genTmpVarDef();
  printf("INT2FLOAT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genStrlen(SStackElem *src) {
  char *dest = genTmpVarDef();
  printf("STRLEN LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genLower(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVarDef();
  printf("LT LF@%s LF@%s LF@%s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genGreater(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVarDef();
  printf("GT LF@%s LF@%s LF@%s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genEqual(SStackElem *element1, SStackElem *element2) {
  char *tmp = genTmpVarDef();
  printf("EQ LF@%s LF@%s LF@%s\n", tmp, element1->data, element2->data);
  return tmp;
}

char *genNot(SStackElem *src) {
  char *tmp = genTmpVarDef();
  printf("NOT LF@%s LF@%s\n", tmp, src->data);
  return tmp;
}


/*
 * No idea. Remove this??? TODO
 */

int genReadFunction(char *varName, char *builtInFnName) {
  
  if(strcmp(builtInFnName, "readi") == 0) {
    printf("READ LF@%s int\n", varName);
  } else if(strcmp(builtInFnName, "readn") == 0) {
    printf("READ LF@%s float\n", varName);
  } else if(strcmp(builtInFnName, "reads") == 0) {
    printf("READ LF@%s string\n", varName);
  } else {
    vypluj ERR(SYNTAX_ERR);
  }

  vypluj 0;
}

int genSubstrFunction(char *target, Token *string, double start, int end, int frame) {

  /*printf("LABEL: $substr");

  printf("PUSHFRAME");

  printf("DEFVAR LF@$substrRET");
  printf("MOVE LF@$substrRET nil@nil");

  printf("DEFVAR LF@$in1");
  printf("MOVE LF@$in1 $STRING");

  printf("DEFVAR LF@$in2");
  printf("MOVE LF@$in2 $START");

  printf("DEFVAR LF@$in3");
  printf("MOVE LF@$in3 $END");*/

  return 0;
}

int genSubstrFunstionCall(Token *string, Token *start, Token *end) {
  
  return 0;
}



/*
 * Functions for the multiassignment
 */

void genExprLabel(char *name) {
  printf("LABEL %s%s\n", labelPrefix, name);
}

/*void genExprJump(char *label) {
  printf("JUMP %s\n", label);
} robí to isté ako genUnconditionalJump, pre istotu to tu nechávam zatiaľ*/

char *getExprLabelName(int num) {
  char *tmp;
  GCMalloc(tmp, sizeof(char) * 30);
  sprintf(tmp, "%sEXPR%d", labelPrefix, exprEndCnt);

  exprLabelCnt++;

  return tmp;
}

char *getExprEndName() {
  char *tmp;
  GCMalloc(tmp, sizeof(char) * 30);
  sprintf(tmp, "%sEXPREND%d", labelPrefix, exprEndCnt);

  exprEndCnt++;

  return tmp;
}

void genExprFirst(AssignElement *element) {
  AssignElement *tmp = AListGetLast(element);

  genUnconditionalJump(tmp->label);
  genExprLabel(element->label);

  element->generated = true;
}

void genExprLast(AssignElement *element) {
  AssignElement *tmp = AListGetLast(element);

  genUnconditionalJump(element->label);
  genExprLabel(tmp->label);

  tmp->generated = true;
}

void genExprSecond(AssignElement *element) {

  genUnconditionalJump(element->end);
  genExprLabel(element->next->label);

  element->next->generated = true;

}

void genExpr(AssignElement *element) {

  genUnconditionalJump(element->prev->prev->label);
  genExprLabel(element->label);
  element->generated = true;
}

void genExprEnd(AssignElement *element) {
  AssignElement *last = AListGetLast(element);

  genUnconditionalJump(last->prev->label);
  genExprLabel(element->end);
  
}




/*
a, b, c = a+10, b+10, c+10

jump treti
prvy:
    ADD A A 10
jump end
druhy:
    ADD B B 10
    jump prvy
treti:
    ADD C C 10
jump druhy

end:

*/

#endif
/* end of file generator.c */
