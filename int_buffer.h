/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef INTBUFFER_H
#define INTBUFFER_H

#include "misc.h"


/**
 * @brief Allocate a new buffer
 *
 * @return new buffer (pointer)
 */
IntBuffer *intBufInit();

/**
 * @brief Append a new int to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param i: int to append
 *
 * @return 0 if successful
 */
int intBufAppend(IntBuffer *buf, int i);

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
