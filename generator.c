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

// Strings containing codes of built-in functions
extern const char *reads;
extern const char *readi;
extern const char *readn;
extern const char *write;
extern const char *tointeger;
extern const char *substr;
extern const char *ord;
extern const char *chr;

// Prefixes of different names
const char *labelPrefix = "*label_";
const char *varPrefix = "%var_";
const char *tmpVarPrefix = "?tmpvar_";
const char *paramPrefix = "$param_";
const char *retPrefix = "!ret_";
const char *fnPrefix = "&fn_";

// Used to generate unique names
int tmpCounter = 0;
int labelCounter = 0;
int paramCounter = 0;
int retCounter = 0;
int writeCount = 0;

/*
 *
 * Generating names for variables and labels
 *
 */

/**
 * @brief Generate a new, unique variable name
 *
 * @param baseName: string that will be included in the name
 * @param frame: frame (depth) of the variable in IFJ21 language
 *
 * @return new name
 */
char *genVarName(char *baseName, int frame) {
  char *newName;
  int mallocLen = strlen(varPrefix) + strlen(baseName) + countDigits(frame) + 2;
  GCMalloc(newName, sizeof(char) * mallocLen);
  sprintf(newName, "%s%s_%d", varPrefix, baseName, frame);
  return newName;
}

/**
 * @brief Generate a new, unique temporary variable name
 *
 * @return new name
 */
char *genTmpVarName() {
  char *varName;
  int mallocLen = strlen(tmpVarPrefix) + countDigits(tmpCounter) + 1;
  GCMalloc(varName, sizeof(char) * mallocLen);
  sprintf(varName, "%s%d", tmpVarPrefix, tmpCounter);
  tmpCounter++;
  appendToVarDeclarationList(varName);
  return varName;
}

/**
 * @brief Generate a new, unique return variable name
 *
 * @param baseName: string that will be included in the name
 *
 * @return new name
 */
char *genRetVarName(char *baseName) {
  char *retName;
  int mallocLen = strlen(retPrefix) + strlen(baseName) + countDigits(retCounter) + 1;
  GCMalloc(retName, sizeof(char) * mallocLen);
  sprintf(retName, "%s%d", retPrefix, retCounter);
  retCounter++;
  return retName;
}

/**
 * @brief Generate a new, unique label name
 *
 * @param baseName: string that will be included in the name
 *
 * @return new name
 */
char *genLabelName(char *baseName) {
  char *varName;
  int mallocLen = strlen(labelPrefix) + strlen(baseName) + countDigits(labelCounter) + 2;
  GCMalloc(varName, sizeof(char) * mallocLen);
  sprintf(varName, "%s%s_%d", labelPrefix, baseName, labelCounter);
  labelCounter++;
  return varName;
}

/**
 * @brief Generate a new, unique parameter name
 *
 * @param baseName: string that will be included in the name
 *
 * @return new name
 */
char *genParamVarName(char *baseName) {
  char *tmp;
  int mallocLen = strlen(paramPrefix) + strlen(baseName) + countDigits(paramCounter) + 2;
  GCMalloc(tmp, sizeof(char) * mallocLen);
  sprintf(tmp, "%s%s_%d", paramPrefix, baseName, paramCounter);
  paramCounter++;
  return tmp;
}

/**
 * @brief Generates .IFJcode21 literal
 */
void genStart() {
  printf(".IFJcode21\n\n");
}

/** 
 * @brief Generate a beginning comment in the ifjcode language
 *
 * @param comment 
 */
void genComment(char *comment){
  printf("\n# %s\n", comment);
}

/** 
 * @brief Generate an ending comment in the ifjcode language
 *
 * @param comment 
 */
void genComment2(char *comment){
  printf("# %s\n\n", comment);
}

/**
 * @brief Reset the variable used to keep track of how many parameters were
 * already passed (generated) to the function
 */
void resetParamCounter(){
  paramCounter = 0;
}

/**
 * @brief Reset the variable used to keep track of how many return values were
 * already returned (generated) from the function
 */
void resetRetCounter(){
  retCounter = 0;
}

/*
 *
 * Operations with variables (def, assign, ...)
 *
 */

/**
 * @brief Generate code to define a variable in the local frame
 *
 * @param name of the variable
 */
void genVarDefLF(char *name) {
  printf("DEFVAR LF@%s\n", name);
}

