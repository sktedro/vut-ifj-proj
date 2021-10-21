#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"
#include "token.h"

// TODO:
// get character from charMem!
// return what we have when we reach the last input character 

//   ♥    //
#define vypluj return

enum finiteStateMachine{
  s_start,
  s_dash,

  s_comment,
  s_unknownComment,
  s_singleLineComment,
  s_multiLineComment,
  s_multiLineCommentPossibleEnd,

  s_exprCannotEnd, // last read character was an operator
  s_exprCanEnd, // last read character was a char/num/'_'
  s_exprPossibleEnd, // got a space but the s_expr might continue
  s_exprEnd,

  s_idOrKeyword,

  s_integer,
  s_number,
  s_scientific,
  s_needNum,
  s_sciNumber,

  s_stringStart,
  s_stringEnd,
};


char charMem = '\0';

bool isNum(char c){
  if(c >= '0' && c <= '9'){
    vypluj true;
  }
  vypluj false;
}

bool isLetter(char c){
  if((c >= 'a' && c <= 'z')||
     (c >= 'A' && c <= 'Z')){
    vypluj true;
  }
  vypluj false;
}

// . ( ) + - / * ~ < = > #
bool isOperator(char c){
  vypluj (c == '.' || c == '-' || c == '/' || c == '~' || // . - / ~
          c == '<' || c == '>' || c == '=' || c == '#' || // < > = #
         (c >= '(' && c <= '+')); // ( ) * +
}

bool isWhitespace(char c){
  vypluj (c == ' ' || c == '\n' || c == '\t');
}

int err(int errCode){
  fprintf(stderr, "error\n"); //TODO
  vypluj errCode;
}

