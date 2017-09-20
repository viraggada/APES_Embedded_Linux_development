/*********************************************************************************************
* Authors : Virag Gada
* Date : 09/06/2017
*
* File : linkedList.c
* Description : Header file for Linked List functions
*               -linkList_state insert_first_node (nodeStruct **head,uint32_t data)
*               -linkList_state push_node (nodeStruct **head,uint32_t data)
*               -linkList_state append_node (nodeStruct **head,uint32_t data)
*               -linkList_state add_node (nodeStruct **head,uint32_t data,uint32_t index)
*               -linkList_state remove_node (nodeStruct **head,uint32_t index, uint32_t *data)
*               -linkList_state search_node (nodeStruct **node,uint32_t data,uint32_t *index)
*               -linkList_state destroy_list (nodeStruct **list)
*               -void dump_list (nodeStruct **node)
*               -uint32_t list_size (nodeStruct **head)
* Tool : Compiler - GCC, Linker - GDB
**********************************************************************************************/

#include<stdlib.h>
#include<stdint.h>

#ifndef SOURCES_LINKEDLIST_H_
#define SOURCES_LINKEDLIST_H_

typedef enum list_states{
  SUCCESS,
  FAILURE,
  NULL_PTR
}linkList_state;

typedef struct linkNodeStruct{
  uint32_t data;
  struct linkNodeStruct *prev;
  struct linkNodeStruct *next;
}nodeStruct;

/******************************************************************************
* This function is used to insert the first node in a Linked list
* The return value is the state of node allocation
* nodeStruct **node - Pointer to the linked list head structure
* uint32_t data - Data to be addded
* linkList_state(return) - Status Enumeration
*******************************************************************************/
linkList_state insert_first_node (nodeStruct **node,uint32_t data);

/******************************************************************************
* This function is used to insert a node at the first position in a linked list
* If list does not exist it will create one
* The return value is the state of node allocation
* nodeStruct **node - Pointer to the linked list head structure
* uint32_t data - Data to be addded
* linkList_state(return) - Status Enumeration
*******************************************************************************/
linkList_state push_node (nodeStruct **head,uint32_t data);

/******************************************************************************
* This function is used to insert a node at the end of linked list
* If list does not exist it will create one
* The return value is the state of node allocation
* nodeStruct **node - Pointer to the linked list head structure
* uint32_t data - Data to be addded
* linkList_state(return) - Status Enumeration
*******************************************************************************/
linkList_state append_node (nodeStruct **head,uint32_t data);

/******************************************************************************
* This function is used to insert a node at the nth position in a linked list
* If list does not exist it will create one
* The return value is the state of node allocation
* nodeStruct **node - Pointer to the linked list head structure
* uint32_t data - Data to be addded
* uint32_t index - Position where node is to be added
* linkList_state(return) - Status Enumeration
*******************************************************************************/
linkList_state add_node (nodeStruct **node,uint32_t data,uint32_t index);

/******************************************************************************
* This function is used to remove all the nodes in a linked list
* The return value is the state of list deletion
* nodeStruct **node - Pointer to the linked list head structure
* linkList_state(return) - Status Enumeration
*******************************************************************************/
linkList_state destroy_list (nodeStruct **node);

/******************************************************************************
* This function is used to remove a node from the first position in a linked list
* The return value is the state of node removal
* nodeStruct **node - Pointer to the linked list head structure
* uint32_t index - Position where the node is to be removed
* uint32_t *data - Pointer to the loaction where the data from the removed node
*                  needs to be stored
* linkList_state(return) - Status Enumeration
*******************************************************************************/
linkList_state remove_node (nodeStruct **head,uint32_t index,uint32_t *data);

/******************************************************************************
* This function is used to search the first data location in the linked list
* The return value is the state of node location
* nodeStruct **node - Pointer to the linked list head structure
* uint32_t data - The data to be found
* uint32_t *index = Pointer to the index where the data is stroed in list
* linkList_state(return) - Status Enumeration
*******************************************************************************/
linkList_state search_node (nodeStruct **node,uint32_t data,uint32_t *index);

/******************************************************************************
* This function is used to find the size of linked list
* The return value is the size of the list
* nodeStruct **node - Pointer to the linked list head structure
* uint32_t(return) - Size of the list
*******************************************************************************/
uint32_t list_size (nodeStruct **head);

/******************************************************************************
* This function is used to print data of all nodes in the linked list
* nodeStruct **node - Pointer to the linked list head structure
*******************************************************************************/
void dump_list (nodeStruct **node);

#endif /* SOURCES_CIRCBUFF_H_ */
