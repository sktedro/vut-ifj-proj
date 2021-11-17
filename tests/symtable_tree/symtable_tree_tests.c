/**
 *
 */

#include "../../misc.h"
#include "../../symtable_tree.h"

int testSetFunctions(STElem *tmp, bool insertParamOrRetTypes){
  if(!tmp){
    printf("Element to be inserted is not found in the tree\n");
    return 1;
  }
  // Set data as for a variable
  treeSetVarData(tmp, 2, 4);
  if(tmp->isVariable != true){
    printf("Element inserted should be a variable but is not\n");
    return 1;
  }
  if(tmp->varDataType != 2){
    printf("Element (var) inserted but data type is wrong\n");
    return 1;
  }
  if(tmp->varAddress != 4){
    printf("Element (var) inserted but address is wrong\n");
    return 1;
  }

  // Set data as for a function (as not defined)
  treeSetFnData(tmp, false);
  if(tmp->isVariable == true){
    printf("Element inserted should be a function but is not\n");
    return 1;
  }
  if(tmp->fnDefined){
    printf("Element (function) should not be defined, but is\n");
    return 1;
  }

  if(insertParamOrRetTypes){
    // Insert one param type
    treeAppendParamType(tmp, 10);
    if(tmp->fnParamTypesBuf->len != 1){
      printf("len: %d\n", tmp->fnParamTypesBuf->len);
      printf("Inserted one param type but buffer length is not 1\n");
      return 1;
    }
    if(tmp->fnParamTypesBuf->data[0] != 10){
      printf("Inserted one param type but data is not right\n");
      return 1;
    }

    // Insert second param type
    treeAppendParamType(tmp, 12);
    if(tmp->fnParamTypesBuf->len != 2){
      printf("Inserted second param type but buffer length is not 2\n");
      return 1;
    }
    if(tmp->fnParamTypesBuf->data[0] != 10){
      printf("Inserted second param type but data[0] is not right\n");
      return 1;
    }
    if(tmp->fnParamTypesBuf->data[1] != 12){
      printf("Inserted second param type but data[1] is not right\n");
      return 1;
    }

    // Insert one ret type
    treeAppendRetType(tmp, 20);
    if(tmp->fnRetTypesBuf->len != 1){
      printf("Inserted one ret type but buffer length is not 1\n");
      return 1;
    }
    if(tmp->fnRetTypesBuf->data[0] != 20){
      printf("Inserted one ret type but data[0] is not right\n");
      return 1;
    }

    // Insert second ret type
    treeAppendRetType(tmp, 22);
    if(tmp->fnRetTypesBuf->len != 2){
      printf("Inserted second ret type but buffer length is not 2\n");
      return 1;
    }
    if(tmp->fnRetTypesBuf->data[0] != 20){
      printf("Inserted second ret type but data[0] is not right\n");
      return 1;
    }
    if(tmp->fnRetTypesBuf->data[1] != 22){
      printf("Inserted second ret type but data[1] is not right\n");
      return 1;
    }
  }

  // Set fn as defined
  treeSetFnData(tmp, true);
  if(!tmp->fnDefined){
    printf("Element (function) should be defined, but is not\n");
    return 1;
  }
}

