#include "misc.h"

// Initial buffer data length (space allocated)
#define BUFINITLEN 16

// Structure defining the buffer
typedef struct{
  char *data;
  int len; // Actual buffer data length
  int size; // Size allocated for the buffer
} Buffer;

// TODO exit

// Allocate a buffer
/**
 * @brief Allocate a new buffer
 *
 * @return new buffer (pointer)
 */
Buffer *bufInit(){
  Buffer *buf = (Buffer*)malloc(sizeof(Buffer));
  if(buf == NULL){
    return NULL;
  }
  buf->data = (char*)malloc(BUFINITLEN * sizeof(char));
  if(buf->data == NULL){
    free(buf);
    return NULL;
  }
  buf->size = BUFINITLEN;
  buf->data[0] = '\0';
  return buf;
}

/**
 * @brief Append a new character to the buffer data
 *
 * @param buf: buffer (pointer) to append to
 * @param c: character to append
 *
 * @return 0 if successful
 */
int bufAppend(Buffer *buf, char c){
  if(buf->len + 1 == buf->size){
    buf->data = (char*)realloc(buf->data, 2 * buf->size * sizeof(char));
    if(buf->data == NULL){
      free(buf);
      buf = NULL;
      return 1;
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
void bufPop(Buffer *buf){
  if(buf && buf->len){
    (buf->len)--;
    buf->data[buf->len] = '\0';
  }
}

/**
 * @brief Clear all characters of the buffer
 *
 * @param buf: pointer to the buffer that is to be cleared
 */
void bufClear(Buffer *buf){
  buf->data[0] = '\0';
  buf->len = 0;
}

/** 
 * @brief Free all memory allocated by the buffer (and its data)
 *
 * @param buf: pointer to a buffer that is to be destroyed
 */
void bufDestroy(Buffer *buf){
  free(buf->data);
  free(buf);
  buf = NULL;
}

/**
 * Appends a string to an existing buffer or initialises a new buffer if the given pointer is null.
 * @param orig a string to append
 * @param buffer a pointer to a buffer
 */
void bufAppendString(char *orig, Buffer **buffer) {
    if(*buffer == NULL) {
        *buffer = bufInit();
    }

    for (unsigned int i = 0; i < strlen(orig); i++) {
        bufAppend(*buffer,orig[i]);
    }
}