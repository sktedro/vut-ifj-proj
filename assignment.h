/**
 * VUT FIT - IFJ 2021
 *
 * @file assignment.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "misc.h"
#include "generator.h"

int AListInit(AssignElement **element);
void AListAdd(AssignElement **head, char *name, char *label, bool first, char *end);
int AListGetLength(AssignElement *element);
AssignElement *AListGetElementByIndex(AssignElement *element, int index);
AssignElement *AListGetLast(AssignElement *element);
void AListGenerate(AssignElement *element);
void AListClear(AssignElement **element);
void AListPrintElement(AssignElement *element, int num);
void AListDebugPrint(AssignElement *element);
bool AListIsLast(AssignElement *element);
AssignElement *AListGetHead(AssignElement *element);

#endif
/* end of file assignment.h */
