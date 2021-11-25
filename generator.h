#ifndef VUT_IFJ_PROJ_GENERATOR_H
#define VUT_IFJ_PROJ_GENERATOR_H

#include "misc.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int digits(int value);
char *genName(char *name, int frame);
char *stringConvert(char *string);
char *genTmpVar();
char *genLabelName();
void genVarDef(SStackElem *element, int frame);
int genVarAssign(SStackElem *element, int frameNumber, char *assignValue);
char *genBinaryOperationAdd(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationSub(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationMul(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationDiv(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationIDiv(SStackElem *src1, SStackElem *src2);
char *genBinaryOperationConcat(SStackElem *src1, SStackElem *src2);
char *genConvertFloatToInt(SStackElem *src);
char *genConvertIntToFloat(SStackElem *src);
char *genUnaryOperation(SStackElem *src);
void genFnCall(SStackElem *element);
void genFnDef(SStackElem *element);
void genFnDefRet();
void genWrite(SStackElem *element, int frame);
void genLowerThan(SStackElem *element1, SStackElem *element2);
void genHigherThan(SStackElem *element1, SStackElem *element2);
char *genLower(SStackElem *element1, SStackElem *element2);
char *genHigher(SStackElem *element1, SStackElem *element2);
char *genEqual(SStackElem *element1, SStackElem *element2);
char *genJumpIfNeq(char *tmp);
char *genJumpIfEq(char *tmp);
void genLabel(char *labelName);
void genStart();

#endif //VUT_IFJ_PROJ_GENERATOR_H