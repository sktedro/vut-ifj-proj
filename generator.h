/*
 * Code generator
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "misc.h"
#include "assignment.h"
#include "parser.h"

int digits(int value);
char *genVarName(char *name, int frame);
char *stringConvert(char *string);
char *getDataTypeFromInt(Token *token);
char *genTmpVarName();
char *genLabelName();
char *genTmpVarDef();
void genConditionalJump(char *label, char *varName, bool condition);
void genUnconditionalJump(char *labelName);
void genJumpIfFalse(char *label, char *varName);
void genJumpIfTrue(char *label, char *varName);
void genVarDefLF(char *name);
void genVarDefTF(char *name);
int genAssignLiteral(char *name, int dataType, char *assignValue, char *frame);
char *genBinaryOperationAdd(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2);
char *genConvertFloatToInt(SStackElem *src);
char *genConvertIntToFloat(SStackElem *src);
char *genStrlen(SStackElem *src);
int genReadFunction(char *varName, char *builtInFnName);
int genStringFunction(char *varName, char *builtInFnName, int frame);
int genSubstrFunction(char *target, Token *string, double start, int end, int frame);
void genFnCallInit();
void genFnCall(char *name);
void genFnDef(char *name);
void genPopframe();
void genReturnInstruction();
void genComment(char *comment);
void genComment2(char *comment);
// void genWriteVariable(char *name, char *frame);
// void genWriteLiteral(Token *token, char *frame);
void genWrite(char *name);
char *genLower(SStackElem *element1, SStackElem *element2);
char *genGreater(SStackElem *element1, SStackElem *element2);
char *genEqual(SStackElem *element1, SStackElem *element2);
char *genNot(SStackElem *src);
void genLabel(char *labelName);
void genStart();
int genPassParam(char *varInLF, char *varInTF);
int genMove(char *dest, char *src);
void genJumpIfFalse(char *label, char *varName);
char *genParamVarName();
char *genRetVarName();
int genReturn(char *varInLF, char *varInTF);
void genUnconditionalJump(char *labelName);

// --------------------------------------------------------------------------------
// FUNCTIONS FOR MULTIPLE ASSIGMENT
void genExprLabel(char *name);
//void genExprJump(char *label); robí to isté ako genUnconditionalJump
char *getExprLabelName(int num);
char *getExprEndName();
void genExprFirst(AssignElement *element);
void genExpr(AssignElement *element);
void genExprSecond(AssignElement *element);
void genExprLast(AssignElement *element);
void genExprEnd(AssignElement *element);
// --------------------------------------------------------------------------------

#endif
/* end of file generator.c */
