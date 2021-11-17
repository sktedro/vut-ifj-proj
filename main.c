#include "misc.h"
#include "parser.h"




int main(int argc, char *argv[]){
  // INIT - symtab
  symtab = STInit();

  STPush(symtab);

  int ret = pStart();
  CondReturn;

  return 0;
}
