/**
 * VUT FIT - IFJ 2021
 *
 * @file dynamic_string_array.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef DYNAMIC_STRING_ARRAY_C
#define DYNAMIC_STRING_ARRAY_C

#include "dynamic_string_array.h"

extern int ret;

/**
 * @brief Allocate a new buffer
 *
 * @param buf: destination pointer
 *
 * @return 0 if successful, errcode otherwise
 */
int dynStrArrInit(DynamicStringArray **buf) {
  GCMalloc(*buf, sizeof(DynamicStringArray));
  GCMalloc((*buf)->data, STRINGBUFINITLEN * sizeof(char));
  (*buf)->len = 0;
  (*buf)->size = STRINGBUFINITLEN / sizeof(char *);
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
int dynStrArrAppend(DynamicStringArray *buf, char *str) {
  if(!buf || !str || !buf->data) {
    return ERR(INTERN_ERR);
  } 
  if (buf->len + 1 == buf->size) {
    GCRealloc(buf->data, 2 * buf->size * sizeof(char *));
    buf->size *= 2;
  }
  GCMalloc(buf->data[buf->len], sizeof(char) * (strlen(str) + 1));
  sprintf(buf->data[buf->len], "%s", str);
  (buf->len)++;
  
  return 0;
}

#endif
/* end of file dynamic_string_array.c */
