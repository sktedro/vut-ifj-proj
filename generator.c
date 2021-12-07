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
char *tmpVarPrefix = "?tmpvar_";
char *paramPrefix = "$param_";
char *retPrefix = "!ret_";
char *fnPrefix = "&fn_";

int tmpCounter = 0;
int labelCounter = 0;
int paramCounter = 0;
int retCounter = 0;
int writeCount = 0;

/* do not remove until var redefinition is solved
VarDefStack *varDefStack = NULL;
*/

// ----------------------------------------------------------------------------
// VARIABLES FOR MULTIPLE ASSIGMENT

int exprLabelCnt = 0;
int exprEndCnt = 0;

// ----------------------------------------------------------------------------

/*
 * Functions to help generate the definitions of variables
 */

/* do not remove until var redefinition is solved
int varDefStackInit(){
  GCMalloc(varDefStack, sizeof(VarDefStack));
}

int varDefStackPush(){
  GCMalloc(varDefStack->next, sizeof(VarDefStack));
}

void defineBufferedVars(){
  // def all vars in the string buffer in the vardef buffer top
  // Pop the vardef buffer top
}
*/

/*
 * Generating names for variables and labels
 */

/**
 * @brief Generates unique name for variables in ifj21code
 */
char *genVarName(char *name, int frame) {
  // TODO do we need that if? I think it is redundant now
  if(name[0] != '%') {
    char *newName;
    int mallocLen = strlen(varPrefix) + strlen(name) + countDigits(frame) + 1;
    GCMalloc(newName, sizeof(char) * mallocLen);
    sprintf(newName, "%s%s_%d", varPrefix, name, frame);
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
  int mallocLen = strlen(tmpVarPrefix) + countDigits(tmpCounter) + 1;
  GCMalloc(varName, sizeof(char) * mallocLen);
  sprintf(varName, "%s%d", tmpVarPrefix, tmpCounter);
  tmpCounter++;
  return varName;
}

char *genRetVarName() {
  char *retName;
  int mallocLen = strlen(retPrefix) + countDigits(retCounter) + 1;
  GCMalloc(retName, sizeof(char) * mallocLen);
  sprintf(retName, "%s%d", retPrefix, retCounter);
  retCounter++;
  return retName;
}
char *genLabelName() {
  char *varName;
  int mallocLen = strlen(labelPrefix) + countDigits(labelCounter) + 1;
  GCMalloc(varName, sizeof(char) * mallocLen);
  sprintf(varName, "%s%d", labelPrefix, labelCounter);
  labelCounter++;
  return varName;
}

char *genParamVarName() {
  char *tmp;
  int mallocLen = strlen(paramPrefix) + countDigits(paramCounter) + 1;
  GCMalloc(tmp, sizeof(char) * mallocLen);
  sprintf(tmp, "%s%d", paramPrefix, paramCounter);
  paramCounter++;
  return tmp;
}

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
 *
 * Code generation
 *
 */

/*
 * Misc
 */

void genStart() {
  printf(".IFJcode21\n\n");
}

void genComment(char *comment){
  printf("\n# %s\n", comment);
}

void genComment2(char *comment){
  printf("# %s\n\n", comment);
}

void resetParamCounter(){
  paramCounter = 0;
}

void resetRetCounter(){
  retCounter = 0;
}

/*
 * Operations with variables (def, assign, ...)
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

int genMove(char *dest, char *src){
  printf("MOVE LF@%s LF@%s\n", dest, src);
  return 0;
}

int genMoveToTF(char *dest, char *src){
  printf("MOVE TF@%s LF@%s\n", dest, src);
  return 0;
}

int genMoveToLF(char *dest, char *src){
  printf("MOVE LF@%s TF@%s\n", dest, src);
  return 0;
}

int genPassParam(char *varInTF, char *varInLF){
  printf("MOVE TF@%s LF@%s\n", varInTF, varInLF);
  return 0;
}

int genReturn(char *varInLF, char *varInTF){
  printf("MOVE LF@%s TF@%s\n", varInLF, varInTF);
  return 0;
}

int genAssignLiteral(char *name, int dataType, char *assignValue, char *frame) {

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

char *genType(char *varName){
  char *newVarName = genTmpVarDef();
  printf("TYPE %s %s\n", newVarName, varName);
  return newVarName;
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
}

void genPushFrame(){
  printf("PUSHFRAME\n");
}

void genPopframe() {
  printf("POPFRAME\n");
}

void genReturnInstruction() {
  printf("RETURN\n");
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
 * Vypíše to literál nemazať
 * 
 */
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

// --------------------------------------------------------------------------------
// FUNCTIONS FOR generating built in functions

void genSubStrFnDef() {

  printf("\nLABEL _$SUBSTR_\n");

  printf("\nPUSHFRAME\n");

  printf("\nDEFVAR LF@$$STRRET\n");
  printf("MOVE LF@$$STRRET string@\n");

  printf("\nDEFVAR LF@$$STRPAR1\n");
  printf("MOVE LF@LF@$$STRPAR1 LF@$$STRPARAM1\n");
  
  printf("\nDEFVAR LF@LF@$$STRPAR2\n");
  printf("MOVE LF@LF@$$STRPAR2 LF@$$STRPARAM2\n");

  printf("\nDEFVAR LF@LF@$$STRPAR3\n");
  printf("MOVE LF@LF@$$STRPAR3 LF@$$STRPARAM3\n");

  printf("\nJUMPIFEQ _STR$ERROR_ LF@$$STRPAR1 nil@nil\n");
  printf("JUMPIFEQ _STR$ERROR_ LF@$$STRPAR2 nil@nil\n");
  printf("JUMPIFEQ _STR$ERROR_ LF@$$STRPAR3 nil@nil\n");

  printf("\nDEFVAR LF@$$STRHELP\n");
  printf("MOVE LF@LF@LF@$$STRHELP nil@nil\n");

  printf("\nDEFVAR LF@$$STRLEN\n");
  printf("MOVE LF@LF@LF@$$STRLEN LF@$$STRPAR1\n");

  printf("\n# if (param3 < param2)\n");
  printf("LT LF@$$STRHELP LF@$$STRPAR3 LF@$$STRPAR2\n");
  printf("JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true\n");

  printf("\n# if (param2 < 1)\n");
  printf("LT LF@$$STRHELP LF@$$STRPAR2 int@1\n");
  printf("JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true\n");

  printf("\n# if (param2 > len(param1))\n");
  printf("GT LF@$$STRHELP LF@$$STRPAR2 LF@$$STRLEN\n");
  printf("JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true\n");

  printf("\n# if (param3 < 1)\n");
  printf("LT LF@$$STRHELP LF@$$STRPAR3 int@1\n");
  printf("JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true\n");

  printf("\n# if (param3 > len(param1))\n");
  printf("GT LF@$$STRHELP LF@$$STRPAR3 LF@$$STRLEN\n");
  printf("JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true\n");

  printf("\n#---------------------------------\n");

  printf("\nDEFVAR LF@$$STRINDEX\n");
  printf("MOVE LF@$$STRINDEX int@0\n");

  printf("DEFVAR LF@$$STRTMP\n");
  printf("ADD LF@$$STRPAR3 LF@$$STRPAR3 int@1\n");

  printf("\nLABEL _STR$WHILE_\n");

  printf("\n# if (param2 < param3)\n");
  printf("LT LF@$$STRHELP LF@$$STRPAR2 LF@$$STRPAR3\n");
  printf("JUMPIFEQ _STR$RET_ LF@$$STRHELP bool@false\n");

  printf("\nGETCHAR LF@$$STRTMP LF@$$STRPAR1 LF@$$STRPAR2\n");
  printf("CONCAT LF@$$STRRET LF@$$STRRET LF@$$STRTMP\n");

  printf("\nADD LF@$$STRINDEX LF@$$STRINDEX int@1\n");
  printf("ADD LF@$$STRPAR2 LF@$$STRPAR2 int@1\n");

  printf("\nJUMP _STR$WHILE_\n");

  printf("\nJUMP _STR$RET_\n");

  printf("\nLABEL _STR$EMPTY_\n");
  printf("MOVE LF@$$STRRET string@\n");
  printf("JUMP _STR$RET_\n");

  printf("\nLABEL _STR$ERROR_\n");
  printf("EXIT int@57\n");

  printf("\nLABEL _STR$RET_\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
}

int genSubStrFnParamVar(char *varName, int param) {
  
  if(param == 1) {
    printf("\nCREATEFRAME\n");
  } else if(param < 1 || param > 3) {
    vypluj err(INTERN_ERR);
  }
  
  printf("DEFVAR TF@$$STRPARAM%d\n", param);

  printf("MOVE TF@$$STRPARAM%d LF@%s\n",param ,varName);
  vypluj 0;
}

int genSubStrFnParamLiteral(Token *token, int param) {
  
  if(param == 1) {
    printf("\nCREATEFRAME\n");
  } else if(param < 1 || param > 3) {
    vypluj err(INTERN_ERR);
  }
  
  printf("DEFVAR TF@$$STRPARAM%d\n", param);
  printf("MOVE TF@$$STRPARAM%d %s@%s\n", param, getDataTypeFromInt(token), token->data);
  vypluj 0;
}

void genSubStrFnCall() {
  printf("CALL _$SUBSTR_\n");
}

void genSubStrFnRet(char *varName) {
  printf("MOVE LF@%s TF@$$STRRET\n", varName);
}

// --------------------------------------------------------------------------------
// FUNCTIONS FOR MULTIPLE ASSIGMENT

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
