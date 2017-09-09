#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "circbuff.h"
#include "linkedList.h"

uint32_t data[4] = {0xA,0xB,0xC,0xD};
char cbuff_message[25][25] = {"NO_ERROR","ERROR","BUFFER_FULL","BUFFER_EMPTY","NULL_POINTER",
	                           "NO_LENGTH","BUFFER_ALLCATION_FAILURE","AVAILABLE"};
char llist_message[10][10] = {"SUCCESS","FAILURE","NULL_PTR"};
int main(int argc, char *argv[])
{
  printf("Testing Double Linked list\n");
  uint32_t ind;
  nodeStruct *first_node = NULL;
  linkList_state state1 = add_node(&first_node,data[0],1);
  printf("Insert first - %s\n",llist_message[state1]);
  printf("Length %d\n",list_size(&first_node));
  dump_list(&first_node);
  state1 = add_node(&first_node,data[2],2);
  printf("Add node - %s\n",llist_message[state1]);
  printf("Length %d\n",list_size(&first_node));
  dump_list(&first_node);
  state1 = add_node(&first_node,data[3],3);
  printf("Add node - %s\n",llist_message[state1]);
  printf("Length %d\n",list_size(&first_node));
  dump_list(&first_node);
  state1 = add_node(&first_node,23,4);
  printf("Add node - %s\n",llist_message[state1]);
  dump_list(&first_node);
  state1 = add_node(&first_node,5,9);
  printf("Length %d\n",list_size(&first_node));
  printf("Add node - %s\n",llist_message[state1]);
  dump_list(&first_node);
  state1 = remove_node(&first_node,3,&ind);
  printf("Remove mode - %s , data %d\n",llist_message[state1], ind);
  dump_list(&first_node);
  printf("Length %d\n",list_size(&first_node));
  state1 = destroy_list(&first_node);
  printf("Destroy - %s\n",llist_message[state1]);
  /**************************************************************/

  printf("\nTesting Circular Buffer\n");
  uint32_t length = BUFFER_SIZE;
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state state = cbuffer_memoryAllocate(&cBuffStruct,length);
  printf("Allocation State %s\n", cbuff_message[state]);
  if(state == ERROR){
    exit(-1);
  }
  cbuffer_init(&cBuffStruct,length);
  state = cbuffer_add(&cBuffStruct,data);
  printf("Add State %s\n", cbuff_message[state]);
  uint32_t size = cbuffer_size(&cBuffStruct);
  printf("Size = %d\n",size);
  state = cbuffer_dump(&cBuffStruct);
  printf("Dump State %s\n", cbuff_message[state]);
  state = cbuffer_full(&cBuffStruct);
  printf("Full State %s\n", cbuff_message[state]);
  state = cbuffer_add(&cBuffStruct,data+1);
  printf("Add State %s\n", cbuff_message[state]);
  size = cbuffer_size(&cBuffStruct);
  printf("Size = %d\n",size);
  state = cbuffer_dump(&cBuffStruct);
  printf("Dump State %s\n", cbuff_message[state]);
  state = cbuffer_full(&cBuffStruct);
  printf("Full State %s\n", cbuff_message[state]);
  state = cbuffer_add(&cBuffStruct,data+2);
  printf("Add State %s\n", cbuff_message[state]);
  size = cbuffer_size(&cBuffStruct);
  printf("Size = %d\n",size);
  state = cbuffer_dump(&cBuffStruct);
  printf("Dump State %s\n", cbuff_message[state]);
  state = cbuffer_full(&cBuffStruct);
  printf("Full State %s\n", cbuff_message[state]);
  state = cbuffer_remove(&cBuffStruct);
  printf("Remove State %s\n", cbuff_message[state]);
  size = cbuffer_size(&cBuffStruct);
  printf("Size = %d\n",size);
  state = cbuffer_dump(&cBuffStruct);
  printf("Dump State %s\n", cbuff_message[state]);
  state = cbuffer_empty(&cBuffStruct);
  printf("Empty State %s\n", cbuff_message[state]);
  state = cbuffer_remove(&cBuffStruct);
  printf("Remove State %s\n", cbuff_message[state]);
  size = cbuffer_size(&cBuffStruct);
  printf("Size = %d\n",size);
  state = cbuffer_dump(&cBuffStruct);
  printf("Dump State %s\n", cbuff_message[state]);
  state = cbuffer_empty(&cBuffStruct);
  printf("Empty State %s\n", cbuff_message[state]);
  state = cbuffer_remove(&cBuffStruct);
  printf("Remove State %s\n", cbuff_message[state]);
  size = cbuffer_size(&cBuffStruct);
  printf("Size = %d\n",size);
  state = cbuffer_dump(&cBuffStruct);
  printf("Dump State %s\n", cbuff_message[state]);
  state = cbuffer_empty(&cBuffStruct);
  printf("Empty State %s\n", cbuff_message[state]);
  state = cbuffer_destroy(&cBuffStruct);
  printf("Destroy State %s\n", cbuff_message[state]);
  return 0;
}
