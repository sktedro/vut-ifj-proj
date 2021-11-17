/*
 * Scanner
 */

#ifndef SCANNER
#define SCANNER

#include "misc.h"
#include "char_buffer.h"
#include "token.h"

// TODO:
// return what we have when we reach the last input character 

// A memory keeping the last read character, if it wasn't a part of the last
// returned token. Upon every scanner called, character from this variable (if
// it is not '\0') should be processed before reading from stdin
char charMem = '\0';

// A memory where we can stash a token (that should be returned with the next 
// scanner() call)
Token *tokenMem = NULL;

/**
 * @brief Stash a token (to be returned on next scanner() call)
 * 
 * @param token to be stashed
 */
int stashToken(Token *token){
  if(tokenMem){
    fprintf(stderr, "This is bad\n");
    return err(1); //TODO err code??
  }
  tokenMem = token;
  return 0;
}

/**
 * @brief Restore last read character that didn't belong to the last returned
 * token
 *
 * @return true if there was a character in charMem (and was restored)
 */
bool restoreChar(CharBuffer *buf, char *c){
  if(charMem != '\0'){
    if(charBufAppend(buf, charMem)){
      return err(99);
    }
    *c = charMem;
    charMem = '\0';
    vypluj true;
  }
  vypluj false;
}

/**
 * @brief returns true if c is a number
 *
 * @param char
 *
 * @return true if char is a number from 0 to 9
 */
bool isNum(char c){
  if(c >= '0' && c <= '9'){
    vypluj true;
  }
  vypluj false;
}

/**
 * @brief returns true if c is a letter
 *
 * @param char
 *
 * @return true if char is a letter (upper or lowercase) from a to z
 */
bool isLetter(char c){
  if((c >= 'a' && c <= 'z')||
     (c >= 'A' && c <= 'Z')){
    vypluj true;
  }
  vypluj false;
}

/**
 * @brief returns true if c is an operator
 *
 * @param char
 *
 * @return true if char is an operator (list: . ( ) + - / * ~ < = > #)
 */
bool isOperator(char c){
  vypluj (c == '.' || c == '-' || c == '/' || c == '~' || // . - / ~
          c == '<' || c == '>' || c == '=' || c == '#' || // < > = #
         (c >= '(' && c <= '+') || c == ':'); // ( ) * +
}


/**
 * @param char
 *
 * @return true if char is a space, newline or tabulator
 */
bool isWhitespace(char c){
  vypluj (c == ' ' || c == '\n' || c == '\t');
}

/**
 * @brief Returns a token (writes it to the provided pointer)
 * 
 * @param token: pointer to address where the new token should be written
 * @param type: type of the token that is to be returned
 * @param buf: buffer from which the token data should be read
 *
 * @return true if char is a space, newline or tabulator
 */
int returnToken(Token **token, int type, CharBuffer *buf){
  *token = tokenInit(type);
  if(!(*token)){
    charBufDestroy(buf);
    vypluj err(99);
  }
  if(tokenAddAttrib(*token, buf->data)){
    charBufDestroy(buf);
    vypluj err(99);
  }
  charBufDestroy(buf);
  vypluj 0;
}

/**
 * @brief Main scanner function - returns the next token based on lexical
 * analysis of characters from the standard input
 *
 * @param token: address to memory where the next token should be written
 * @return error code
 */
