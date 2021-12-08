/**
 * VUT FIT - IFJ 2021
 *
 * @file int_buffer.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef INT_BUFFER_H
#define INT_BUFFER_H

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
int intBufInit(IntBuffer **buf) ForceRetUse;

/**
 * @brief Append a new int to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param i: int to append
 *
 * @return 0 if successful, errcode otherwise
 */
int intBufAppend(IntBuffer *buf, int i) ForceRetUse;

/**
 * @brief Remove the last int from the buffer
 *
 * @param buf: from which the int should be removed
 */
void intBufPop(IntBuffer *buf);

/**
 * @brief Clear all ints of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void intBufClear(IntBuffer *buf);


#endif
/* end of file int_buffer.h */
