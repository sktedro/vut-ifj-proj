/**
 * VUT FIT - IFJ 2021
 *
 * @file dynamic_char_array.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef DYNAMIC_CHAR_ARRAY_C
#define DYNAMIC_CHAR_ARRAY_C

#include "dynamic_char_array.h"

extern int ret;

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int dynCharArrInit(DynamicCharArray **buf) {
  GCMalloc(*buf, sizeof(DynamicCharArray));
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
int dynCharArrAppend(DynamicCharArray *buf, char c) {
  if(buf){
    if (buf->len + 1 == buf->size) {
      GCRealloc(buf->data, 2 * buf->size * sizeof(char));
      buf->size *= 2;
    }
    buf->data[buf->len] = c;
    (buf->len)++;
    buf->data[buf->len] = '\0';
    return 0;
  }
  return ERR(INTERN_ERR);
}

/**
 * @brief Remove the last character from the buffer
 *
 * @param buf: from which the character should be removed
 */
void dynCharArrPop(DynamicCharArray *buf) {
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
void dynCharArrClear(DynamicCharArray *buf) {
  if(buf && buf->data){
    buf->data[0] = '\0';
    buf->len = 0;
  }
}

/**
 * @brief Appends a string to a buffer 
 * 
 * @param buffer a pointer to a buffer
 * @param str a string to append
 *
 * @return 0 if successful, errcode otherwise
 */
int dynCharArrAppendString(DynamicCharArray *buffer, char *str) {
  if (str && buffer) {
    int len = strlen(str);
    for(int i = 0; i < len; i++){
      TryCall(dynCharArrAppend, buffer, str[i]);
    }
    return 0;
  }
  return ERR(INTERN_ERR);
}

#endif
/* end of file dynamic_char_array.c */