int scanner(Token **token) {
  Buffer *buf = bufInit();
  int state = s_start;
  char c;
  bool lastChar = false;
  while(!lastChar){
    c = fgetc(stdin);
    if(c != EOF){
      bufAppend(buf, c);
    }else{
      lastChar = true;
      /*
       * *token = tokenInit(t_expression);
       * if(!tokenAddAttribute(*token, buf->data)){
       *   vypluj err(99);
       * }
       * vypluj 0;
       */
    }
    switch (state){
      case s_start: 
        if(c == '"' || c == '\''){
          state = s_stringStart;
          bufPop(buf); // We don't need the starting '"'
        }else if(isNum(c)){
          state = s_integer;
        }else if(isLetter(c) || c == '_'){
          state = s_idOrKeyword;
        }else if(c == '-'){
          state = s_dash;
        }else if(c == '#'){
          state = s_exprCannotEnd;
        }else if(c == ' ' || c == '\n' || c == '\t'){
          bufPop(buf);
        }else{
          vypluj err(1);
        }
        break;

      case s_dash:
        if(c == '-'){
          state = s_comment;
        }else if(isNum(c)){
          state = s_exprCanEnd;
        }else{
          vypluj err(1);
        }
        break;
      case s_comment:
        if(c == '['){
          state = s_unknownComment;
        }else{
          state = s_singleLineComment;
        }
        break;
      case s_unknownComment:
        if(c == '['){
          state = s_multiLineComment;
        }else{
          state = s_singleLineComment;
        }
        break;
      case s_singleLineComment:
        if(c == '\n'){
          bufClear(buf);
          state = s_start;
        }
        break;
      case s_multiLineComment:
        if(c == ']'){
          state = s_multiLineCommentPossibleEnd;
        }
        break;
      case s_multiLineCommentPossibleEnd:
        if(c == ']'){
          bufClear(buf);
          state = s_start;
        }else{
          state = s_multiLineComment;
        }
        break;

      case s_stringStart:
        if(c == '"' || c == '\''){ // end of string
          bufPop(buf);
          *token = tokenInit(t_string);
         if(!tokenAddAttribute(*token, buf->data)){
            vypluj err(99);
          }
          vypluj 0;
        }else if(c == '\\'){
          c = fgetc(stdin);
          bufAppend(buf, c);
        }else if(c <= 31){
          vypluj err(1);
        }
        break;

      case s_integer:
        if(!isNum(c)){
          if(c == '.'){
            state = s_number;
          }else if(c == 'e' || c == 'E'){
            state = s_scientific;
          }else if(isOperator(c)){
            state = s_exprCannotEnd;
          }else if(c == '\n'){
            *token = tokenInit(t_integer);
            if(!tokenAddAttribute(*token, buf->data)){
              vypluj err(99);
            }
            vypluj 0;
          }else if(isWhitespace(c)){
            state = s_exprPossibleEnd;
          }else{
            vypluj err(1);
          }
        }
        break;

      case s_number:
        if(!isNum(c)){
          if(c == 'e' || c == 'E'){
            state = s_scientific;
          }else if(isOperator(c)){
            state = s_exprCannotEnd;
          }else if(c == '\n'){
            *token = tokenInit(t_number);
            if(!tokenAddAttribute(*token, buf->data)){
              vypluj err(99);
            }
            vypluj 0;
          }else if(isWhitespace(c)){
            state = s_exprPossibleEnd;
          }else{
            vypluj err(1);
          }
        }
        break;

      case s_scientific:
        if(c == '+' || c == '-'){
          state = s_needNum;
        }else if(isNum(c)){
          state = s_sciNumber;
        }else{
          vypluj err(1);
        }
        break;

      case s_needNum:
        if(isNum(c)){
          state = s_sciNumber;
        }else{
          vypluj err(1);
        }
        break;

      case s_sciNumber:
        if(isOperator(c)){
          state = s_exprCannotEnd;
        }else if(c == '\n'){
            *token = tokenInit(t_scientificNumber);
            if(!tokenAddAttribute(*token, buf->data)){
              vypluj err(99);
            }
            vypluj 0;
        }else if(isWhitespace(c)){
          state = s_exprPossibleEnd;
        }else if(!isNum(c)){
          vypluj err(1);
        }
        break;


      case s_idOrKeyword:
        if(isOperator(c)){
          state = s_exprCannotEnd;
        }else if(c == '\n'){
            *token = tokenInit(t_idOrKeyword);
            if(!tokenAddAttribute(*token, buf->data)){
              vypluj err(99);
            }
            vypluj 0;
        }else if(isWhitespace(c)){
          state = s_exprPossibleEnd;
        }else if(!(isLetter(c) || isNum(c) || c == '_')){
          vypluj err(1);
        }
        break;

      case s_exprCanEnd:
        if(c == ' '){
          state = s_exprPossibleEnd;
        }else if(isOperator(c)){
          state = s_exprCannotEnd;
        }else if(!(isLetter(c) || isNum(c) || c == '_')){
          vypluj err(1);
        }
        break;

      case s_exprCannotEnd:
        if(isLetter(c) || isNum(c) || c == '_'){
          state = s_exprCanEnd;
        }else if(!isOperator(c) && c != ' '){
          vypluj err(1);
        }
        break;

      case s_exprPossibleEnd:
        if(isLetter(c) || isNum(c) || c == '_' || lastChar){
          state = s_exprEnd; // s_exprEnd is a useless state
          *token = tokenInit(t_expression);
          if(!tokenAddAttribute(*token, buf->data)){
            vypluj err(99);
          }
          vypluj 0;
        }else if(isOperator(c)){
          state = s_exprCannotEnd;
        }else if(c != ' '){
          vypluj err(1);
        }
        break;
    }
  }
  bufDestroy(buf);
  vypluj 0;
}

/*
 * int main(){
 *   char c;
 *   Buffer *buf = bufInit();
 *   while((c = fgetc(stdin)) != EOF){
 *     if(c == ' '){
 *       printf("%s\n", buf->data);
 *       printf("Length: %d\n", buf->len);
 *       printf("Size: %d\n", buf->size);
 *       bufClear(buf);
 *     }else{
 *       bufAppend(buf, c);
 *     }
 *   }
 *   bufDestroy(buf);
 * 
 *   vypluj 0;
 * }
 */
