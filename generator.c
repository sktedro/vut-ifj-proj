/**
 * VUT FIT - IFJ 2021
 *
 * @file generator.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef GENERATOR_C
#define GENERATOR_C

#include "generator.h"

extern int ret;

extern GarbageCollector garbageCollector;

extern const char *reads;
extern const char *readi;
extern const char *readn;
extern const char *write;
extern const char *tointeger;
extern const char *substr;
extern const char *ord;
extern const char *chr;

const char *labelPrefix = "*label_";
const char *varPrefix = "%var_";
const char *tmpVarPrefix = "?tmpvar_";
const char *paramPrefix = "$param_";
const char *retPrefix = "!ret_";
const char *fnPrefix = "&fn_";

extern StringBuffer *varDefBuff;

int tmpCounter = 0;
int labelCounter = 0;
int paramCounter = 0;
int retCounter = 0;
int writeCount = 0;

// Return x nils
int genNilsReturn(int amount){
  for(int i = 0; i < amount; i++){
    // Generate and define the retArgName
    char *retArgName = genRetVarName("");
    genVarDefLF(retArgName);
    // Pass nil to retVarName
    genAssignLiteral(retArgName, dt_nil, "nil", "LF");
  }
  return 0;
}

/*
 *
 * Generating names for variables and labels
 *
 */

/**
 * @brief Generates unique name for variables in ifj21code
 */
char *genVarName(char *name, int frame) {
  char *newName;
  int mallocLen = strlen(varPrefix) + strlen(name) + countDigits(frame) + 2;
  GCMalloc(newName, sizeof(char) * mallocLen);
  sprintf(newName, "%s%s_%d", varPrefix, name, frame);
  return newName;
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
  condAppendToStringBuff(varName);
  return varName;
}

char *genRetVarName(char *baseName) {
  char *retName;
  int mallocLen = strlen(retPrefix) + strlen(baseName) + countDigits(retCounter) + 1;
  GCMalloc(retName, sizeof(char) * mallocLen);
  sprintf(retName, "%s%d", retPrefix, retCounter);
  retCounter++;
  return retName;
}

char *genLabelName(char *baseName) {
  char *varName;
  int mallocLen = strlen(labelPrefix) + strlen(baseName) + countDigits(labelCounter) + 2;
  GCMalloc(varName, sizeof(char) * mallocLen);
  sprintf(varName, "%s%s_%d", labelPrefix, baseName, labelCounter);
  labelCounter++;
  return varName;
}

