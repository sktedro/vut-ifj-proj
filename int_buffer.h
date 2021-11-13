/*
 * A buffer - basically just a dynamic-sized string
 */

#ifndef INTBUFFER
#define INTBUFFER

#include "misc.h"


// Initial buffer data length (space allocated)
#define INTBUFINITLEN 16


/**
 * @brief Allocate a new buffer
 *
 * @return new buffer (pointer)
 */
IntBuffer *intBufInit(){
  IntBuffer *buf = (IntBuffer*)malloc(sizeof(IntBuffer));
  if(buf == NULL){
    exit(MALLOC_ERROR);
  }

  buf->data = (int*)malloc(INTBUFINITLEN * sizeof(int));
  if(buf->data == NULL){
    free(buf);
    exit(MALLOC_ERROR);
  }
  buf->size = INTBUFINITLEN;
  buf->len = 0;
  return buf;
}

/**
 * @brief Append a new int to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param i: int to append
 *
 * @return 0 if successful
 */
int intBufAppend(IntBuffer *buf, int i){
  if(buf->len + 1 == buf->size){
    buf->data = (int*)realloc(buf->data, 2 * buf->size * sizeof(int));
    if(buf->data == NULL){
      free(buf);
      buf = NULL;
      return 1;
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
void intBufPop(IntBuffer *buf){
  if(buf && buf->len){
    (buf->len)--;
  }
}

/**
 * @brief Clear all ints of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void intBufClear(IntBuffer *buf){
  buf->len = 0;
}

/** 
 * @brief Free all memory allocated by the buffer (and its data)
 *
 * @param buf: pointer to a buffer that is to be destroyed
 */
void intBufDestroy(IntBuffer *buf){
  if(buf){
    free(buf->data);
    free(buf);
    buf = NULL;
  }
}

#endif
/* end of file int_buffer.h */