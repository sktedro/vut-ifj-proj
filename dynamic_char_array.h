/**
 * VUT FIT - IFJ 2021
 *
 * @file dynamic_char_array.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef DYNAMIC_CHAR_ARRAY_H
#define DYNAMIC_CHAR_ARRAY_H

#include "misc.h"

// Initial buffer data length (space allocated)
#define CHARBUFINITLEN 16

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int dynCharArrInit(DynamicCharArray **buf) ForceRetUse;

/**
 * @brief Append a new character to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param c: character to append
 *
 * @return 0 if successful, errcode otherwise
 */
int dynCharArrAppend(DynamicCharArray *buf, char c) ForceRetUse;

/**
 * @brief Remove the last character from the buffer
 *
 * @param buf: from which the character should be removed
 */
void dynCharArrPop(DynamicCharArray *buf);

/**
 * @brief Clear all characters of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void dynCharArrClear(DynamicCharArray *buf);

/**
 * @brief Appends a string to a buffer 
 * 
 * @param buffer a pointer to a buffer
 * @param str a string to append
 *
 * @return 0 if successful, errcode otherwise
 */
int dynCharArrAppendString(DynamicCharArray *buffer, char *str) ForceRetUse;

#endif
/* end of file dynamic_char_array.h */
