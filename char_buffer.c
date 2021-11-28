/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef CHARBUFFER_C
#define CHARBUFFER_C

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
  *buf = (CharBuffer *)malloc(sizeof(CharBuffer));
  if (!(*buf)) {
    return err(INTERN_ERR);
  }
  (*buf)->data = (char *)malloc(CHARBUFINITLEN * sizeof(char));
  if (!(*buf)->data) {
    free(*buf);
    return err(INTERN_ERR);
  }
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
  if (buf->len + 1 == buf->size) {
    buf->data = (char *)realloc(buf->data, 2 * buf->size * sizeof(char));
    if (buf->data == NULL) {
      free(buf);
      buf = NULL;
      return err(INTERN_ERR);
    }
    buf->size *= 2;
  }
  buf->data[buf->len] = c;
  (buf->len)++;
  buf->data[buf->len] = '\0';
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
 * @brief Free all memory allocated by the buffer (and its data)
 *
 * @param buf: pointer to a buffer that is to be destroyed
 */
void charBufDestroy(CharBuffer *buf) {
  free(buf->data);
  free(buf);
  buf = NULL;
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
    CondCall(charBufInit, buffer);
  }

  for (unsigned int i = 0; i < strlen(orig); i++) {
    CondCall(charBufAppend, *buffer, orig[i]);
  }

  return 0;
}

#endif
/* end of file char_buffer.c */
