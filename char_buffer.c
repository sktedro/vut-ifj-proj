/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef CHAR_BUFFER_C
#define CHAR_BUFFER_C

#include "char_buffer.h"

// Initial buffer data length (space allocated)
#define CHARBUFINITLEN 16

extern int ret;

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int charBufInit(CharBuffer **buf) {
  GCMalloc(*buf, sizeof(CharBuffer));
  GCMalloc((*buf)->data, CHARBUFINITLEN * sizeof(char));
  (*buf)->len = 0;
  (*buf)->size = CHARBUFINITLEN;
  (*buf)->data[0] = '\0';
  return 0;
}

/**
 * @brief Append a new character to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param c: character to append
 *
 * @return 0 if successful, errcode otherwise
 */
int charBufAppend(CharBuffer *buf, char c) {
  if(buf){
    if (buf->len + 1 == buf->size) {
      GCRealloc(buf->data, 2 * buf->size * sizeof(char));
      buf->size *= 2;
    }
    buf->data[buf->len] = c;
    (buf->len)++;
    buf->data[buf->len] = '\0';
  }
  return 0;
}

/**
 * @brief Remove the last character from the buffer
 *
 * @param buf: from which the character should be removed
 */
void charBufPop(CharBuffer *buf) {
  if (buf && buf->len) {
    (buf->len)--;
    buf->data[buf->len] = '\0';
  }
}

/**
 * @brief Clear all characters of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void charBufClear(CharBuffer *buf) {
  buf->data[0] = '\0';
  buf->len = 0;
}

/**
 * @brief Appends a string to an existing buffer or initialises a new buffer if the 
 * given pointer is null.
 * 
 * @param orig a string to append
 * @param buffer a pointer to a buffer
 *
 * @return 0 if successful, errcode otherwise
 */
int charBufAppendString(char *orig, CharBuffer **buffer) {
  if (*buffer == NULL) {
    TryCall(charBufInit, buffer);
  }

  for (unsigned int i = 0; i < strlen(orig); i++) {
    TryCall(charBufAppend, *buffer, orig[i]);
  }

  return 0;
}

#endif
/* end of file char_buffer.c */
