/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef STRING_BUFFER_C
#define STRING_BUFFER_C

#include "string_buffer.h"

// Initial buffer data length (space allocated)
#define STRINGBUFINITLEN 16

extern int ret;

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int stringBufInit(StringBuffer **buf) {
  GCMalloc(*buf, sizeof(StringBuffer));
  GCMalloc((*buf)->data, STRINGBUFINITLEN * sizeof(char));
  (*buf)->len = 0;
  (*buf)->size = STRINGBUFINITLEN;
  return 0;
}

/**
 * @brief Append a new string to the buffer
 *
 * @param buf: buffer (pointer) to append to
 * @param str: string to append
 *
 * @return 0 if successful, errcode otherwise
 */
int stringBufAppend(StringBuffer *buf, char *str) {

  if(buf){
    if (buf->len + 1 == buf->size) {
      GCRealloc(buf->data, 2 * buf->size * sizeof(char *));
      buf->size *= 2;
    }
    GCMalloc(buf->data[buf->len], sizeof(char) * (strlen(str) + 1));
    memcpy(buf->data[buf->len], str, strlen(str) + 1);
    (buf->len)++;
  }
  return 0;
}

#endif
/* end of file string_buffer.c */