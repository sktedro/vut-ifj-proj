/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef INT_BUFFER_C
#define INT_BUFFER_C

#include "int_buffer.h"

extern int ret;

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int intBufInit(IntBuffer **buf) {
  GCMalloc(*buf, sizeof(IntBuffer));
  GCMalloc((*buf)->data, INTBUFINITLEN * sizeof(int));
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
  if(buf) {
    if (buf->len + 1 == buf->size) {
      GCRealloc(buf->data, 2 * buf->size * sizeof(int));
      buf->size *= 2;
    }
    buf->data[buf->len] = i;
    (buf->len)++;
    return 0;
  } 
  return ERR(INTERN_ERR);
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


#endif
/* end of file int_buffer.c */
