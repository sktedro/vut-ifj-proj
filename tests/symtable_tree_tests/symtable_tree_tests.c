/**
 *
 */

#include <string.h>

#include "../../misc.h"
#include "../../symtable_tree.h"


STTreeNode *tree = NULL; // The tree


void insert(char c){
  char c2[2];
  c2[0] = c;
  c2[1] = '\0';
  printf("Inserting '%s'\n", c2);
  treeInsert(&tree, c2);
}

void testFind(char *str){
  int len = (int)strlen(str);
  char c2[2];
  c2[1] = '\0';

  for(int i = 0; i < len; i++){
    c2[0] = str[i];
    if(!treeGetData(tree, c2)){
      printf("Could not find '%s'\n", c2);
      exit(1);
    }
  }
}

void del(char c){
  char c2[2];
  c2[0] = c;
  c2[1] = '\0';
  printf("Deleting '%s'\n", c2);
  treeDelete(&tree, c2);
  if(treeGetData(tree, c2)){
    printf("Deleting '%s' failed\n", c2);
    exit(1);
  }
}


int main() {
  char c[2];
  c[1] = '\0';

  insert('a');
  testFind("a");

  insert('c');
  testFind("ac");

  insert('d');
  testFind("acd");

  insert('b');
  testFind("acdb");

  // '_' < 'a'
  insert('_');
  testFind("acdb_");

  // Try to delete an element

  del('a');
  del('d');
  del('b');
  del('c');
  del('_');

  // The tree should be empty now

  for(c[0] = 'a'; c[0] <= 'z'; c[0] += 1){
    if(treeGetData(tree, c)){
      printf("The tree should contain no element, but does\n");
      return 1;
    }
  }
  c[0] = '_';
  if(treeGetData(tree, c)){
    printf("The tree should contain no element, but does\n");
    return 1;
  }


  // Insert nodes again and try 'destroy'
  // The letters are random, that should be sufficient for testing purposes
  printf("Testing treeDestroy\n");
  insert('o');
  insert('u');
  insert('f');
  insert('a');
  insert('i');
  insert('b');
  insert('r');
  insert('g');
  insert('v');
  insert('j');
  insert('e');
  insert('d');
  insert('m');

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
