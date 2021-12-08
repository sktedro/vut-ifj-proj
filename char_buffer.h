/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef CHAR_BUFFER_H
#define CHAR_BUFFER_H

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
int charBufInit(CharBuffer **buf) ForceRetUse;

/**
 * @brief Append a new character to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param c: character to append
 *
 * @return 0 if successful, errcode otherwise
 */
int charBufAppend(CharBuffer *buf, char c) ForceRetUse;

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
 * @brief Appends a string to a buffer 
 * 
 * @param buffer a pointer to a buffer
 * @param str a string to append
 *
 * @return 0 if successful, errcode otherwise
 */
int charBufAppendString(CharBuffer *buffer, char *str) ForceRetUse;

#endif
/* end of file char_buffer.h */
