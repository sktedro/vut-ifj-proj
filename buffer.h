#define BUFINITLEN 16

typedef struct{
  char *data;
  int len;
  int size;
} Buffer;

// TODO exit

// Allocate a buffer
Buffer *bufInit(){
  Buffer *buf = (Buffer*)malloc(sizeof(char*) + 2 * sizeof(int));
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
int bufClear(Buffer *buf){
  buf->data[0] = '\0';
  buf->len = 0;
}

// Free the buffer
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