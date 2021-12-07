#include "parser.c"

extern GarbageCollector garbageCollector;

int main() {
  // Init the garbage collector
  TryCall(GCInit);

  // INIT - symtab
  TryCall(STInit, &symtab);
  TryCall(STPush, symtab);

  // Insert the built in functions to the symtable
  // TODO move this somewhere else when it's done?
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

  // substr
  TryCall(STInsert, symtab, "substr");
  TryCall(STSetName, symtab, "substr", "_$SUBSTR_");
  STSetIsVariable(symtab, "substr", false);
  STSetFnDefined(symtab, "substr", true);
  TryCall(STAppendParamType, symtab, "substr", dt_string);
  TryCall(STAppendParamType, symtab, "substr", dt_integer);
  TryCall(STAppendParamType, symtab, "substr", dt_integer);
  TryCall(STAppendParamName, symtab, "substr", "s");
  TryCall(STAppendParamName, symtab, "substr", "i");
  TryCall(STAppendParamName, symtab, "substr", "j");
  TryCall(STAppendRetType, symtab, "substr", dt_string);

  // ord
  TryCall(STInsert, symtab, "ord");
  TryCall(STSetName, symtab, "ord", "_$ORD_");
  STSetIsVariable(symtab, "ord", false);
  STSetFnDefined(symtab, "ord", true);
  TryCall(STAppendParamType, symtab, "ord", dt_string);
  TryCall(STAppendParamType, symtab, "ord", dt_integer);
  TryCall(STAppendParamName, symtab, "ord", "s");
  TryCall(STAppendParamName, symtab, "ord", "i");
  TryCall(STAppendRetType, symtab, "ord", dt_integer);

  // chr
  TryCall(STInsert, symtab, "chr");
  TryCall(STSetName, symtab, "chr", "_$CHR_");
  STSetIsVariable(symtab, "chr", false);
  STSetFnDefined(symtab, "chr", true);
  TryCall(STAppendParamType, symtab, "chr", dt_integer);
  TryCall(STAppendParamName, symtab, "chr", "i");
  TryCall(STAppendRetType, symtab, "chr", dt_string);
  

  ret = pStart();
  if(ret){
    GCCollect();
    free(garbageCollector.pointers);
  }

  return ret;
}
