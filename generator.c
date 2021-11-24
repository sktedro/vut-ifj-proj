/*
 * Code generator
 */

#include "generator.h"

/*
name | frame | type | value |
a1   |  0    | int  |   4   |
a2   |  1    | int  |   ?   |

nazovPrem-> a -> prem0
nazovPrem-> b -> prem1
nazovPrem-> add -> prem3
*/

int digits(int value) {

  if(value == 0) {
    return 1;
  }

  int result = 0;
  while( value != 0 ) {
    value /= 10;
    result++;
  }
  return result;
}

/*
 * Generate a unique name for ifj21code
 *
 *
 *
 * */
char *genName(char *name, int frame) {
  char *frameNum = malloc(sizeof(char) * (digits(frame) + 1));
  sprintf(frameNum, "%d", frame);
  char *newName = malloc(sizeof(char) * (strlen(name) + strlen(frameNum) + 1));
  memcpy(newName, name, strlen(name));
  memcpy(&newName[strlen(name)], frameNum, strlen(frameNum) + 1);
  return newName;
}

char *stringConvert(char *string) {
  char *newString = malloc(sizeof(char) * (strlen(string)*3));
  int k = 0;
  int asciiValue = 0;
  int digits = 0;
  
  for(int i=0; i<strlen(string); i++) {

    if((*string >= 'a' && *string <= 'z') || (*string >= 'A' && *string <= 'Z') || (*string >= '0' && *string <= '9')) {

        newString[k] = string[i];
    } else {
        asciiValue = (int) string[i];
        digits = digits(asciiValue);

        

    }


  }

}

/*
 * identifikátor, frame number
 * a = 1
 * a je v globále
 * v ifjcode bude názov a0
 */
void genVarDef(char *id, int frameNumber) {
  printf("DEFVAR LF@%s\n", genName(id, frameNumber));
}

/*
 * identifikátor, frame number, priradzovaná hodnota
 * TODO na konstantu pouzit %a
 */
int genVarAssign(char *id, int frameNumber, char *assignValue, tokenTypeEnum tokenType) {

  if(tokenType == t_int) {
    printf("ADD TF@%s int@0 int@%s\n", genName(id, frameNumber), assignValue);
  } else if(tokenType == t_num) {
    printf("ADD TF@%s float@0 float@%s\n", genName(id, frameNumber), assignValue);
  } else if(tokenType == t_sciNum) {
    printf("ADD TF@%s float@0 float@%s\n", genName(id, frameNumber), assignValue);
  } else if(tokenType == t_str) {
    printf("ADD TF@%s int@0 int@%s\n", genName(id, frameNumber), assignValue);
  } else if(tokenType == t_idOrKeyword && strcmp(assignValue, "nil") == 0) {
    printf("ADD TF@%s int@0 int@%s\n", genName(id, frameNumber), assignValue);
  } else if(tokenType == t_idOrKeyword && strcmp(assignValue, "readi") == 0) {
    printf("READ  TF@%s int\n", genName(id, frameNumber));
  } else if(tokenType == t_idOrKeyword && strcmp(assignValue, "readn") == 0) {
    printf("READ TF@%s float\n", genName(id, frameNumber), assignValue);
  } else if(tokenType == t_idOrKeyword && strcmp(assignValue, "reads") == 0) {
    printf("READ TF@%s string\n", genName(id, frameNumber), assignValue);
  } else {
    // TODO add error code
    return 1;
  }

  return 0;
}

/*
 * volane bude z precedenčnej analýzy (iba)
 * potrebujeme asi tri symbol stack elementy - ľavý op, pravý op a operátor
 */
void genBinaryOperation() {

}

/*
 * podobné ako binaryOp, ale dva elementy - "#VAR" alebo "VAR.."
 */
void genUnaryOperation() {

}

void genFnCall() {
  // push TF to LF
  printf("PUSHFRAME\n");

  // use TF in function


  // create name
  // if return MOVE that variable to local stack
  // return
}


void genStart() {
  printf(".IFJcode21\n\n");
}

/*
 * MOV
 * CREATEFRAME
 * PUSHFRAME
 * POPFRAME
 * DEFVAR
 * CALL
 * RETURN
 *
 * PUSHS
 * POPS
 * CLEARS
 *
 * ADD
 * SUB
 * IDIV
 * ADDS
 * SUBS
 * MULS
 * DIVS
 * IDIVS
 * LET
 * GT
 * EQ
 * LTS
 * GTS
 * EQS
 * AND
 * OR
 * NOT
 * ANDS
 * ORS
 * NOTS
 * INT2FLOAT
 * FLOAT2INT
 * INT2CHAR
 * STR2INT
 * INT2FLOATS
 * FLOAT2INTS
 * INT2CHARS
 * STRI2INTS
 *
 * READ
 * WRITE
 *
 * CONCAT
 * STRLEN
 * GETCHAR
 * SETCHAR
 *
 * TYPE
 *
 * LABEL
 * JUMP
 * JUMIFEQ
 * JUMPIFNEQ
 * JUMPIFEQS
 * JUMPIFNEQS
 * EXIT
 *
 * BREAK
 * DPRINT
 */
