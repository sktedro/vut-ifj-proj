
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

#endif
/* end of file assignment.h */