int main() {
  int ret = 0; // Return value of helper functions in this file
  STTreeNode *tree = NULL; // The tree
  STElem *tmp = NULL; // Pointer to a temporary element (node) of a tree
  char c[2];
  c[1] = '\0';


  printf("Inserting the first element (a) and testing:\n");
  c[0] = 'a';
  treeInsert(&tree, c);

  c[0] = 'a';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), true)){
    /** printf("Searching for a after first elem insertion\n"); */
    return 1;
  }

  printf("Inserting the second element (c) and testing:\n");
  c[0] = 'c';
  treeInsert(&tree, c);

  c[0] = 'a';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    /** printf("Searching for a\n"); */
    return 1;
  }
  c[0] = 'c';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), true)){
    /** printf("Searching for c\n"); */
    return 1;
  }

  printf("Inserting a third element (d) and testing:\n");
  c[0] = 'd';
  treeInsert(&tree, c);

  c[0] = 'a';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'c';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'd';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), true)){
    return 1;
  }

  printf("Inserting a fourth element (b) and testing:\n");
  c[0] = 'b';
  treeInsert(&tree, c);

  c[0] = 'a';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'c';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'd';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'b';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), true)){
    return 1;
  }

  // '_' < 'a'
  printf("Inserting a fifth element (_) and testing:\n");
  c[0] = '_';
  treeInsert(&tree, c);

  c[0] = 'a';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'c';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'd';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = 'b';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), false)){
    return 1;
  }
  c[0] = '_';
  printf("Testing '%s'\n", c);
  if(testSetFunctions(treeGetData(tree, c), true)){
    return 1;
  }

  // Probably a redundant search test
  
  tmp = treeGetData(tree, c);
  if(tmp->fnRetTypesBuf->len != 2){
    printf("Inserted second ret type but buffer length is not 2\n");
    return 1;
  }
  if(tmp->fnRetTypesBuf->data[0] != 20){
    printf("Inserted second ret type but data[0] is not right\n");
    return 1;
  }
  if(tmp->fnRetTypesBuf->data[1] != 22){
    printf("Inserted second ret type but data[1] is not right\n");
    return 1;
  }
  if(!tmp->fnDefined){
    printf("Element (function) should be defined, but is not\n");
    return 1;
  }

  tmp = treeGetData(tree, c);
  if(tmp->fnRetTypesBuf->len != 2){
    printf("Inserted second ret type but buffer length is not 2\n");
    return 1;
  }
  if(tmp->fnRetTypesBuf->data[0] != 20){
    printf("Inserted second ret type but data[0] is not right\n");
    return 1;
  }
  if(tmp->fnRetTypesBuf->data[1] != 22){
    printf("Inserted second ret type but data[1] is not right\n");
    return 1;
  }
  if(!tmp->fnDefined){
    printf("Element (function) should be defined, but is not\n");
    return 1;
  }


  // Try to delete an element

  printf("Deleting 'a' - root\n");

  treeDelete(&tree, c);
  if(treeGetData(tree, c)){
    printf("Deleting a failed\n");
    return 1;
  }

  printf("Deleting 'd' - leaf\n");

  treeDelete(&tree, c);
  if(treeGetData(tree, c)){
    printf("Deleting d failed\n");
    return 1;
  }

  printf("Deleting the rest\n");

  treeDelete(&tree, c);
  if(treeGetData(tree, c)){
    printf("Deleting b failed\n");
    return 1;
  }
  treeDelete(&tree, c);
  if(treeGetData(tree, c)){
    printf("Deleting c failed\n");
    return 1;
  }
  treeDelete(&tree, c);
  if(treeGetData(tree, c)){
    printf("Deleting _ failed\n");
    return 1;
  }

  // The tree should be empty now

  if(treeGetData(tree, c)
      || treeGetData(tree, c)
      || treeGetData(tree, c)
      || treeGetData(tree, c)
      || treeGetData(tree, c)){
    printf("That's weird\n");
    return 1;
  }


  // Insert nodes again and try 'destroy'
  // The letters are random, that should be sufficient for testing purposes
  printf("Testing treeDestroy\n");
  treeInsert(&tree, "o");
  treeInsert(&tree, "u");
  treeInsert(&tree, "f");
  treeInsert(&tree, "a");
  treeInsert(&tree, "i");
  treeInsert(&tree, "b");
  treeInsert(&tree, "r");
  treeInsert(&tree, "g");
  treeInsert(&tree, "v");
  treeInsert(&tree, "j");
  treeInsert(&tree, "e");
  treeInsert(&tree, "f");
  treeInsert(&tree, "d");
  treeInsert(&tree, "m");
  // Destroy the tree
  treeDestroy(&tree);
  // Check if any element can be found
  for(c[0] = 'a'; c[0] <= 'z'; c[0] += 1){
    if(treeGetData(tree, c)){
      printf("The tree was destroyed but an element could still be found\n");
      return 1;
    }
  }
  printf("Test finished and passed if it reached this line\n");
  vypluj 0;
}