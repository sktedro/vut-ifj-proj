/**
 * VUT FIT - IFJ 2021
 *
 * @file generator.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "misc.h"
#include "linked_list.h"
#include "parser.h"

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
char *genVarName(char *baseName, int frame);

/**
 * @brief Generate a new, unique temporary variable name
 *
 * @return new name
 */
char *genTmpVarName();

/**
 * @brief Generate a new, unique return variable name
 *
 * @param baseName: string that will be included in the name
 *
 * @return new name
 */
char *genRetVarName(char *baseName);

/**
 * @brief Generate a new, unique label name
 *
 * @param baseName: string that will be included in the name
 *
 * @return new name
 */
char *genLabelName(char *baseName);

/**
 * @brief Generate a new, unique parameter name
 *
 * @param baseName: string that will be included in the name
 *
 * @return new name
 */
char *genParamVarName(char *baseName);

/**
 * @brief Generates .IFJcode21 literal
 */
void genStart();

/** 
 * @brief Generate a beginning comment in the ifjcode language
 *
 * @param comment 
 */
void genComment(char *comment);

/** 
 * @brief Generate an ending comment in the ifjcode language
 *
 * @param comment 
 */
void genComment2(char *comment);

/**
 * @brief Reset the variable used to keep track of how many parameters were
 * already passed (generated) to the function
 */
void resetParamCounter();

/**
 * @brief Reset the variable used to keep track of how many return values were
 * already returned (generated) from the function
 */
void resetRetCounter();

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
void genVarDefLF(char *name);

/**
 * @brief Generate code to define a variable in the temporary frame
 *
 * @param name of the variable
 */
void genVarDefTF(char *name);

/**
 * @brief Generate code to move from LF to LF
 *
 * @param dest
 * @param src
 */
void genMove(char *dest, char *src);

/**
 * @brief Generate code to move from LF to TF
 *
 * @param dest
 * @param src
 */
void genMoveToTF(char *dest, char *src);

/**
 * @brief Generate code to move from TF to LF
 *
 * @param dest
 * @param src
 */
void genMoveToLF(char *dest, char *src);

/**
 * @brief Generate code to pass a parameter to a function
 *
 * @param varInTF: name of the variable
 * @param varInLF: name of the variable
 *
 * @return 0 if successful, errcode otherwise
 */
int genPassParam(char *varInTF, char *varInLF) ForceRetUse;

/**
 * @brief Generates code that returns 'amount' of nils from a function
 *
 * @param amount
 *
 * @return 0 if successful, errcode otherwise
 */
int genNilsReturn(int amount) ForceRetUse;

/**
 * @brief Assign a 'nil' string literal to a variable
 *
 * @param name of the variable
 * @param frame: LF or TF (string)
 */
void genAssignLiteralStringNil(char *name, char *frame);

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
int genAssignLiteral(char *name, int dataType, char *assignValue, char *frame) ForceRetUse;

/** 
 * @brief Generate TYPE instruction
 *
 * @param varName 
 *
 * @return name of the variable where the result string is stored
 */
char *genType(char *varName);

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
void genFnCall(char *fnName);

/** 
 * @brief Generate the start of a function definition
 */
void genFnDef(char *name);

/** 
 * @brief Generate a CREATEFRAME instruction
 */
void genCreateframe();

/** 
 * @brief Generate a POPFRAME instruction
 */
void genPopframe();

/** 
 * @brief Generate a RETURN instruction
 */
void genReturnInstruction();

/*
 *
 * Jumps and labels
 *
 */

/** Brief generate an unconditional jump (JUMP instruction)
 *
 * @param labelName of a label to jump to
 */
void genUnconditionalJump(char *labelName);

/** @brief Generate a conditional jump (if the varName is evaluated as 'false')
 *
 * @param label to jump to
 * @param varName to evaluate
 */
void genJumpIfFalse(char *label, char *varName);

/** @brief Generate a conditional jump (if the varName is evaluated as 'true')
 *
 * @param label to jump to
 * @param varName to evaluate
 */
void genJumpIfTrue(char *label, char *varName);

/**
 * @brief Generate a LABEL instruction
 *
 * @param labelName
 */
void genLabel(char *labelName);

/**
 * @brief Generates a code representing an 'if' in other programming
 * languages
 *
 * @param label to jump to if the condition is met
 * @param varName of which value to test
 * @param condition: requirement for the varName value (true or false)
 */
void genConditionalJump(char *label, char *varName, bool condition);

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
char *genBinaryOperationAdd(SStackElem *src1, SStackElem *src2);

/**
 * @brief Generate SUB instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2);

/**
 * @brief Generate MUL instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2);

/**
 * @brief Generate DIV instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2);

/**
 * @brief Generate IDIV (integer division) instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2);

/**
 * @brief Generate CONCAT instruction
 *
 * @param src1: first operand
 * @param src2: second operand
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2);

/**
 * @brief Generate FLOAT2INT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genConvertFloatToInt(SStackElem *src);

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genConvertIntToFloat(SStackElem *src);

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genStrlen(SStackElem *src);

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genLower(SStackElem *element1, SStackElem *element2);

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genGreater(SStackElem *element1, SStackElem *element2);

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genEqual(SStackElem *element1, SStackElem *element2);

/**
 * @brief Generate NOT instruction
 *
 * @param src: the input variable
 *
 * @return String containing the name of the variable where the result is stored
 */
char *genNot(SStackElem *src);

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
void genWrite(char *name);

/**
 * @brief Generate all built in functions (to be called before generating any
 * other code with the exception of .ifjcode21)
 */
void genBuiltInFunctions();

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
char *getDataTypeFromInt(Token *token);

/**
 * @brief Converts ifj21 string to ifj21code string
 * 
 * @param destPtr
 * @param string
 *
 * @return 0 if successful, errcode otherwise
 */
int stringConvert(char **destPtr, char *string) ForceRetUse;

#endif
/* end of file generator.h */
