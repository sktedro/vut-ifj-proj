/**
 * VUT FIT - IFJ 2021
 *
 * @file dynamic_string_array.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef DYNAMIC_STRING_ARRAY_H
#define DYNAMIC_STRING_ARRAY_H

#include "misc.h"

// Initial buffer data length (space allocated)
#define STRINGBUFINITLEN 256

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int dynStrArrInit(DynamicStringArray **buf) ForceRetUse;

/**
 * @brief Append a new string to the buffer
 *
 * @param buf: buffer (pointer) to append to
 * @param str: string to append
 *
 * @return 0 if successful, errcode otherwise
 */
int dynStrArrAppend(DynamicStringArray *buf, char *str) ForceRetUse;

#endif
/* end of file dynamic_string_array.h */
