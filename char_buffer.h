/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef CHARBUFFER_H
#define CHARBUFFER_H

#include "misc.h"

/**
 * @brief Allocate a new buffer
 *
 * @return new buffer (pointer)
 */
CharBuffer *charBufInit();

/**
 * @brief Append a new character to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param c: character to append
 *
 * @return 0 if successful
 */
int charBufAppend(CharBuffer *buf, char c);

/**
 * @brief Remove the last character from the buffer
 *
 * @param buf: from which the character should be removed
 */
void charBufPop(CharBuffer *buf);

/**
 * @brief Clear all characters of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void charBufClear(CharBuffer *buf);

/**
 * @brief Free all memory allocated by the buffer (and its data)
 *
 * @param buf: pointer to a buffer that is to be destroyed
 */
void charBufDestroy(CharBuffer *buf);

/**
 * Appends a string to an existing buffer or initialises a new buffer if the given pointer is null.
 * @param orig a string to append
 * @param buffer a pointer to a buffer
 */
void charBufAppendString(char *orig, CharBuffer **buffer);

#endif
/* end of file char_buffer.h */
