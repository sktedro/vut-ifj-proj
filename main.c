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
  TryCall(STSetIsVariable, symtab, "write", false);
  TryCall(STSetFnDefined, symtab, "write", true);
  TryCall(STAppendParamType, symtab, "write", dt_string);
  TryCall(STInsert, symtab, "readi");
  TryCall(STSetIsVariable, symtab, "readi", false);
  TryCall(STSetFnDefined, symtab, "readi", true);
  TryCall(STInsert, symtab, "readn");
  TryCall(STSetIsVariable, symtab, "readn", false);
  TryCall(STSetFnDefined, symtab, "readn", true);
  TryCall(STInsert, symtab, "reads");
  TryCall(STSetIsVariable, symtab, "reads", false);
  TryCall(STSetFnDefined, symtab, "reads", true);

  // substr
  TryCall(STInsert, symtab, "substr");
  TryCall(STSetName, symtab, "substr", "_$SUBSTR_");
  TryCall(STSetIsVariable, symtab, "substr", false);
  TryCall(STSetFnDefined, symtab, "substr", true);
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
  TryCall(STSetIsVariable, symtab, "ord", false);
  TryCall(STSetFnDefined, symtab, "ord", true);
  TryCall(STAppendParamType, symtab, "ord", dt_string);
  TryCall(STAppendParamType, symtab, "ord", dt_integer);
  TryCall(STAppendParamName, symtab, "ord", "s");
  TryCall(STAppendParamName, symtab, "ord", "i");
  TryCall(STAppendRetType, symtab, "ord", dt_integer);

  // chr
  TryCall(STInsert, symtab, "chr");
  TryCall(STSetName, symtab, "chr", "_$CHR_");
  TryCall(STSetIsVariable, symtab, "chr", false);
  TryCall(STSetFnDefined, symtab, "chr", true);
  TryCall(STAppendParamType, symtab, "chr", dt_integer);
  TryCall(STAppendParamName, symtab, "chr", "i");
  TryCall(STAppendRetType, symtab, "chr", dt_string);
  
  // TODO check if there is a function in the symtable that is not defined!
  // If so -> error


  ret = pStart();
  if(ret){
    GCCollect();
    free(garbageCollector.pointers);
    return ret;
  }

  ret = STFindUndefinedFunctions(symtab);

  GCCollect();
  free(garbageCollector.pointers);

  return ret;
}