char *genParamVarName(char *baseName) {
  char *tmp;
  int mallocLen = strlen(paramPrefix) + strlen(baseName) + countDigits(paramCounter) + 2;
  GCMalloc(tmp, sizeof(char) * mallocLen);
  sprintf(tmp, "%s%s_%d", paramPrefix, baseName, paramCounter);
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
int stringConvert(char **destPtr, char *string) {
  CharBuffer *newString;
  CharBuffer *tempBuffer;
  TryCall(charBufInit, &newString);
  TryCall(charBufInit, &tempBuffer);

  int i = 0;
  while(string[i] != '\0'){
    LOG("CHAR: %c", string[i]);
    // Basic characters
    if((string[i] >= 'a' && string[i] <= 'z') 
        || (string[i] >= 'A' && string[i] <= 'Z') 
        || (string[i] >= '0' && string[i] <= '9')) {
      TryCall(charBufAppend, newString, string[i]);

    // After a '\' we can get
    // Special characters: \n \t \" and \\ (backslash)
    // Escaped ascii value: \000 to \255
    } else if(string[i] == '\\'){
      i++;

      // Special characters:
      if(string[i] == 'n'){
        LOG("NEWLINE");
        string[i] = '\n';
        continue;
      }else if(string[i] == 't'){
        string[i] = '\t';
        continue;
      }else if(string[i] == '"'){
        string[i] = '"';
        continue;
      }else if(string[i] == '\\'){
        TryCall(charBufAppendString, newString, "\\092");
        i++;
        continue;
      }

      // Escaped ascii value: \000 to \255
      // Add the number digits to the tempBuffer
      TryCall(charBufAppend, newString, '\\');
      for(int j = 0; j < 3; j++){
        if((int)strlen(string) < i){
          return ERR(LEX_ERR);
        }
        TryCall(charBufAppend, tempBuffer, string[i]);
        i++;
      }
      // Check if the value is a number below 255 (and above 0)
      char *tolptr = NULL;
      int ascii = strtol(tempBuffer->data, &tolptr, 10);
      LOG("TOLOPTR: %s", tolptr);
      if(tolptr[0] || ascii < 0 || ascii > 255){
        return ERR(LEX_ERR);
      }
      // If the value is all right, append the temp data to the new string
      TryCall(charBufAppendString, newString, tempBuffer->data);
      // Clear the temp buffer
      charBufClear(tempBuffer);
      
    // Other special character
    } else {
      char *escapedSeq = NULL;
      GCMalloc(escapedSeq, sizeof(char) * 5);
      sprintf(escapedSeq, "\\%03d", (int) string[i]);
      LOG("got %s", escapedSeq);
      TryCall(charBufAppendString, newString, escapedSeq);
    }

    i++;
  }
  *destPtr = newString->data;
  return 0;
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
  char *isNil = genLabelName("isnil");
  char *notNil = genLabelName("notnil");
  printf("JUMPIFEQ %s LF@%s nil@nil\n", isNil, varInLF);
  printf("MOVE TF@%s LF@%s\n", varInTF, varInLF);
  genUnconditionalJump(notNil);
  genLabel(isNil);
  printf("MOVE TF@%s string@nil\n", varInTF);
  genLabel(notNil);
  return 0;
}

int genReturn(char *src1, char *src2){
  printf("MOVE LF@%s LF@%s\n", src1, src2);
  return 0;
}

/**
 * for write function assing string@nil not nill@nil
 * @return
 */
void genAssignLiteralStringNil(char *name, char *frame) {
  printf("MOVE %s@%s string@nil\n",frame, name);
}

int genAssignLiteral(char *name, int dataType, char *assignValue, char *frame) {
  if(dataType == dt_nil) {
    printf("MOVE %s@%s nil@nil\n",frame, name);
  } else if(dataType == dt_integer) {
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
    char *convertedString = NULL;
    TryCall(stringConvert, &convertedString, assignValue);
    printf("MOVE %s@%s string@%s\n",frame, name, convertedString);
  
  } else if(strcmp(assignValue, "nil") == 0) {
    printf("MOVE %s@%s nil@nil\n", frame, name);
  
  } else if(dataType == -1) {
    printf("MOVE %s@%s %s@%s\n",frame, name, frame, assignValue);
  } else {
    return ERR(INTERN_ERR);
  }

  return 0;
}

char *genType(char *varName){
  char *newVarName = genTmpVarName();
  condAppendToStringBuff(newVarName);
  printf("TYPE LF@%s LF@%s\n", newVarName, varName);
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
  printf("PUSHFRAME\n");
}

// Create a TF
void genFnCallInit(){
  printf("CREATEFRAME\n");
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
  char *falseLabelName = genLabelName("exprIsFalse");
  char *trueLabelName = genLabelName("exprIsTrue");
  char *noJumpLabelName = genLabelName("noJump");

  // typeVarName = type(varName)
  char *typeVarName = genType(varName);

  // if(typeVarName == "nil") jump falseLabelName
  printf("JUMPIFEQ %s LF@%s string@nil\n", falseLabelName, typeVarName);

  // if(typeVarName != "bool") jump trueLabelName
  printf("JUMPIFNEQ %s LF@%s string@bool\n", trueLabelName, typeVarName);

  // if(varName == false) jump falseLabelName
  printf("JUMPIFEQ %s LF@%s bool@false\n", falseLabelName, varName);
  
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
  char *dest = genTmpVarName();
  printf("ADD LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("SUB LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("MUL LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("DIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("IDIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("CONCAT LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

char *genConvertFloatToInt(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("FLOAT2INT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genConvertIntToFloat(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("INT2FLOAT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genStrlen(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("STRLEN LF@%s LF@%s\n", dest, src->data);
  return dest;
}

char *genLower(SStackElem *element1, SStackElem *element2) {
  char *dest = genTmpVarName();
  printf("LT LF@%s LF@%s LF@%s\n", dest, element1->data, element2->data);
  return dest;
}

char *genGreater(SStackElem *element1, SStackElem *element2) {
  char *dest = genTmpVarName();
  printf("GT LF@%s LF@%s LF@%s\n", dest, element1->data, element2->data);
  return dest;
}

char *genEqual(SStackElem *element1, SStackElem *element2) {
  char *dest = genTmpVarName();
  printf("EQ LF@%s LF@%s LF@%s\n", dest, element1->data, element2->data);
  return dest;
}

char *genNot(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("NOT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

void genWrite(char *name) {
  printf("WRITE TF@%s\n", name);
}

// FUNCTIONS FOR generating built in functions

void genBuiltInFunctions(){
  genUnconditionalJump("*built_in_functions_bypass");
  printf("%s\n", reads);
  printf("%s\n", readi);
  printf("%s\n", readn);
  printf("%s\n", tointeger);
  printf("%s\n", substr);
  printf("%s\n", ord);
  printf("%s\n", chr);
  genLabel("*built_in_functions_bypass");
  printf("\n# END OF BUILT IN FUNCTIONS DEFINITIONS\n");
}

#endif
/* end of file generator.c */
