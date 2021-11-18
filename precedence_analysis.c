/*
 * Precedence analysis
 */

#ifndef PRECEDENCE_ANALYSIS_C
#define PRECEDENCE_ANALYSIS_C

#include "precedence_analysis.h"

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


int precedenceAnalysis(STStack *symtable, Token *token) {

  if(token->type == t_idOrKeyword) {
    return 1;
  }
    

  return 0;
}

int isExpression(char *data) {

  /*if(strcmp()){
  }*/
  
  return 0;
}


#endif
/* end of file precedence_analysis.h*/
