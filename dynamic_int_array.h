/**
 * VUT FIT - IFJ 2021
 *
 * @file dynamic_int_array.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef DYNAMIC_INT_ARRAY_H
#define DYNAMIC_INT_ARRAY_H

#include "misc.h"

// Initial buffer data length (space allocated)
#define INTBUFINITLEN 16

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int dynIntArrInit(DynamicIntArray **buf) ForceRetUse;

/**
 * @brief Append a new int to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param i: int to append
 *
 * @return 0 if successful, errcode otherwise
 */
int dynIntArrAppend(DynamicIntArray *buf, int i) ForceRetUse;

/**
 * @brief Remove the last int from the buffer
 *
 * @param buf: from which the int should be removed
 */
void dynIntArrPop(DynamicIntArray *buf);

/**
 * @brief Clear all ints of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void dynIntArrClear(DynamicIntArray *buf);


#endif
/* end of file dynamic_int_array.h */
