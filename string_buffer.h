/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include "misc.h"

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int stringBufInit(StringBuffer **buf);

/**
 * @brief Append a new string to the buffer
 *
 * @param buf: buffer (pointer) to append to
 * @param str: string to append
 *
 * @return 0 if successful, errcode otherwise
 */
int stringBufAppend(StringBuffer *buf, char *str);

#endif
/* end of file string_buffer.h */