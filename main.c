/**
 * VUT FIT - IFJ 2021
 *
 * @file main.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

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
