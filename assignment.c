/**
 * VUT FIT - IFJ 2021
 *
 * @file assignment.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef ASSIGNMENT_C
#define ASSIGNMENT_C

#include "assignment.h"

extern int ret;

int AListInit(AssignElement **element) {

  GCMalloc(*element, sizeof(AssignElement));
  GCMalloc((*element)->name, sizeof(char) * 30);
  GCMalloc((*element)->label, sizeof(char) * 30);
  GCMalloc((*element)->end, sizeof(char) * 30);
  
  (*element)->next = NULL;
  (*element)->prev = NULL;
  (*element)->first = true;
  
  return 0;
}

void AListAdd(AssignElement **head, char *name, char *label, bool first,int dataType, char *end) {
  
   AssignElement *element;
  AssignElement *tmp = *head;

  if((*head) != NULL && (*head)->name == NULL) {
    (*head)->name = name;
    (*head)->label = label;
    (*head)->dataType = dataType;
    return;
  }

  AListInit(&element);

  element->name = name;
  element->label = label;
  element->first = first;
  element->generated = false;
  element->end = end;

  while (tmp->next != NULL) {
      tmp = tmp->next;
  }
  tmp->next = element;
  element->prev = tmp;
}

int AListGetLength(AssignElement *element) {
  int cnt = 0;
  AssignElement *tmp = element;

  if(tmp != NULL && tmp->next != NULL) {
    cnt++;
    while (tmp->next != NULL) {
      tmp = tmp->next;
      cnt++;
    }
  }

  return cnt;
}

char *printBool(bool val) {
  char *data;
  GCMalloc(data, sizeof(char) * 10);

  if(val == true) {
    strcpy(data, "true");
    return data;
  } else if(val == false) {
    strcpy(data, "false");
    return data;
  }
  vypluj NULL;
}

char *printIfNUll(AssignElement *element) {
  char *data;

  GCMalloc(data, sizeof(char) * 10);

  if(element == NULL) {
      strcpy(data, "(NULL)");
  } else {
      strcpy(data, "exists");
  }

  vypluj data;
}

void AListPrintElement(AssignElement *element, int num) {
  printf("Element %d name : %s\n", num, element->name);
  printf("Element %d label : %s\n", num, element->label);
  printf("Element %d is generated : %s\n", num, printBool(element->generated));
  printf("Element %d is first : %s\n",num, printBool(element->first));
  printf("Element %d has next : %s\n",num, printIfNUll(element->next));
  printf("Element %d has previous : %s\n",num, printIfNUll(element->prev));
}

void AListDebugPrint(AssignElement *element) {
  AssignElement *tmp = element;
  int cnt = 0;

  if(tmp == NULL) {
    printf("AList is NULL\n");
    printf("AList len in %d\n", cnt);
  } else {

    while(tmp != NULL) {
      cnt++;
      AListPrintElement(tmp, cnt);
      tmp = tmp->next;
    }
  }
  printf("-----------DONE-------------\n");
}

AssignElement *AListGetElementByIndex(AssignElement *element, int index) {
  AssignElement *tmp = element;
  int cnt = 0;
  
  if(index == 0) {
    return element;
  }

  if(tmp != NULL) {
    
    while (tmp->next != NULL) {
      tmp = tmp->next;
      cnt++;

      if(index == cnt) {
        return tmp;
      }
    }

    return NULL;

  } else {
    return NULL;
  }


}

void AListClear(AssignElement **element) {
  (*element)->next = NULL;
  (*element)->end = NULL;
  (*element)->generated = false;
  (*element)->name = NULL;
  (*element)->label = NULL;
}

AssignElement *AListGetLast(AssignElement *element) {
  AssignElement *tmp = element;

  if(element == NULL) {
    vypluj NULL;
  }

  while(tmp->next != NULL) {
    tmp = tmp->next;
  }

  return tmp;
}

AssignElement *AListGetHead(AssignElement *element) {
  AssignElement *tmp = element;

  if(element == NULL) {
    vypluj NULL;
  }

  while(tmp->prev != NULL) {
    tmp = tmp->prev;
  }

  return tmp;

}

bool AListIsLast(AssignElement *element) {

  if(element->next == NULL) {
    vypluj true;
  }
  vypluj false;
}

#endif
