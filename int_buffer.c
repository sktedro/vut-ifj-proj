/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef INTBUFFER_C
#define INTBUFFER_C

#include "int_buffer.h"

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int intBufInit(IntBuffer **buf) {
  *buf = (IntBuffer *)malloc(sizeof(IntBuffer));
  if (!(*buf)) {
    return err(INTERN_ERR);
  }

  (*buf)->data = (int *)malloc(INTBUFINITLEN * sizeof(int));
  if (!(*buf)->data) {
    free(*buf);
    return err(INTERN_ERR);
  }
  (*buf)->size = INTBUFINITLEN;
  (*buf)->len = 0;
  return 0;
}

/**
 * @brief Append a new int to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param i: int to append
 *
 * @return 0 if successful, errcode otherwise
 */
int intBufAppend(IntBuffer *buf, int i) {
  if (buf->len + 1 == buf->size) {
    buf->data = (int *)realloc(buf->data, 2 * buf->size * sizeof(int));
    if (buf->data == NULL) {
      free(buf);
      buf = NULL;
      return err(INTERN_ERR);
    }
    buf->size *= 2;
  }
  buf->data[buf->len] = i;
  (buf->len)++;
  return 0;
}

/**
 * @brief Remove the last int from the buffer
 *
 * @param buf: from which the int should be removed
 */
void intBufPop(IntBuffer *buf) {
  if (buf && buf->len) {
    (buf->len)--;
  }
}

/**
 * @brief Clear all ints of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void intBufClear(IntBuffer *buf) {
  buf->len = 0;
}

/** 
 * @brief Free all memory allocated by the buffer (and its data)
 *
 * @param buf: pointer to a buffer that is to be destroyed
 */
void intBufDestroy(IntBuffer *buf) {
  if (buf) {
    free(buf->data);
    free(buf);
    buf = NULL;
  }
}

#endif
/* end of file int_buffer.c */
