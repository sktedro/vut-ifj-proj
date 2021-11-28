/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef INTBUFFER_H
#define INTBUFFER_H

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

/**
 * @brief Free all memory allocated by the buffer (and its data)
 *
 * @param buf: pointer to a buffer that is to be destroyed
 */
void intBufDestroy(IntBuffer *buf);

#endif
/* end of file int_buffer.h */
