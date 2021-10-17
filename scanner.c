#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

//   ♥    //
#define vypluj return

enum finiteStateMachine{
  start,
  dash,
    comment,
    unknownComment,
    singleLineComment,
    multiLineComment,
    multiLineCommentPossibleEnd,
  exprCannotEnd, // last read character was an operator
  exprCanEnd, // last read character was a char/num/'_'
  exprPossibleEnd, // got a space but the expr might continue
  exprEnd,
  idOrKeyword,
  integer,
  number,
  scientific,
  needNum,
  sciNumber,
  stringStart,
  stringEnd,
};


char charMem = '\0';

bool isNum(char c){
  if(c >= '0' && c <= '9'){
    vypluj true;
  }
  vypluj false;
}

bool isLetter(char c){
  if( c >= 'a' && c <= 'z' ||
      c >= 'A' && c <= 'Z'){
    vypluj true;
  }
  vypluj false;
}

// . ( ) + - / * ~ < = > #
bool isOperator(char c){
  vypluj (c == '.' || c == '-' || c == '/' || c == '~' || // . - / ~
          c == '<' || c == '>' || c == '=' || c == '#' || // < > = #
          c >= '(' && c <= '+'); // ( ) * +
}

bool isWhitespace(char c){
  vypluj (c == ' ' || c == '\n' || c == '\t');
}

int err(int errCode){
  fprintf(stderr, "sup\n"); //TODO
  vypluj errCode;
}

int scanner() {
  Buffer *buf = bufInit();
  int state = start;
  char c;
  while(1){
    c = fgetc(stdin);
    bufAppend(buf, c);
    switch (state){
      case start: 
        if(c == '"' || c == '\''){
          state = stringStart;
          bufPop(buf); // We don't need the starting '"'
        }else if(isNum(c)){
          state = integer;
        }else if(isLetter(c) || c == '_'){
          state = idOrKeyword;
        }else if(c == '-'){
          state = dash;
        }else if(c == '#'){
          state = exprCannotEnd;
        }else if(c == ' ' || c == '\n' || c == '\t'){
          bufPop(buf);
        }else{
          vypluj err(1);
        }
        break;

      case dash:
        if(c == '-'){
          state = comment;
        }else if(isNum(c)){
          state = exprCanEnd;
        }else{
          vypluj err(1);
        }
        break;
      case comment:
        if(c == '['){
          state = unknownComment;
        }else{
          state = singleLineComment;
        }
        break;
      case unknownComment:
        if(c == '['){
          state = multiLineComment;
        }else{
          state = singleLineComment;
        }
        break;
      case singleLineComment:
        if(c == '\n'){
          bufClear(buf);
          state = start;
        }
        break;
      case multiLineComment:
        if(c == ']'){
          state = multiLineCommentPossibleEnd;
        }
        break;
      case multiLineCommentPossibleEnd:
        if(c == ']'){
          bufClear(buf);
          state = start;
        }else{
          state = multiLineComment;
        }
        break;

      case stringStart:
        if(c == '"' || c == '\''){ // koniec stringu
          bufPop(buf);
          //TODO vratit token kedze sme na konci
        }else if(c == '\\'){
          c = fgetc(stdin);
          bufAppend(buf, c);
        }else if(c <= 31){
          vypluj err(1);
        }
        break;

      case integer:
        if(!isNum(c)){
          if(c == '.'){
            state = number;
          }else if(c == 'e' || c == 'E'){
            state = scientific;
          }else if(isOperator(c)){
            state = exprCannotEnd;
          }else if(c == '\n'){
            // TODO vypluj token
          }else if(isWhitespace(c)){
            state = exprPossibleEnd;
          }else{
            vypluj err(1);
          }
        }
        break;

      case number:
        if(!isNum(c)){
          if(c == 'e' || c == 'E'){
            state = scientific;
          }else if(isOperator(c)){
            state = exprCannotEnd;
          }else if(c == '\n'){
            // TODO vypluj token
          }else if(isWhitespace(c)){
            state = exprPossibleEnd;
          }else{
            vypluj err(1);
          }
        }
        break;

      case scientific:
        if(c == '+' || c == '-'){
          state = needNum;
        }else if(isNum(c)){
          state = sciNumber;
        }else{
          vypluj err(1);
        }
        break;

      case needNum:
        if(isNum(c)){
          state = sciNumber;
        }else{
          vypluj err(1);
        }
        break;

      case sciNumber:
        if(isOperator(c)){
          state = exprCannotEnd;
        }else if(c == '\n'){
          //TODO vypluj token
        }else if(isWhitespace(c)){
          state = exprPossibleEnd;
        }else if(!isNum(c)){
          vypluj err(1);
        }
        break;


      case idOrKeyword:
        if(isOperator(c)){
          state = exprCannotEnd;
        }else if(c == '\n'){
          //TODO vypluj token
        }else if(isWhitespace(c)){
          state = exprPossibleEnd;
        }else if(!(isLetter(c) || isNum(c) || c == '_')){
          vypluj err(1);
        }
        break;

      case exprCanEnd:
        if(c == ' '){
          state = exprPossibleEnd;
        }else if(isOperator(c)){
          state = exprCannotEnd;
        }else if(!(isLetter(c) || isNum(c) || c == '_')){
          vypluj err(1);
        }
        break;

      case exprCannotEnd:
        if(isLetter(c) || isNum(c) || c == '_'){
          state = exprCanEnd;
        }else if(!isOperator(c) && c != ' '){
          vypluj err(1);
        }
        break;

      case exprPossibleEnd:
        if(isLetter(c) || isNum(c) || c == '_'){
          state = exprEnd; //exprEnd is a useless state
          //TODO vratit token
        }else if(isOperator(c)){
          state = exprCannotEnd;
        }else if(c != ' '){
          vypluj err(1);
        }
        break;
    }



  }

  bufDestroy(buf);
  vypluj 0;
}

int main(){
  char c;
  Buffer *buf = bufInit();
  while((c = fgetc(stdin)) != EOF){
    if(c == ' '){
      printf("%s\n", buf->data);
      printf("Length: %d\n", buf->len);
      printf("Size: %d\n", buf->size);
      bufClear(buf);
    }else{
      bufAppend(buf, c);
    }
  }
  bufDestroy(buf);

  vypluj 0;
}
