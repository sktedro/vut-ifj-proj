/** #include "misc.c" */
#include "parser.c"

int main(int argc, char *argv[]){
  // INIT - symtab
  symtab = STInit();

  STPush(symtab);

  ret = pStart();
  CondReturn;

  return 0;
}
