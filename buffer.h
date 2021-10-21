#include <stdlib.h>

#define BUFINITLEN 16

typedef struct{
  char *data;
  int len;
  int size;
} Buffer;

// Allocate a buffer
Buffer *bufInit(){
  // Buffer *buf = (Buffer*)malloc(sizeof(char*) + 2 * sizeof(int));
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

// Append a new character to the buffer data
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

void bufPop(Buffer *buf){
  if(buf && buf->len){
    buf->data[buf->len - 1] = '\0';
  }
}

// Clear all characters of the buffer
void bufClear(Buffer *buf){
  buf->data[0] = '\0';
  buf->len = 0;
}

// Free the buffer
void bufDestroy(Buffer *buf){
  free(buf->data);
  free(buf);
  buf = NULL;
}