/**
 * @brief Generate code to define a variable in the temporary frame
 *
 * @param name of the variable
 */
void genVarDefTF(char *name) {
  printf("DEFVAR TF@%s\n", name);
}

/**
 * @brief Generate code to move from LF to LF
 *
 * @param dest
 * @param src
 */
void genMove(char *dest, char *src){
  printf("MOVE LF@%s LF@%s\n", dest, src);
}

/**
 * @brief Generate code to move from LF to TF
 *
 * @param dest
 * @param src
 */
void genMoveToTF(char *dest, char *src){
  printf("MOVE TF@%s LF@%s\n", dest, src);
}

/**
 * @brief Generate code to move from TF to LF
 *
 * @param dest
 * @param src
 */
void genMoveToLF(char *dest, char *src){
  printf("MOVE LF@%s TF@%s\n", dest, src);
}

/**
 * @brief Generate code to pass a parameter to a function
 *
 * @param varInTF: name of the variable
 * @param varInLF: name of the variable
 *
 * @return 0 if successful, errcode otherwise
 */
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

/**
 * @brief Generates code that returns 'amount' of nils from a function
 *
 * @param amount
 *
 * @return 0 if successful, errcode otherwise
 */
int genNilsReturn(int amount){
  for(int i = 0; i < amount; i++){
    // Generate and define the retArgName
    char *retArgName = genRetVarName("");
    genVarDefLF(retArgName);
    // Pass nil to retVarName
    TryCall(genAssignLiteral, retArgName, dt_nil, "nil", "LF");
  }
  return 0;
}

/**
 * @brief Assign a 'nil' string literal to a variable
 *
 * @param name of the variable
 * @param frame: LF or TF (string)
 */
void genAssignLiteralStringNil(char *name, char *frame) {
  printf("MOVE %s@%s string@nil\n",frame, name);
}

/**
 * @brief Assign a literal to a variable
 *
 * @param name of the variable where the literal should be stored
 * @param dataType of the literal
 * @param assignValue: the literal itself
 * @param frame: TF or LF (string)
 *
 * @return 0 if successful, errcode otherwise
 */
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

/** 
 * @brief Generate TYPE instruction
 *
 * @param varName 
 *
 * @return name of the variable where the result string is stored
 */
char *genType(char *varName){
  char *newVarName = genTmpVarName();
  appendToVarDeclarationList(newVarName);
  printf("TYPE LF@%s LF@%s\n", newVarName, varName);
  return newVarName;
}

/*
 *
 * Function calls
 *
 */

/** 
 * @brief Generate CALL instruction to call fnName
 *
 * @param fnName: name of the function to call
 */
void genFnCall(char *fnName) {
  printf("CALL %s0\n", fnName);
}

/** 
 * @brief Generate the start of a function definition
 */
void genFnDef(char *name) {
  printf("LABEL %s0\n\n", name);
  printf("PUSHFRAME\n");
}

/** 
 * @brief Generate a CREATEFRAME instruction
 */
void genCreateframe(){
  printf("CREATEFRAME\n");
}

/** 
 * @brief Generate a POPFRAME instruction
 */
void genPopframe() {
  printf("POPFRAME\n");
}

/** 
 * @brief Generate a RETURN instruction
 */
void genReturnInstruction() {
  printf("RETURN\n");
}

/*
 *
 * Jumps and labels
 *
 */

/** Brief generate an unconditional jump (JUMP instruction)
 *
 * @param labelName of a label to jump to
 */
void genUnconditionalJump(char *labelName){
  printf("JUMP %s\n", labelName);
}

/** @brief Generate a conditional jump (if the varName is evaluated as 'false')
 *
 * @param label to jump to
 * @param varName to evaluate
 */
void genJumpIfFalse(char *label, char *varName) {
  genConditionalJump(label, varName, false);
}

/** @brief Generate a conditional jump (if the varName is evaluated as 'true')
 *
 * @param label to jump to
 * @param varName to evaluate
 */
void genJumpIfTrue(char *label, char *varName) {
  genConditionalJump(label, varName, true);
}

/**
 * @brief Generate a LABEL instruction
 *
 * @param labelName
 */
void genLabel(char *labelName) {
  printf("LABEL %s\n", labelName);
}

/**
 * @brief Generates a code representing an 'if' in other programming
 * languages
 *
 * @param label to jump to if the condition is met
 * @param varName of which value to test
 * @param condition: requirement for the varName value (true or false)
 */
