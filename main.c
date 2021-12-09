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

  // Init the symtable
  TryCall(STInit, &symtab);
  TryCall(STPush, symtab);

  // Insert the built-in functions into the symtable
  TryCall(initBuiltInFunctions, symtab);

  // Run the parser
  ret = pStart();
  if(ret){
    // Free all allocated memory
    GCCollect();
    free(garbageCollector.pointers);
    return ret;
  }

  // Check for functions that have been declared but not defined
  ret = STFindUndefinedFunctions(symtab);

  // Free all allocated memory
  GCCollect();
  free(garbageCollector.pointers);

  return ret;
}

/* end of file main.c */
