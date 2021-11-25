/** #include "misc.c" */
#include "parser.c"

int main() {
  // INIT - symtab
  symtab = STInit();

  STPush(symtab);

  STInsert(symtab, "readi");
  STSetIsVariable(symtab, "readi", false);
  STSetFnDefined(symtab, "readi", true);
  STInsert(symtab, "readn");
  STSetIsVariable(symtab, "readn", false);
  STSetFnDefined(symtab, "readn", true);
  STInsert(symtab, "reads");
  STSetIsVariable(symtab, "reads", false);
  STSetFnDefined(symtab, "reads", true);
  STInsert(symtab, "substr");
  STSetIsVariable(symtab, "substr", false);
  STSetFnDefined(symtab, "substr", true);
  STInsert(symtab, "ord");
  STSetIsVariable(symtab, "ord", false);
  STSetFnDefined(symtab, "ord", true);
  STInsert(symtab, "chr");
  STSetIsVariable(symtab, "chr", false);
  STSetFnDefined(symtab, "chr", true);

  ret = pStart();
  CondReturn;

  return 0;
}
