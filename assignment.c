#ifndef ASSIGNMENT_C
#define ASSIGNMENT_C

#include "assignment.h"

extern int ret;

int AListInit(AssignElement **element) {

  GCMalloc(*element, sizeof(AssignElement));
  if (!(*element)) {
    return ERR(INTERN_ERR);
  }
  
  GCMalloc((*element)->name, sizeof(char) * 30);
  if (!(*element)->name) {
    return ERR(INTERN_ERR);
  }

  GCMalloc((*element)->label, sizeof(char) * 30);
  if (!(*element)->label) {
    return ERR(INTERN_ERR);
  }

  GCMalloc((*element)->end, sizeof(char) * 30);
  if (!(*element)->end) {
    return ERR(INTERN_ERR);
  }

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

  if(tmp != NULL) {
    cnt++;
    while (tmp->next != NULL) {
      tmp = tmp->next;
      cnt++;
    }
  }

  return cnt;
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

*/


#endif