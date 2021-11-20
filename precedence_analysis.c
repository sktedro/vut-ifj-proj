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
  precedenceAnalysis(symtable, token);
  return 0;
}

/**
 * The precedence analysis algorithm
 */
int precedenceAnalysis(STStack *symtable, Token *token) {
  SStack *symstack = SStackInit();
  SStackPush(pt_dollar);

  bool exprEnd = false;
  int input_index;
  int stack_index;

  while (1){
    // get a new token
    if(!exprEnd) {

      // TODO par dalsich podmienok asi. Ocheckovat keywordy a take
      if(token->type == t_colon 
          || token->type == t_comma 
          || token->type == t_assignment){
        stashToken(token);
        exprEnd = true;
        token = NULL;
      }
    }

    SStackElem *top = SStackTopTerminal(symstack);
    // TODO namapovať každý token (t_..) na symbol (st/sym?_..) - nejaká funkcia
    // alebo niečo - aby sme získali symbol
    // tl;dr treba funkciu ktorá z tokenu spraví symbol
    // SStackElem *inputSymbol = ...?
    //
    // Taktiez TODO v misc.h - asi treba dorobit enum na symbol stack type a
    // symbol stack symbol

    input_index = ...;
    stack_index = ...;
    precTab[input_index][stack_index]; // nebo naopak

    if(precTab[top->/*type alebo symbol?*/][] == /*= enum*/){
      // TODO SStackPush prerobit aby bral element, nie type a symbol
      SStackPush(symstack, inputSymbol);

      ret = scanner(&token);
      condVypluj;

    }else if( == /* < enum*/){
      SStackPushAfterTopTerminal(/* < enum*/);
      SStackPush(symstack, inputSymbol);

      ret = scanner(&token);
      condVypluj;

    }else if( == /* > enum*/){

    }else{
      //err
    }

  }


  vypluj 0;
}


int isExpression(char *data) {

  /*if(strcmp()){
    }*/

  vypluj 0;
}


#endif
/* end of file precedence_analysis.h*/
