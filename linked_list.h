/**
 * VUT FIT - IFJ 2021
 *
 * @file linked_list.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "misc.h"
#include "generator.h"

/**
 * @brief Initialize an empty element of the linked list
 *
 * @param element: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int LLInit(LinkedList **element) ForceRetUse;

/**
 * @brief Append a new element to the end of the linked list
 *
 * @param head: linked list element
 * @param name
 * @param dataType
 *
 * @return 0 if successful, errcode otherwise
 */
int LLAppend(LinkedList **head, char *name, int dataType) ForceRetUse;

/**
 * @brief Get the amount of elements in the linked list starting with 'element'
 *
 * @param element: linked list
 *
 * @return Amount of elements in the linked list
 */
int LLGetLength(LinkedList *element);

/**
 * @brief Get a pointer to an element in the linked list on index 'index'
 *
 * @param element: linked list
 * @param index
 *
 * @return Element of the linked list at the index specified
 */
LinkedList *LLGetElemByIndex(LinkedList *element, int index);

#endif
/* end of file linked_list.h */
