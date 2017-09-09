/***************************************************************************************************
* Authors : Virag Gada
* Date : 09/06/2017
*
* File : linkedList.c
* Description : Source file for Linked List functions
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
* Code modified from - https://github.com/viraggada/ese-repo/blob/master/project4/SOURCES/circbuff.c
***************************************************************************************************/

#include <stdio.h>
#include "linkedList.h"

/*This function is used to insert the first node in a Linked list*/
linkList_state insert_first_node (nodeStruct **head,uint32_t data)
{ /*Check for NULL pointer else this can't be first node*/
  if(*head == NULL)
  { /*For first node, next and previous pointers will be NULL*/
    nodeStruct *node = (nodeStruct *)malloc(sizeof(nodeStruct));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    *head = node;
    return SUCCESS;
  }
  return FAILURE;
}

/*This function is used to insert a node at the first position in a linked list*/
linkList_state push_node (nodeStruct **head,uint32_t data)
{
  nodeStruct *new_head = (nodeStruct *)malloc(sizeof(nodeStruct));
  /*Check if memory is not allocated*/
  if(new_head == NULL)
  {
    return FAILURE;
  }

  /*To add new head node make this new node point to the current head*/
  new_head->data = data;
  new_head->next = *head;
  new_head->prev = NULL;

  /*If there is a head node then make it point to this new node*/
  if((*head)!=NULL)
  {
    (*head)->prev = new_head;
  }

  /*Make this as your new head node*/
  *head = new_head;
  return SUCCESS;
}

/*This function is used to insert a node at the end of linked list*/
linkList_state append_node (nodeStruct **head,uint32_t data)
{
  nodeStruct *new_tail = (nodeStruct *)malloc(sizeof(nodeStruct));
  /*Check if memory is not allocated*/
  if(new_tail == NULL)
  {
    return FAILURE;
  }
  nodeStruct *node_copy = *head;
  new_tail->data = data;
  new_tail->next = NULL;
  if(*head == NULL)
  { /*if there is no head node then make this as the head node*/
    new_tail->prev = NULL;
    *head = new_tail;
    return SUCCESS;
  }
  /*Go till the current tail to append this node*/
  while(node_copy->next!=NULL)
  {
    node_copy = node_copy->next;
  }
  node_copy->next = new_tail;
  new_tail->prev = node_copy;
  return SUCCESS;
}

/*This function is used to insert a node at the nth position in a linked list*/
linkList_state add_node (nodeStruct **head,uint32_t data,uint32_t index)
{
  linkList_state state;
  /*Check if the index is invalid*/
  if(index > list_size(head)+1)
  {
    return FAILURE;
  }
  /*Check if *head==NULL - New list
   *index ==1 - first node in this list */
  if((*head == NULL) || (index == 1))
  {
    state = push_node(head,data);
  }else
  { /*If node has to be added in between or at the end*/
    nodeStruct *copy_node = *head;
    uint32_t i;
    /*Traverse till the index is reached or last node*/
    for(i=2;(i<index) && (copy_node->next!=NULL);i++)
    {
      copy_node = copy_node->next;
    }
    /*If last node needs to be added call append function*/
    if(copy_node->next == NULL)
    {
      state = append_node(&copy_node,data);
    }else
    { /*Add node in between*/
      nodeStruct *node = (nodeStruct *)malloc(sizeof(nodeStruct));
      /*Check if memory is not allocated*/
      if(node == NULL)
      {
        state = FAILURE;
      }else
      {
        node->data = data;
        node->next = copy_node->next;
        node->prev = copy_node;
        node->next->prev = copy_node;
        copy_node->next = node;
        state = SUCCESS;
      }
    }
  }
  return state;
}

/*This function is used to find the size of linked list*/
uint32_t list_size (nodeStruct **head)
{ /*If this is an invalid pointer return 0*/
  if(*head == NULL)
  {
    return 0;
  }
  nodeStruct *node = *head;
  uint32_t count = 0;
  /*Go till the last node*/
  do
  {
    count++;
    node = node->next;
  }while(node != NULL);
  return count;
}


/*This function is used to remove all the nodes in a linked list*/
linkList_state destroy_list (nodeStruct **list)
{ /*Check if list pointer is invalid*/
  if(*list == NULL)
  {
    return NULL_PTR;
  }
  nodeStruct *node = *list;
  nodeStruct *next_node;
  /*Traverse the list and free all nodes*/
  while(node!=NULL)
  {
    next_node = node->next;
    free(node);
    node = next_node;
  }
  return SUCCESS;
}


/*This function is used to remove a node from the first position in a linked list*/
linkList_state remove_node (nodeStruct **head,uint32_t index, uint32_t *data)
{
  linkList_state state;
  /*Check if list is invalid*/
  if(*head == NULL)
  {
    return NULL_PTR;
  }
  /*Check if index is invalid*/
  if(index > list_size(head))
  {
    state = FAILURE;
  }else
  {
    nodeStruct *node_copy = *head;
    if(index == 1)
    {
      *data = (node_copy)->data;
      (node_copy)->next->prev = NULL;
      *head = node_copy->next;
    }else
    {
      uint32_t i;
      /*Traverse the list and remove the nth node*/
      for(i=1;(i<index) && (node_copy->next!=NULL);i++)
      {
        node_copy = node_copy->next;
      }
      *data = node_copy->data;
      node_copy->prev->next = node_copy->next;
      /*If this is not the last node them make the (n+1)th node
        point to the (n-1)th node*/
      if(node_copy->next != NULL)
      {
        node_copy->next->prev = node_copy->prev;
      }
    }
    free(node_copy);
    state = SUCCESS;
  }
  return state;
}


/*This function is used to print data of all nodes in the linked list*/
void dump_list (nodeStruct **node)
{
  uint32_t index = 1;
  nodeStruct *copy = *node;
  /*Traverse through the list and print data*/
  while(copy != NULL)
  {
    printf("Element at %d is %d\n",index++,copy->data);
    copy = copy->next;
  }
}


/*This function is used to search the first data location in the linked list*/
linkList_state search_node (nodeStruct **node,uint32_t data,uint32_t *index)
{
  /*Check if pointer is invalid*/
  if(*node == NULL)
  {
    return NULL_PTR;
  }
  linkList_state state;
  nodeStruct *node_copy = *node;
  uint32_t count = 1;
  /*Traverse through the list to find the data*/
  while((node_copy->data != data))
  {
    node_copy = node_copy->next;
    count++;
  }
  *index = count;
  /*If data is not present return FAILURE*/
  if(*index == 0)
  {
    state = FAILURE;
  }else
  {
    state = SUCCESS;
  }
  return state;
}