void genConditionalJump(char *label, char *varName, bool condition){
  /*
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
 *
 * Operations to be called by the shift-reduce parser
 *
 */

/**
 * @brief Generate ADD instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationAdd(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("ADD LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

/**
 * @brief Generate SUB instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("SUB LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

/**
 * @brief Generate MUL instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("MUL LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

/**
 * @brief Generate DIV instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("DIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

/**
 * @brief Generate IDIV (integer division) instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("IDIV LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

/**
 * @brief Generate CONCAT instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2) {
  char *dest = genTmpVarName();
  printf("CONCAT LF@%s LF@%s LF@%s\n", dest, src1->data, src2->data);
  return dest;
}

/**
 * @brief Generate FLOAT2INT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genConvertFloatToInt(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("FLOAT2INT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genConvertIntToFloat(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("INT2FLOAT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genStrlen(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("STRLEN LF@%s LF@%s\n", dest, src->data);
  return dest;
}

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genLower(SStackElem *element1, SStackElem *element2) {
  char *dest = genTmpVarName();
  printf("LT LF@%s LF@%s LF@%s\n", dest, element1->data, element2->data);
  return dest;
}

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genGreater(SStackElem *element1, SStackElem *element2) {
  char *dest = genTmpVarName();
  printf("GT LF@%s LF@%s LF@%s\n", dest, element1->data, element2->data);
  return dest;
}

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genEqual(SStackElem *element1, SStackElem *element2) {
  char *dest = genTmpVarName();
  printf("EQ LF@%s LF@%s LF@%s\n", dest, element1->data, element2->data);
  return dest;
}

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genNot(SStackElem *src) {
  char *dest = genTmpVarName();
  printf("NOT LF@%s LF@%s\n", dest, src->data);
  return dest;
}

/*
 *
 * Generating the built in functions
 *
 */

/**
 * @brief Generates the write instruction
 *
 * @param name of the variable to pass the function
 */
void genWrite(char *name) {
  printf("WRITE TF@%s\n", name);
}

/**
 * @brief Generate all built in functions (to be called before generating any
 * other code with the exception of .ifjcode21)
 */
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

/*
 *
 * Other helper functions
 *
 */

/**
 * @brief Returns a string representing a data type of the input token
 *
 * @param token
 *
 * @return String representing the data type of the token
 */
char *getDataTypeFromInt(Token *token) {
  char *tmp;
  GCMalloc(tmp, sizeof(char) * 10);
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
 * @param destPtr
 * @param string
 *
 * @return 0 if successful, errcode otherwise
 */
int stringConvert(char **destPtr, char *string) {
  DynamicCharArray *newString;
  DynamicCharArray *tempBuffer;
  TryCall(dynCharArrInit, &newString);
  TryCall(dynCharArrInit, &tempBuffer);

  int i = 0;
  while(string[i] != '\0'){
    LOG("CHAR: %c", string[i]);
    // Basic characters
    if((string[i] >= 'a' && string[i] <= 'z') 
        || (string[i] >= 'A' && string[i] <= 'Z') 
        || (string[i] >= '0' && string[i] <= '9')) {
      TryCall(dynCharArrAppend, newString, string[i]);

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
        TryCall(dynCharArrAppendString, newString, "\\092");
        i++;
        continue;
      }

      // Escaped ascii value: \000 to \255
      // Add the number digits to the tempBuffer
      TryCall(dynCharArrAppend, newString, '\\');
      for(int j = 0; j < 3; j++){
        if((int)strlen(string) < i){
          return ERR(LEX_ERR);
        }
        TryCall(dynCharArrAppend, tempBuffer, string[i]);
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
      TryCall(dynCharArrAppendString, newString, tempBuffer->data);
      // Clear the temp buffer
      dynCharArrClear(tempBuffer);
      
    // Other special character
    } else {
      char *escapedSeq = NULL;
      GCMalloc(escapedSeq, sizeof(char) * 5);
      sprintf(escapedSeq, "\\%03d", (int) string[i]);
      LOG("got %s", escapedSeq);
      TryCall(dynCharArrAppendString, newString, escapedSeq);
    }

    i++;
  }
  *destPtr = newString->data;
  return 0;
}

#endif
/* end of file generator.c */
