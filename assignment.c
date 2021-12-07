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

void AListAdd(AssignElement **head, char *name, char *label, bool first, char *end) {
  AssignElement *element;
  AssignElement *tmp = *head;

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

  while(tmp->next != NULL) {
    tmp = tmp->next;
  }

  return tmp;
}

void AListGenerate(AssignElement *element) {
  AssignElement *tmp = element;
  int cnt = 0;

  while(tmp != NULL) {
    if(tmp->generated == false) {
      if(cnt == 0) {
        genExprFirst(element);
        return;
      } else if(cnt == 2) {
        genExprSecond(element);
        return;
      } else if(tmp->next == NULL) {
        genExprLast(element);
        return;
      } else {
        genExpr(tmp);
        return;
      }
    } else {
      tmp = tmp->next;
      cnt++;
    }
  }
  

}

/*

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


a, b, c, d = a+10, b+10, c+10, d+10

jump last
prvy:
  ADD A A 10

jump end
druhy:
  ADD B B 10

jump prvy
treti:
  ADD C C 10

jump druhy
last:
  ADD D D 10

jump treti
end:
-----------------------------------------------------

c = 3
a,b = c, 20
--
a = 3
b = 20
--

jump last
label prvy
  move a c

jump end
label last

  move b 20
jump prvy

label end

  DEFVAR LF@?tmpvar_0
  MOVE LF@?tmpvar_0 int@3
  MOVE LF@%var_c_1 LF@?tmpvar_0

JUMP *label_EXPR1

LABEL *label_EXPR0
  MOVE LF@%var_b_1 LF@%var_c_1

JUMP *label_EXPR0

LABEL *label_EXPR1

  DEFVAR LF@?tmpvar_1
  MOVE LF@?tmpvar_1 int@20
  MOVE LF@%var_a_1 LF@?tmpvar_1
















*/


#endif