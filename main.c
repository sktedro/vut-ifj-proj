#include "parser.c"

extern GarbageCollector garbageCollector;

int main() {
  // Init the garbage collector
  CondCall(GCInit);

  // INIT - symtab
  CondCall(STInit, &symtab);
  CondCall(STPush, symtab);

  /**
    * CondCall(STInsert, symtab, "readi");
    * STSetIsVariable(symtab, "readi", false);
    * STSetFnDefined(symtab, "readi", true);
    * CondCall(STInsert, symtab, "readn");
    * STSetIsVariable(symtab, "readn", false);
    * STSetFnDefined(symtab, "readn", true);
    * CondCall(STInsert, symtab, "reads");
    * STSetIsVariable(symtab, "reads", false);
    * STSetFnDefined(symtab, "reads", true);
    * CondCall(STInsert, symtab, "substr");
    * STSetIsVariable(symtab, "substr", false);
    * STSetFnDefined(symtab, "substr", true);
    * CondCall(STInsert, symtab, "ord");
    * STSetIsVariable(symtab, "ord", false);
    * STSetFnDefined(symtab, "ord", true);
    * CondCall(STInsert, symtab, "chr");
    * STSetIsVariable(symtab, "chr", false);
    * STSetFnDefined(symtab, "chr", true);
    */

  ret = pStart();
  if(ret){
    GCCollect();
    free(garbageCollector.pointers);
  }

  return ret;
}
