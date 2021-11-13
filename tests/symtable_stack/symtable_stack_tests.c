#include "../../misc.h"
#include "../../symtable_stack.h"


int main(int argc, char *argv[]){
  // Create three elements of the stack - trees
  STTreeNode *tree1 = NULL, *tree2 = NULL, *tree3 = NULL;


  // Initialize the stack
  STStack *stack = STStackInit();
  if(!stack){
    printf("Couldn't allocate\n");
    return 1;
  }
  if(stack->top){
    printf("Init doesn't initialize stack top to NULL\n");
    return 1;
  }
  

  // Try to get elements of an empty stack
  if(STStackTop(stack)){
    printf("Top is not null while stack is empty\n");
    return 1;
  }
  if(STStackBottom(stack)){
    printf("Bottom is not null while stack is empty\n");
    return 1;
  }
  if(STStackNthElem(stack, 0) || STStackNthElem(stack, 1)){
    printf("Nth elem is not null while stack is empty\n");
    return 1;
  }


  // Push one element and test all functions
  treeInsert(&tree1, "abc");
  STStackPush(stack, tree1, 0);
  if(stack->top->depth != 0 || stack->top->table != tree1){
    printf("Element pushed is not on the stack\n");
    return 1;
  }
  if(stack->top->next){
    printf("Only one element was pushed but its 'next' pointer ain't NULL\n");
    return 1;
  }
  if(STStackTop(stack)->table != tree1){
    printf("Top doesn't return the right element (stack size = 1)\n");
    return 1;
  }
  if(STStackBottom(stack)->table != tree1){
    printf("Bottom doesn't return the right element (stack size = 1)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0)->table != tree1){
    printf("Nth doesn't return the right element (stack size = 1)\n");
    return 1;
  }
  // Pop the only element in the stack
  STStackPop(stack);
  if(stack->top){
    printf("Stack pop unsuccessful\n");
    return 1;
  }
  tree1 = NULL;


  // Push one element and test all functions again
  treeInsert(&tree1, "abc");
  STStackPush(stack, tree1, 0);
  if(stack->top->depth != 0 || stack->top->table != tree1){
    printf("Element pushed is not on the stack\n");
    return 1;
  }
  if(stack->top->next){
    printf("Only one element was pushed but its 'next' pointer ain't NULL\n");
    return 1;
  }
  if(STStackTop(stack)->table != tree1){
    printf("Top doesn't return the right element (stack size = 1, test 2)\n");
    return 1;
  }
  if(STStackBottom(stack)->table != tree1){
    printf("Bottom doesn't return the right element (stack size = 1, test 2)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0)->table != tree1){
    printf("Nth doesn't return the right element (stack size = 1, test 2)\n");
    return 1;
  }
  // Destroy the stack with only one element
  STStackDestroy(&stack);
  if(stack){
    printf("Destroying the stack unsuccessful\n");
    return 1;
  }
  tree1 = NULL;

  // Insert two elements and test all functions
  stack = STStackInit();
  treeInsert(&tree1, "abc");
  treeInsert(&tree2, "def");
  STStackPush(stack, tree1, 0);
  STStackPush(stack, tree2, 1);
  if(STStackTop(stack)->table != tree2){
    printf("Top doesn't return the right element (stack size = 2)\n");
    return 1;
  }
  if(STStackBottom(stack)->table != tree1){
    printf("Bottom doesn't return the right element (stack size = 2)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0)->table != tree2 || STStackNthElem(stack, 1)->table != tree1){
    printf("Nth doesn't return the right element (stack size = 2)\n");
    return 1;
  }
  // Destroy a stack with two elements
  STStackDestroy(&stack);
  if(stack){
    printf("Destroying the stack unsuccessful (stack size was 2)\n");
    return 1;
  }
  tree1 = tree2 = NULL;

  // Insert three elements and test all functions
  stack = STStackInit();
  treeInsert(&tree1, "abc");
  treeInsert(&tree2, "def");
  treeInsert(&tree3, "ghi");
  STStackPush(stack, tree1, 0);
  STStackPush(stack, tree2, 1);
  STStackPush(stack, tree3, 2);
  if(STStackTop(stack)->table != tree3){
    printf("Top doesn't return the right element (stack size = 3)\n");
    return 1;
  }
  if(STStackTop(stack)->depth != 2){
    printf("Elements depth is wrong (Top)\n");
    return 1;
  }
  if(STStackBottom(stack)->table != tree1){
    printf("Bottom doesn't return the right element (stack size = 3)\n");
    return 1;
  }
  if(STStackBottom(stack)->depth != 0){
    printf("Elements depth is wrong (Bottom)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0)->table != tree3 
      || STStackNthElem(stack, 1)->table != tree2
      || STStackNthElem(stack, 2)->table != tree1){
    printf("Nth doesn't return the right element (stack size = 3)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0)->depth != 2
      || STStackNthElem(stack, 1)->depth != 1
      || STStackNthElem(stack, 2)->depth != 0){
    printf("Elements depth is wrong (NthElem)\n");
    return 1;
  }
  // Pop one
  STStackPop(stack);
  if(STStackTop(stack)->table != tree2){
    printf("Top doesn't return the right element (stack size = 2 after pop)\n");
    return 1;
  }
  if(STStackBottom(stack)->table != tree1){
    printf("Bottom doesn't return the right element (stack size = 2 after pop)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0)->table != tree2 
      || STStackNthElem(stack, 1)->table != tree1){
    printf("Nth doesn't return the right element (stack size = 2 after pop)\n");
    return 1;
  }
  // Pop a second one
  STStackPop(stack);
  if(STStackTop(stack)->table != tree1){
    printf("Top doesn't return the right element (stack size = 1 after pop)\n");
    return 1;
  }
  if(STStackBottom(stack)->table != tree1){
    printf("Bottom doesn't return the right element (stack size = 1 after pop)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0)->table != tree1){
    printf("Nth doesn't return the right element (stack size = 1 after pop)\n");
    return 1;
  }
  // Pop the last one
  STStackPop(stack);
  // Try to get elements of an empty stack
  if(STStackTop(stack)){
    printf("Top is not null while stack is empty (stack size = 0 after pop)\n");
    return 1;
  }
  if(STStackBottom(stack)){
    printf("Bottom is not null while stack is empty (stack size = 0 after pop)\n");
    return 1;
  }
  if(STStackNthElem(stack, 0) || STStackNthElem(stack, 1)){
    printf("Nth elem is not null while stack is empty (stack size = 0 after pop)\n");
    return 1;
  }


}
