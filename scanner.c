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
  exprBegin,
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

int scanner() {
  Buffer *buf = bufInit();
  int state = start;
  char c;
  while(1){
    c = fgetc(stdin);
    bufAppend(buf, c);
    switch (state){
      case start: 
        if(c == '"'){
          state = string;
          bufPop(buf); // We don't need the starting '"'
        }else if(isNum(c)){
          state = integer;
        }else if(isLetter(c) || c == '_'){
          state = idOrKeyword;
        }else if(c == '-'){
          state = dash;
        }else if(c == '.'){
          state = dot;
        }else if(c == '<' || c == '>' || c == '='){
          state = possibleCompare;
        }else if(c == '#' || c == '*' || c == '+' || c == ':'){
          state = operator;
        }else if(c == '~'){
          state = tilda;
        }else if(c == '/'){
          state = slash;
        }else if(c == '('){
          state = leftParen;
        }else if(c == ')'){
          state = rightParen;
        }
        break;

      case dash:
        if(c == '-'){
          state = comment;
        }else{
          state = exprCannotEnd;
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
          state = start;
        }
        break;
      case multiLineComment:
        if(c == ']'){
          state = multiLinePossibleEnd;
        }
        break;
      case multiLinePossibleEnd:
        if(c == ']'){
          state = start;
        }else{
          state = multiLineComment;
        }
        break;

      case string:
        if(c == '"'){
          bufPop(buf);
          //TODO vratit token kedze sme na konci
        }else if (c == '\\'){
          c = fgetc(stdin);
          bufAppend(buf, c);
        }
        break;
      case integer:
        if(!isNum(c)){
          if(c == '.'){
            state = number;
          }else if(c == ' '){
            // TODO vypluj token
          }else{
            // TODO vypluj token or throw error
          }
        }
        break;
      case number:
        if(!isNum(c)){
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
