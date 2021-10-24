#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"
#include "token.h"

// TODO:
// return what we have when we reach the last input character 

//   ♥    //
#define vypluj return

enum finiteStateMachine{
  // s_start,

  // s_idOrKeyword

  // s_integer,
  // s_number,
  // s_scientific,
  // s_needNum,
  // s_sciNumber,

  // s_comment,
  // s_unknownComment,
  // s_singleLineComment,
  // s_multiLineComment,
  // s_multiLineCommentPossibleEnd,

  // s_arithmOpDash,
  s_arithmOpDiv,
  s_arithmOp,
  s_dot,
  s_strOp,
  s_tilde,
  s_relOpSimple,
  s_relOp,
  s_assignment,

  // s_stringStart,
  // s_stringEnd
};


char charMem = '\0';
bool charMemUsed = false;

// Returns true if there was a character in charMem and it was restored
bool restoreChar(Buffer *buf, char *c){
  if(charMem != '\0'){
    bufAppend(buf, charMem);
    *c = charMem;
    charMem = '\0';
    vypluj true;
  }
  vypluj false;
}

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
  fprintf(stderr, "Chyba programu v rámci lexikálnej analýzy.\n"); //TODO
  vypluj errCode;
}

int returnToken(Token **token, int type, Buffer *buf){
  *token = tokenInit(t_integer);
  if(!tokenAddAttribute(*token, buf->data)){
    vypluj err(99);
  }
  vypluj 0;
}

int scanner(Token **token) {
  Buffer *buf = bufInit();
  // If there is a character in memory which we didn't process with the last
  // token, add it to the buffer this time.
  int state = s_start;
  char c;
  bool lastChar = false;
  while(!lastChar){
    if(!restoreChar(buf, &c)){
      c = fgetc(stdin);
      if(c != EOF){
        bufAppend(buf, c);
      }else{
        lastChar = true;
        /*
         * vyplut token???
         */
      }
    }
    switch (state){
      case s_start: 
        if(c == '"'){
          state = s_stringStart;
          bufPop(buf); // We don't need the starting '"'
        }else if(isNum(c)){
          state = s_integer;
        }else if(isLetter(c) || c == '_'){
          state = s_idOrKeyword;
        }else if(c == '-'){
          state = s_arithmOpDash;
        }else if(c == '+' || c == '-' || c == '*'){
          // state = s_arithmOp;
          // VYPLUT TOKEN
        }else if(c == '/'){
          state = s_arithmOpDiv;
        }else if(c == '#'){
          state = s_strOp;
        }else if(c == '.'){
          state = s_dot;
        }else if(c == '~'){
          state = s_tilde;
        }else if(c == '<' || c == '>'){
          state = s_relOpSimple;
        }else if(c == '='){
          state = s_assignment;
        }else if(c == '('){
          state = s_leftParen;
        }else if(c == ')'){
          state = s_rightParen;
        }else if(isWhitespace(c)){
          bufPop(buf); // We don't need the starting '"'
        }else if(c == EOF){
          *token = NULL;
        }else{
          vypluj err(1);
        }


      case s_arithmOpDash:
        if(c == '-'){
          state = s_comment;
        }else{
          charMem = c;
          bufPop(buf);
          return returnToken(token, 0 /*TODO token enum*/, buf);
          //TODO vypluj token
        }
        break;

      case s_comment:
        bufPop(buf);
        if(c == '['){
          state = s_unknownComment;
        }else if(c == '\n'){
          state = s_start;
        }else{
          state = s_singleLineComment;
        }
        break;

      case s_unknownComment:
        bufPop(buf);
        if(c == '['){
          state = s_multiLineComment;
        }else if(c == '\n'){
          state = s_start;
        }else{
          state = s_singleLineComment;
        }
        break;
      case s_singleLineComment:
        bufPop(buf);
        if(c == '\n'){
          state = s_start;
        }
        break;
      case s_multiLineComment:
        bufPop(buf);
        if(c == ']'){
          state = s_multiLineCommentPossibleEnd;
        }
        break;
      case s_multiLineCommentPossibleEnd:
        bufPop(buf);
        if(c == ']'){
          state = s_start;
        }else{
          state = s_multiLineComment;
        }
        break;

      case s_stringStart:
        if(c == '"'){ // end of string
          bufPop(buf);
          // vypluj token
        }else if(c == '\\'){
          c = fgetc(stdin);
          if(c <= 31){ //TODO nepovolene znaky???
            vypluj err(1);
          }
          bufAppend(buf, c);
        }else if(c <= 31){ //TODO nepovolene znaky??
          vypluj err(1);
        }else{
        }
        break;

      case s_integer:
        if(!isNum(c)){
          if(c == '.'){
            state = s_number;
          }else if(c == 'e' || c == 'E'){
            state = s_scientific;
          }else if(isWhitespace(c)){
            bufPop(buf);
            // vypluj token
          }else{
            vypluj err(1);
          }
        }
        break;

      case s_number:
        if(!isNum(c)){
          if(c == 'e' || c == 'E'){
            state = s_scientific;
          }else if(isWhitespace(c)){
            bufPop(buf);
            // vypluj token
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
        if(!isNum(c)){
          if(isWhitespace(c) || isOperator(c)){
            charMem = c;
            bufPop(buf);
            // vypluj token
          }else{
            vypluj err(1);
          }
        }
        break;


      case s_idOrKeyword:
        if(!(isLetter(c) || isNum(c) || c == '_')){
          charMem = c;
          bufPop(buf);
          // vypluj token
        }
        break;

      case s_arithmOpDiv:
        if(c == '/'){
          // state = s_arithmOp;
          //vypluj token celociselne delenie (//)
        }else{
          //err
        }
        break;

      case s_dot:
        if(c == '.'){
          // state = s_stringOp
          // vypluj token
        }else{
          //err
        }
        break;
      case s_tilde:
        if(c == '='){
          // state = s_relOp
          // vypluj token
        }else{
           //err
        }
        break;
      case s_relOpSimple:
        if(c == '='){
          // state = s_relOp
          // vypluj token
        }else{
          charMem = c;
          bufPop(buf);
          //vypluj token;
        break;
      case s_assignment:
        if(c == '='){
          // state = s_relOp
          // vypluj token
        }else{
          charMem = c;
          bufPop(buf);
          //vypluj token;
        }
        break;

    }
  }
  bufDestroy(buf);
  vypluj 0;
}
