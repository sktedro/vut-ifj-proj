/**
 * VUT FIT - IFJ 2021
 *
 * @file linked_list.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef LINKED_LIST_C
#define LINKED_LIST_C

#include "linked_list.h"

extern int ret;

/**
 * @brief Initialize an empty element of the linked list
 *
 * @param element: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int LLInit(LinkedList **element) {
  GCMalloc(*element, sizeof(LinkedList));
  (*element)->name = NULL;
  (*element)->dataType = -1;
  (*element)->next = NULL;
  return 0;
}

/**
 * @brief Append a new element to the end of the linked list
 * 
 * @param head: linked list element
 * @param name
 * @param dataType
 *
 * @return 0 if successful, errcode otherwise
 */
int LLAppend(LinkedList **head, char *name, int dataType) {
  // Create the new element and allocate space
  LinkedList *element;
  TryCall(LLInit, &element);
  GCMalloc(element->name, sizeof(char) * (strlen(name) + 1));

  // Copy the data
  memcpy(element, name, strlen(name) + 1);
  element->dataType = dataType;

  // Append it to the end
  LinkedList *tmp = *head;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = element;

  return 0;
}

/**
 * @brief Get the amount of elements in the linked list starting with 'element'
 *
 * @param element: linked list
 *
 * @return Amount of elements in the linked list
 */
int LLGetLength(LinkedList *element) {
  int cnt = 0;
  if(element) {
    cnt++;
    while (element->next != NULL) {
      element = element->next;
      cnt++;
    }
  }
  return cnt;
}

/**
 * @brief Get a pointer to an element in the linked list on index 'index'
 *
 * @param element: linked list
 * @param index
 *
 * @return Element of the linked list at the index specified
 */
LinkedList *LLGetElemByIndex(LinkedList *element, int index) {
  // If the index is zero, return the head
  if(index == 0) {
    return element;
  }

  // Get the element at the index
  int cnt = 0;
  while (element && element->next && index != cnt) {
    element = element->next;
    cnt++;
  }

  // If the element was found, return element
  if(cnt == index){
    return element;
  }

  // If the element does not exist, return NULL
  return NULL;
}

#endif
/* end of file linked_list.c */
