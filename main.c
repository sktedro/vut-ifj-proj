#include "parser.c"

extern GarbageCollector garbageCollector;

int main() {
  // Init the garbage collector
  TryCall(GCInit);

  // INIT - symtab
  TryCall(STInit, &symtab);
  TryCall(STPush, symtab);

  TryCall(initBuiltInFunctions, symtab);

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