int scanner(Token **token) {
  if(tokenMem){
    *token = tokenMem;
    tokenMem = NULL;
    return 0;
  }

  // Token data (characters composing it) will be written here
  CharBuffer *buf = charBufInit();
  if(!buf){
    return err(99);
  }


  // Starting state of the finite state machine is s_start
  int state = s_start;

  // Used to temporarily store a character read from stdin
  char c;

  // Indicates if EOF was encountered
  bool lastChar = false;

  // Main loop - read a character and based on the actual state and the
  // character decide what to do - change state, return token,...
  while(!lastChar){

    // Only if there was no character in charMem, read a new one from stdin
    // (otherwise process the character from charMem)
    if(!restoreChar(buf, &c)){
      c = fgetc(stdin); // TODO c should be int?
      if(c != EOF){
        charBufAppend(buf, c);
      }else{
        lastChar = true;
        /*
         * vyplut token??? TODO
         */
      }

    }

    // Main switch to change the program flow based on the actual FSM state
    switch (state){
      case s_start: 
        // "
        if(c == '"'){
          // We don't need (and want) the starting '"'
          charBufPop(buf);
          state = s_strStart;

        // 0-9
        }else if(isNum(c)){
          state = s_int;

        // a-z A-Z _
        }else if(isLetter(c) || c == '_'){
          state = s_idOrKeyword;

        // -
        }else if(c == '-'){
          state = s_arithmOpDash;

        // + - *
        }else if(c == '+' || c == '-' || c == '*'){
          // state = s_arithmOp;
          return returnToken(token, t_arithmOp, buf);

        // ,
        }else if(c == ','){
          // state = s_comma;
          return returnToken(token, t_comma, buf);

        // :
        }else if(c == ':'){
          // state = s_colon;
          return returnToken(token, t_colon, buf);

        // /
        }else if(c == '/'){
          state = s_arithmOpDiv;

        // #
        }else if(c == '#'){
          state = s_strOp;

        // .
        }else if(c == '.'){
          state = s_dot;

        // ~
        }else if(c == '~'){
          state = s_tilde;

        // < >
        }else if(c == '<' || c == '>'){
          state = s_relOpSimple;

        // =
        }else if(c == '='){
          state = s_assignment;

        // (
        }else if(c == '('){
          // state = s_leftParen;
          return returnToken(token, t_leftParen, buf);

        // )
        }else if(c == ')'){
          // state = s_rightParen
          return returnToken(token, t_rightParen, buf);

        // space, \n, \t
        }else if(isWhitespace(c)){
          // Ignoring the whitespaces
          charBufPop(buf); 

        // EOF
        }else if(c == EOF){
          // Return no token
          *token = NULL;

        // ELSE
        }else{
          vypluj err(1);
        }

        break;


      // Got a dash - could be an arithmetic operator (minus) or a comment (--)
      case s_arithmOpDash:
        charBufPop(buf);
        if(c == '-'){
          charBufPop(buf);
          state = s_comment;
        }else{
          charMem = c;
          return returnToken(token, t_arithmOp, buf);
        }
        break;

      // Definitely a comment (got --). Don't yet know if it is a singleline or 
      // a multiline comment
      case s_comment:
        charBufPop(buf);
        if(c == '['){
          state = s_unknownComment;
        }else if(c == '\n'){
          state = s_start;
        }else if(c != ' '){
          state = s_singleLineComment;
        }
        break;

      // Got a '[', so it could be a multiline, based on the next character
      case s_unknownComment:
        charBufPop(buf);
        if(c == '['){
          state = s_multiLineComment;
        }else if(c == '\n'){
          state = s_start;
        }else{
          state = s_singleLineComment;
        }
        break;

      // Definitely a singleline comment
      case s_singleLineComment:
        charBufPop(buf);
        if(c == '\n'){
            charBufClear(buf);
          state = s_start;
        }
        break;

      // Definitely a multiline comment
      case s_multiLineComment:
        charBufPop(buf);
        if(c == ']'){
          state = s_multiLineCommentPossibleEnd;
        }
        break;

      // Got a ']' in a multiline comment, receiving another ']' ends it
      case s_multiLineCommentPossibleEnd:
        charBufPop(buf);
        if(c == ']'){
          state = s_start;
        }else{
          state = s_multiLineComment;
        }
        break;

      // Got a '"', getting another finishes the string
      case s_strStart:
        // End of string
        if(c == '"'){
          charBufPop(buf);
          // state = s_strEnd
          return returnToken(token, t_str, buf);
        // If there is an escaped character, instantly append it
        }else if(c == '\\'){
          c = fgetc(stdin);
          if(c <= 31){ //TODO nepovolene znaky???
            vypluj err(1);
          }
          if(charBufAppend(buf, c)){
            return err(99);
          }
        }else if(c <= 31){ //TODO nepovolene znaky??
          vypluj err(1);
        }
        break;

      // So far, received only digits (0-9) so it is an integer literal
      case s_int:
        if(!isNum(c)){
          if(c == '.'){
            state = s_num;
          }else if(c == 'e' || c == 'E'){
            state = s_scientific;
          }else {
              if (!isWhitespace(c)) {
                  charMem = c;
              }
              charBufPop(buf);
              return returnToken(token, t_int, buf);
          }
        }
        break;

      // So far, received digits (0-9) and a dot (.) so it is a 'number'
      // literal
      case s_num:
        if(!isNum(c)){
          if(c == 'e' || c == 'E'){
            state = s_scientific;
          }else {
              if (!isWhitespace(c)) {
                  charMem = c;
              }
            charBufPop(buf);
            return returnToken(token, t_num, buf);
          }
        }
        break;

      // Received 'e' or 'E', which means the literal is in scientific form
      // (but is not yet fully processed!)
      case s_scientific:
        if(c == '+' || c == '-'){
          state = s_needNum;
        }else if(isNum(c)){
          state = s_sciNum;
        }else{
          vypluj err(1);
        }
        break;

      // After receiving 'e' or 'E' which indicates a scientific form of a
      // number and after receiving '+' or '-', we need at least one digit to
      // end the literal
      case s_needNum:
        if(isNum(c)){
          state = s_sciNum;
        }else{
          vypluj err(1);
        }
        break;

      // The scientific number literal is now fully processed and can be
      // returned
      case s_sciNum:
        if(!isNum(c)){
            charMem = c;
            charBufPop(buf);
            return returnToken(token, t_sciNum, buf);
        }
        break;

      // Could be an identificator or a keyword
      case s_idOrKeyword:
        if(!(isLetter(c) || isNum(c) || c == '_')){
          charMem = c;
          charBufPop(buf);
          vypluj returnToken(token, t_idOrKeyword, buf);
        }
        break;

      // Got one '/', but might get another which would make it a different
      // operator ('//')
      case s_arithmOpDiv:
            if(c == '/'){
          // state = s_arithmOp;
              charBufAppend(buf,c);
          return returnToken(token, t_arithmOp, buf);
        }else {
          charMem = c;
          charBufPop(buf);
          return returnToken(token, t_arithmOp, buf);
          }
        break;

      // Got one '.', but two are necessary for it to make a token
      case s_dot:
        if(c == '.'){
          // state = s_strOp
          return returnToken(token, t_strOp, buf);
        }else{
          vypluj err(1);
        }
        break;

      // After the '~', '=' must follow
      case s_tilde:
        if(c == '='){
          // state = s_relOp
          return returnToken(token, t_relOp, buf);
        }else{
          vypluj err(1);
        }
        break;

      // Got a '<' or '>', might get one '='
      case s_relOpSimple:
        if(c == '='){
          // state = s_relOp
          return returnToken(token, t_relOp, buf);
        }else{
          charMem = c;
          charBufPop(buf);
          return returnToken(token, t_relOp, buf);
        }
        break;

      // A simple assignment ('=')
      case s_assignment:
        //printf("C = %c\n",c);
        if(c == '='){
          // state = s_relOp
          return returnToken(token, t_relOp, buf);
        }else{
          charMem = c;
          charBufPop(buf);
          return returnToken(token, t_assignment, buf);
        }
        break;

    }
  }
  
  // We should never reach these lines, but in case we do...
  // Free the allocated buffer
  charBufDestroy(buf);
  vypluj 0;
}

#endif
/* end of file scanner.h */