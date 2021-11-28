#include "../../misc.h"
#include "../../symtable.h"


// Check if a frame exists and has the right depth
int checkFrame(STStack *stack, int depth){
  if(stack->top->depth != depth){
    printf("A frame on top of the stack does not have the right depth %d\n", depth);
    return 1;
  }
  return 0;
}

// Check if a node exists (search for it)
int checkNode(STStack *stack, char *key){
  if(!STFind(stack, key)){
    printf("A node with key %s could not be found\n", key);
    return 1;
  }
  return 0;
}


int main(){
  printf("Message is an error message only if it is the last one from the output and the return value != 0\n");

  // Stack init
  STStack *stack = NULL;
  STInit(&stack);
  if(!stack){
    printf("Error while initializing\n");
    return 1;
  }

  // Push first frame
  if(STPush(stack)){
    printf("Error while pushing\n");
    return 1;
  }
  if(!stack || !stack->top){
    printf("First frame should be pushed but probably isn't\n");
    return 1;
  }
  if(checkFrame(stack, 0)){
    return 1;
  }
  if(stack->top->table){
    printf("First frame pushed and should have no items, but does have\n");
    return 1;
  }
  if(stack->top->next){
    printf("First frame pushed but 'next' ain't null\n");
    return 1;
  }

  // Pop an only frame
  STPop(stack);
  if(!stack){
    printf("Stack was destroyed when popping a frame\n");
    return 1;
  }
  if(stack->top){
    printf("Frame should be popped but stack->top is not NULL\n");
    return 1;
  }


  // Push a frame
  if(STPush(stack)){
    printf("Error while pushing a frame\n");
    return 1;
  }
  if(!stack || !stack->top){
    printf("One frame should be pushed but probably isn't\n");
    return 1;
  }
  if(checkFrame(stack, 0)){
    return 1;
  }
  if(stack->top->table){
    printf("First frame pushed and should have no items, but does have\n");
    return 1;
  }
  if(stack->top->next){
    printf("One frame pushed but 'next' ain't null\n");
    return 1;
  }

  // Insert two nodes
  if(STInsert(stack, "abc")){
    printf("Error while inserting 'abc'\n");
    return 1;
  }
  if(STInsert(stack, "def")){
    printf("Error while inserting 'def'\n");
    return 1;
  }
  // Check if they were successfuly inserted and can be found
  if(checkNode(stack, "abc") || checkNode(stack, "def")){
    return 1;
  }
  // Check if we are able to find a non existant node
  if(!checkNode(stack, "ghi")){
    printf("A node that should not exist was found\n");
    return 1;
  }


  // Push a second frame
  if(STPush(stack)){
    printf("Error while pushing a frame\n");
    return 1;
  }
  // Check if the frame with depth 1 is on top
  if(checkFrame(stack, 1)){
    printf("Top should be a frame with depth 1 but is not\n");
    return 1;
  }
  // Insert two new nodes
  if(STInsert(stack, "a")){
    printf("Error while inserting 'a'\n");
    return 1;
  }
  if(STInsert(stack, "b")){
    printf("Error while inserting 'b'\n");
    return 1;
  }
  // Check if they can be found
  if(checkNode(stack, "a") || checkNode(stack, "b")){
    return 1;
  }
  // Check if we can find nodes from the first frame
  if(checkNode(stack, "abc") || checkNode(stack, "def")){
    return 1;
  }


  // Push a third frame (depth = 2)
  if(STPush(stack)){
    printf("Error while pushing a frame\n");
    return 1;
  }
  // Check if the frame with depth 2 is on top
  if(checkFrame(stack, 2)){
    return 1;
  }
  // Check if nodes from previous frames can be found
  if(checkNode(stack, "abc") || checkNode(stack, "def")){
    return 1;
  }
  if(checkNode(stack, "a") || checkNode(stack, "b")){
    return 1;
  }
  // Insert one node
  if(STInsert(stack, "1")){
    printf("Error while inserting '1'\n");
    return 1;
  }
  // Check if it can be found
  if(checkNode(stack, "1")){
    return 1;
  }
  // Insert another node
  if(STInsert(stack, "2")){
    printf("Error while inserting '2'\n");
    return 1;
  }
  // Check if both can be found
  if(checkNode(stack, "1")){
    return 1;
  }
  if(checkNode(stack, "2")){
    return 1;
  }
  // Insert few more nodes
  if(STInsert(stack, "5")){
    printf("Error while inserting '3'\n");
    return 1;
  }
  if(STInsert(stack, "4")){
    printf("Error while inserting '3'\n");
    return 1;
  }
  if(STInsert(stack, "3")){
    printf("Error while inserting '3'\n");
    return 1;
  }
  if(STInsert(stack, "6")){
    printf("Error while inserting '3'\n");
    return 1;
  }
  if(STInsert(stack, "7")){
    printf("Error while inserting '3'\n");
    return 1;
  }
  // Check if all of them can be found
  if(checkNode(stack, "1")
      || checkNode(stack, "2")
      || checkNode(stack, "3")
      || checkNode(stack, "4")
      || checkNode(stack, "5")
      || checkNode(stack, "6")
      || checkNode(stack, "7")){
    return 1;
  }


  // Pop a frame
  STPop(stack);
  // Check if the frame with depth 1 is on top
  if(checkFrame(stack, 1)){
    return 1;
  }
  // Check if number nodes can be found (from frame 3)
  if(!checkNode(stack, "1")
      || !checkNode(stack, "2")
      || !checkNode(stack, "3")
      || !checkNode(stack, "4")
      || !checkNode(stack, "5")
      || !checkNode(stack, "6")
      || !checkNode(stack, "7")){
    return 1;
  }
  // But nodes from frame 1 and 2 should still be there
  if(checkNode(stack, "abc") || checkNode(stack, "def")){
    return 1;
  }
  if(checkNode(stack, "a") || checkNode(stack, "b")){
    return 1;
  }


  // Pop a second frame
  STPop(stack);
  // Check if the frame with depth 0 is on top
  if(checkFrame(stack, 0)){
    return 1;
  }
  // Nothing but nodes from frame 1 should be found now
  if(checkNode(stack, "abc") || checkNode(stack, "def")){
    return 1;
  }
  if(!checkNode(stack, "a") || !checkNode(stack, "b")){
    return 1;
  }
  if(!checkNode(stack, "1")
      || !checkNode(stack, "2")
      || !checkNode(stack, "3")
      || !checkNode(stack, "4")
      || !checkNode(stack, "5")
      || !checkNode(stack, "6")
      || !checkNode(stack, "7")){
    return 1;
  }


  // Pop the last frame
  STPop(stack);
  // Stack should have empty top
  if(stack->top){
    printf("Last frame popped but stack has a top\n");
    return 1;
  }
  // No nodes should still exist
  if(!checkNode(stack, "abc") || !checkNode(stack, "def")){
    return 1;
  }
  if(!checkNode(stack, "a") || !checkNode(stack, "b")){
    return 1;
  }
  if(!checkNode(stack, "1")
      || !checkNode(stack, "2")
      || !checkNode(stack, "3")
      || !checkNode(stack, "4")
      || !checkNode(stack, "5")
      || !checkNode(stack, "6")
      || !checkNode(stack, "7")){
    return 1;
  }



  // Done testing basic operations
  printf("Basic operations (init, push, pop, insert, find) working\n");



  // Push a frame
  STPush(stack);
  // Insert two nodes - var and fn
  STInsert(stack, "var");
  STInsert(stack, "fn");
  // Set its data
  STSetIsVariable(stack, "var", true);
  STSetIsVariable(stack, "fn", false);
  STSetVarDataType(stack, "var", 2);
  STSetVarAddress(stack, "var", 3);
  STSetFnDefined(stack, "fn", false);
  STAppendParamType(stack, "fn", 4);
  STAppendRetType(stack, "fn", 5);
  // Check its data
  if(STGetDepth(stack, "var") != 0){
    printf("Depth of 'var' is wrong\n");
    return 1;
  }
  if(STGetIsVariable(stack, "var") != true){
    printf("IsVariable of 'var' is wrong\n");
    return 1;
  }
  if(STGetVarDataType(stack, "var") != 2){
    printf("Data type of 'var' is wrong\n");
    return 1;
  }
  if(STGetVarAddress(stack, "var") != 3){
    printf("Address of 'var' is wrong\n");
    return 1;
  }
  if(STGetDepth(stack, "fn") != 0){
    printf("Depth of 'fn' is wrong\n");
    return 1;
  }
  if(STGetIsVariable(stack, "fn") != false){
    printf("IsVariable of 'fn' is wrong\n");
    return 1;
  }
  if(STGetFnDefined(stack, "fn") != false){
    printf("FnDefined of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 0) != 4){
    printf("Param[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 1) != -1){
    printf("Param[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 0) != 5){
    printf("Ret[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 1) != -1){
    printf("Ret[1] of 'fn' is wrong\n");
    return 1;
  }
  // Modify its data and check it 
  STSetVarDataType(stack, "var", 22);
  if(STGetVarDataType(stack, "var") != 22){
    printf("Data type of 'var' is wrong\n");
    return 1;
  }
  STSetVarAddress(stack, "var", 33);
  if(STGetVarAddress(stack, "var") != 33){
    printf("Address of 'var' is wrong\n");
    return 1;
  }
  STSetFnDefined(stack, "fn", true);
  if(STGetFnDefined(stack, "fn") != true){
    printf("FnDefined of 'fn' is wrong\n");
    return 1;
  }
  // Append param types and test
  STAppendParamType(stack, "fn", 44);
  if(STGetParamType(stack, "fn", 0) != 4){
    printf("Param[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 1) != 44){
    printf("Param[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 2) != -1){
    printf("Param[2] of 'fn' is wrong\n");
    return 1;
  }
  STAppendParamType(stack, "fn", 444);
  if(STGetParamType(stack, "fn", 0) != 4){
    printf("Param[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 1) != 44){
    printf("Param[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 2) != 444){
    printf("Param[2] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 3) != -1){
    printf("Param[3] of 'fn' is wrong\n");
    return 1;
  }
  // Append ret types and test
  STAppendRetType(stack, "fn", 55);
  if(STGetRetType(stack, "fn", 0) != 5){
    printf("Ret[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 1) != 55){
    printf("Ret[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 2) != -1){
    printf("Ret[2] of 'fn' is wrong\n");
    return 1;
  }

  // Push a new frame
  STPush(stack);
  // Insert new nodes and set their props
  STInsert(stack, "1");
  STInsert(stack, "2");
  STInsert(stack, "0");
  STSetIsVariable(stack, "0", true);
  STSetIsVariable(stack, "1", true);
  STSetIsVariable(stack, "2", false);
  STSetVarDataType(stack, "0", 2);
  STSetVarDataType(stack, "1", 3);
  STSetVarAddress(stack, "0", 4);
  STSetVarAddress(stack, "1", 5);
  STSetFnDefined(stack, "2", false);
  STAppendParamType(stack, "2", 6);
  STAppendRetType(stack, "2", 7);
  // Check if their props are right
  if(STGetDepth(stack, "0") != 1
    || STGetDepth(stack, "1") != 1
    || STGetDepth(stack, "2") != 1){
    printf("Depth of '0/1/2' is wrong\n");
    return 1;
  }
  if(STGetIsVariable(stack, "0") != true
    || STGetIsVariable(stack, "1") != true
    || STGetIsVariable(stack, "2") != false){
    printf("IsVariable of 0/1/2 is wrong\n");
    return 1;
  }
  if(STGetVarDataType(stack, "0") != 2
      || STGetVarDataType(stack, "1") != 3){
    printf("Data type of 0/1 is wrong\n");
    return 1;
  }
  if(STGetVarAddress(stack, "0") != 4
      || STGetVarAddress(stack, "1") != 5){
    printf("Address of 0/1 is wrong\n");
    return 1;
  }
  if(STGetFnDefined(stack, "2") != false){
    printf("FnDefined of '2' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "2", 0) != 6){
    printf("Param[0] of '2' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "2", 0) != 7){
    printf("Ret[0] of '2' is wrong\n");
    return 1;
  }
  // Check if nodes from frame 1 (depth = 0) can be found and their props are
  // right
  if(STGetDepth(stack, "var") != 0){
    printf("Depth of 'var' is wrong\n");
    return 1;
  }
  if(STGetDepth(stack, "fn") != 0){
    printf("Depth of 'fn' is wrong\n");
    return 1;
  }
  if(STGetIsVariable(stack, "var") != true){
    printf("IsVariable of 'var' is wrong\n");
    return 1;
  }
  if(STGetIsVariable(stack, "fn") != false){
    printf("IsVariable of 'fn' is wrong\n");
    return 1;
  }
  if(STGetVarDataType(stack, "var") != 22){
    printf("Data type of 'var' is wrong\n");
    return 1;
  }
  if(STGetVarAddress(stack, "var") != 33){
    printf("Address of 'var' is wrong\n");
    return 1;
  }
  if(STGetFnDefined(stack, "fn") != true){
    printf("FnDefined of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 0) != 4){
    printf("Param[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 1) != 44){
    printf("Param[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 2) != 444){
    printf("Param[2] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 3) != -1){
    printf("Param[3] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 0) != 5){
    printf("Ret[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 1) != 55){
    printf("Ret[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 2) != -1){
    printf("Ret[2] of 'fn' is wrong\n");
    return 1;
  }

  // Pop the second frame
  STPop(stack);

  // Check if we can still reach nodes from the first frame
  if(STGetDepth(stack, "var") != 0){
    printf("Depth of 'var' is wrong\n");
    return 1;
  }
  if(STGetDepth(stack, "fn") != 0){
    printf("Depth of 'fn' is wrong\n");
    return 1;
  }
  if(STGetIsVariable(stack, "var") != true){
    printf("IsVariable of 'var' is wrong\n");
    return 1;
  }
  if(STGetIsVariable(stack, "fn") != false){
    printf("IsVariable of 'fn' is wrong\n");
    return 1;
  }
  if(STGetVarDataType(stack, "var") != 22){
    printf("Data type of 'var' is wrong\n");
    return 1;
  }
  if(STGetVarAddress(stack, "var") != 33){
    printf("Address of 'var' is wrong\n");
    return 1;
  }
  if(STGetFnDefined(stack, "fn") != true){
    printf("FnDefined of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 0) != 4){
    printf("Param[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 1) != 44){
    printf("Param[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 2) != 444){
    printf("Param[2] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetParamType(stack, "fn", 3) != -1){
    printf("Param[3] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 0) != 5){
    printf("Ret[0] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 1) != 55){
    printf("Ret[1] of 'fn' is wrong\n");
    return 1;
  }
  if(STGetRetType(stack, "fn", 2) != -1){
    printf("Ret[2] of 'fn' is wrong\n");
    return 1;
  }

  // Pop the last frame
  STPop(stack);

  // Check if a node can be found
  if(!checkNode(stack, "a")
    || !checkNode(stack, "b")
    || !checkNode(stack, "abc")
    || !checkNode(stack, "def")
    || !checkNode(stack, "1")
    || !checkNode(stack, "2")
    || !checkNode(stack, "3")
    || !checkNode(stack, "")){
    printf("A node was found but none should exist");
    return 1;
  }
  // Check if a frame exists
  if(stack->top){
    printf("A frame exists but should not\n");
    return 1;
  }

  printf("Symtable tests finished\n");
  return 0;
}

