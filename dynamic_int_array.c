/**
 * VUT FIT - IFJ 2021
 *
 * @file dynamic_int_array.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef INT_BUFFER_C
#define INT_BUFFER_C

#include "dynamic_int_array.h"

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
  (*buf)->len = 0;
  (*buf)->size = INTBUFINITLEN / sizeof(int);
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
/* end of file dynamic_int_array.c */
