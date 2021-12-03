#include "parser.c"

extern GarbageCollector garbageCollector;

int main() {
  // Init the garbage collector
  TryCall(GCInit);

  // INIT - symtab
  TryCall(STInit, &symtab);
  TryCall(STPush, symtab);

  
  TryCall(STInsert, symtab, "write");
  STSetIsVariable(symtab, "write", false);
  STSetFnDefined(symtab, "write", true);
  TryCall(STAppendParamType, symtab, "write", dt_string);
  TryCall(STInsert, symtab, "readi");
  STSetIsVariable(symtab, "readi", false);
  STSetFnDefined(symtab, "readi", true);
  TryCall(STInsert, symtab, "readn");
  STSetIsVariable(symtab, "readn", false);
  STSetFnDefined(symtab, "readn", true);
  TryCall(STInsert, symtab, "reads");
  STSetIsVariable(symtab, "reads", false);
  STSetFnDefined(symtab, "reads", true);
  TryCall(STInsert, symtab, "substr");
  STSetIsVariable(symtab, "substr", false);
  STSetFnDefined(symtab, "substr", true);
  TryCall(STInsert, symtab, "ord");
  STSetIsVariable(symtab, "ord", false);
  STSetFnDefined(symtab, "ord", true);
  TryCall(STInsert, symtab, "chr");
  STSetIsVariable(symtab, "chr", false);
  STSetFnDefined(symtab, "chr", true);
  

  ret = pStart();
  if(ret){
    GCCollect();
    free(garbageCollector.pointers);
  }

  return ret;
}
