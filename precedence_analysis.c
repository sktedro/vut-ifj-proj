/*
 * Precedence analysis
 */

#ifndef PRECEDENCE_ANALYSIS_C
#define PRECEDENCE_ANALYSIS_C

#include "precedence_analysis.h"

int ret = 0;

// Precedence table
// Could be simpler since rows (columns) repeat
char precTab[12][12] = {
  //#   *   /   //  +   -   .. rel  (   )   i   $
  {'_','>','>','>','>','>','>','>','<','>','<','>'}, // pt_strlen
  {'<','>','>','>','>','>','>','>','<','>','<','>'}, // pt_mult
  {'<','>','>','>','>','>','>','>','<','>','<','>'}, // pt_div
  {'<','>','>','>','>','>','>','>','<','>','<','>'}, // pt_intDiv
  {'<','<','<','<','>','>','>','>','<','>','<','>'}, // pt_add
  {'<','<','<','<','>','>','>','>','<','>','<','>'}, // pt_sub
  {'<','<','<','<','<','<','<','>','<','>','<','>'}, // pt_concat
  {'<','<','<','<','<','<','<','>','<','>','<','>'}, // pt_relOp
  {'<','<','<','<','<','<','<','<','<','=','<','_'}, // pt_lParen
  {'>','>','>','>','>','>','>','>','_','>','_','>'}, // pt_rParen
  {'>','>','>','>','>','>','>','>','_','>','_','>'}, // pt_id
  {'<','<','<','<','<','<','<','<','<','_','<','_'}  // pt_dollar
};

// todo zomriet
//TODO pravidlá, pushovanie na stack, čítanie zo stacku

/**
 * This is called from the parser
 */
int parseExpression(STStack *symtable, Token *token){
  return 0;
}

/**
 * The precedence analysis algorithm
 */
int precedenceAnalysis(STStack *symtable, Token *token) {
  Token *token = NULL;
  SStack *symstack = SStackInit();
  SStackPush(pt_dollar);

  bool input_empty = false;
  int input_index;
  int stack_index;

  while (1){
    // get a new token
    if(!input_empty) {
      ret = scanner(&token);
      condVypluj;
    }

    if(/*expression end*/) {
      // stash token
      input_empty = true;
      token = NULL;
    }

    input_index = ...;
    stack_index = ...;
    precTab[input_index][stack_index]; // nebo naopak


  }
  if(token->type == t_idOrKeyword) {
    vypluj 1;


    vypluj 0;
  }

  int isExpression(char *data) {

    /*if(strcmp()){
      }*/

    vypluj 0;
  }


#endif
  /* end of file precedence_analysis.h*/
