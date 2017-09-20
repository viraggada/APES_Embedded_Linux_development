/************************************************************************
* Authors: Virag Gada
* Date: 09/15/2017
* File: test_hw2.c
* Description : Source file for testing circular buffer and linked list functions
*               -test_circbuff_allocate_free()
*               -test_circbuff_add_invalid_pointer()
*               -test_circbuff_add_over_fill()
*               -test_circbuff_add_available()
*               -test_circbuff_buff_full()
*               -test_circbuff_full_invalid_pointer()
*               -test_circbuff_full_available()
*               -test_circbuff_remove_invalid_pointer()
*               -test_circbuff_remove_over_remove()
*               -test_circbuff_remove_available()
*               -test_circbuff_empty_invalid_pointer()
*               -test_circbuff_empty_available()
*               -test_circbuff_add_wrap()
*               -test_circbuff_remove_wrap()
*               -test_dll_insert_first()
*               -test_dll_insert_middle()
*               -test_dll_insert_last()
*               -test_dll_add_invalid_position()
*               -test_dll_remove_first()
*               -test_dll_remove_last()
*               -test_dll_remove_middle()
*               -test_dll_remove_invalid_remove_position()
*               -test_dll_remove_invalid_list()
*               -test_dll_destroy_invalid_list()
*               -test_dll_destroy_valid()
*               -test_dll_size_valid()
*               -test_dll_size_invalid()
*               -test_dll_search_invalid_list()
*               -test_dll_search_valid_data()
*               -test_dll_search_invalid_data()
*
* Tester : Virag Gada, MS in Embedded Systems
* Testing environment : To test the functions we need to install the
* Cmocka framework libraries in our folder path and make changes to
* the test Makefile to include our source files and test files.
*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "circbuff.h"
#include "linkedList.h"

uint32_t length_buff = 10;
uint32_t data[11] = {1,2,3,4,5,6,7,8,9,10,11};

/*************************************************************************
* test_circbuf_allocate_free() - Function to test cbuffer_memoryAllocate
* function and cbuffer_Destroy to check if buffer function can be used
* to allocate and free memory from heap
* Input parameter - For this we send the circbuff pointer and length
* to memory allocate function and pass it again to the destory function
* Expected Result - Receive a NO_ERROR enum from the functions
*************************************************************************/
void test_circbuff_allocate_free(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    status = cbuffer_destroy(&cBuffStruct);
  }

  assert_int_equal(status,NO_ERROR);
}

/***************************************************************************
* test_circbuf_invalid_add_pointer() - Function to test if circbuff function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* cbuffer_full function
* Expected Result - Receive a NULL_POINTER enum from the function
***************************************************************************/
void test_circbuff_add_invalid_pointer(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status = cbuffer_add(&cBuffStruct,data);
  assert_int_equal(status,NULL_POINTER);
}

/***********************************************************************
* test_circbuff_add_full() - Function to test if circular buffer is
* full and return corresponding enum
* Input parameter - For this we initialize buffer and add data to it
* until buffer is full
* Expected Result - Receive a BUFFER_FULL enum from the function
***********************************************************************/
void test_circbuff_add_over_fill(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    for(uint32_t i = 0;i<length_buff+1;i++)
    {
      status = cbuffer_add(&cBuffStruct,(data+i));
      if(status == ERROR)
      {
        break;
      }
    }
  }
  assert_int_equal(status,BUFFER_FULL);
}

/************************************************************************
* test_circbuff_add_available() - Function to test if circular buffer has
* space available and return corresponding enum
* Input parameter - For this we initialize buffer and add data only some
* data to it
* Expected Result - Receive a AVAILABLE enum from the function
*************************************************************************/
void test_circbuff_add_available(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    for(uint32_t i = 0;i<length_buff-5;i++)
    {
      status = cbuffer_add(&cBuffStruct,(data+i));
      if(status == ERROR)
      {
        break;
      }
    }
  }
  assert_int_equal(status,AVAILABLE);
}

/***********************************************************************
* test_circbuff_buff_full() - Function to test if circular buffer is
* full and return corresponding enum
* Input parameter - For this we initialize buffer and add data to it
* until buffer is full. Then call the cbuffer_full function
* Expected Result - Receive a BUFFER_FULL enum from the function
***********************************************************************/
void test_circbuff_buff_full(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    for(uint32_t i = 0;i<length_buff;i++)
    {
      status = cbuffer_add(&cBuffStruct,(data+i));
      if(status == ERROR)
      {
        break;
      }
      status = cbuffer_full(&cBuffStruct);
    }
  }
  assert_int_equal(status,BUFFER_FULL);
}

/****************************************************************************
* test_circbuf_full_invalid_pointer() - Function to test if circbuff function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* cbuffer_full function
* Expected Result - Receive a NULL_POINTER enum from the function
****************************************************************************/
void test_circbuff_full_invalid_pointer(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status = cbuffer_full(&cBuffStruct);
  assert_int_equal(status,NULL_POINTER);
}


/************************************************************************
* test_circbuff_full_available() - Function to test if circular buffer has
* space available and return corresponding enum
* Input parameter - For this we initialize buffer and add data only some
* data to it. Then call cbuffer_full function
* Expected Result - Receive a AVAILABLE enum from the function
*************************************************************************/
void test_circbuff_full_available(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    for(uint32_t i = 0;i<length_buff-5;i++)
    {
      status = cbuffer_add(&cBuffStruct,(data+i));
    }
    status = cbuffer_full(&cBuffStruct);
  }
  assert_int_equal(status,AVAILABLE);
}

/*******************************************************************************
* test_circbuff_remove_invalid_pointer() - Function to test if circbuff function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* cbuffer_remove function
* Expected Result - Receive a NULL_POINTER enum from the function
*******************************************************************************/
void test_circbuff_remove_invalid_pointer(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status = cbuffer_remove(&cBuffStruct);
  assert_int_equal(status,NULL_POINTER);
}

/*******************************************************************************
* test_circbuff_remove_over_remove() - Function to test if circbuff function
* can handle removing more data than present.
* Input parameter - For this we add data only once and try to remove from it
* twice
* Expected Result - Receive a BUFFER_EMPTY enum from the function
*******************************************************************************/
void test_circbuff_remove_over_remove(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);

    status = cbuffer_add(&cBuffStruct,data);
    if(status == AVAILABLE)
    {
      status = cbuffer_remove(&cBuffStruct);
      if(status == NO_ERROR)
      {
        status = cbuffer_remove(&cBuffStruct);
      }
    }
  }
  assert_int_equal(status,BUFFER_EMPTY);
}

/****************************************************************************
* test_circbuff_remove_available() - Function to test if circular buffer has
* space available and return corresponding enum
* Input parameter - For this we initialize buffer and add data once to it
* and remove once.
* Expected Result - Receive a NO_ERROR enum from the function
****************************************************************************/
void test_circbuff_remove_available(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);

    status = cbuffer_add(&cBuffStruct,data);
    if(status == AVAILABLE)
    {
      status = cbuffer_remove(&cBuffStruct);
    }
  }
  assert_int_equal(status,NO_ERROR);
}

/******************************************************************************
* test_circbuff_empty_invalid_pointer() - Function to test if circbuff function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* cbuffer_empty function
* Expected Result - Receive a NULL_POINTER enum from the function
******************************************************************************/
void test_circbuff_empty_invalid_pointer(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status = cbuffer_empty(&cBuffStruct);
  assert_int_equal(status,NULL_POINTER);
}

/****************************************************************************
* test_circbuff_empty_available() - Function to test if circular buffer has
* space available and return corresponding enum
* Input parameter - For this we initialize buffer and add data once to it.
* Then call the cbuffer_empty function
* Expected Result - Receive a AVAILABLE enum from the function
****************************************************************************/
void test_circbuff_empty_available(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    for(uint32_t i = 0;i<length_buff-5;i++)
    {
      status = cbuffer_add(&cBuffStruct,(data+i));
    }
    status = cbuffer_empty(&cBuffStruct);
  }
  assert_int_equal(status,AVAILABLE);
}

/*************************************************************************
* test_circbuff_add_wrap() - Function to test if circular buffer can wrap
* near the boundary when data is added to it
* Input parameter - For this we initialize buffer and add data to it
* until buffer is full and check the head pointer with its previous value
* Expected Result - If the head pointer and previous value point
* at the same position then there is no error
*************************************************************************/
void test_circbuff_add_wrap(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  volatile uint32_t *last;
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    last = cBuffStruct->head;
    for(uint32_t i = 0;i<length_buff+1;i++)
    {
      status = cbuffer_add(&cBuffStruct,(data+i));
      if(status == ERROR)
      {
        break;
      }
    }
  }
  if(last == (cBuffStruct->head))
  {
    status = NO_ERROR;
  }
  assert_int_equal(status,NO_ERROR);
}

/***********************************************************************
* test_circbuff_remove_wrap() - Function to test if circular buffer can
* wrap near the boundary when data is removed from it
* Input parameter - For this we initialize buffer and add data to it
* until buffer is full and remove the same amount of data then check the
* tail pointer with its previous value
* Expected Result - If the tail pointer and previous value point
* at the same position then there is no error
***********************************************************************/
void test_circbuff_remove_wrap(void **state)
{
  circBuffStruct *cBuffStruct = NULL;
  cbuff_state status;

  status = cbuffer_memoryAllocate(&cBuffStruct,length_buff);
  volatile uint32_t *last;
  if(status == NO_ERROR)
  {
    cbuffer_init(&cBuffStruct,length_buff);
    last = cBuffStruct->tail;
    for(uint32_t i = 0;i<length_buff+1;i++)
    {
      status = cbuffer_add(&cBuffStruct,(data+i));
      if(status == ERROR)
      {
        break;
      }
    }
        for(uint32_t i = 0;i<length_buff+1;i++)
    {
      status = cbuffer_remove(&cBuffStruct);
      if(status == ERROR)
      {
        break;
      }
    }
  }
  if(last == (cBuffStruct->tail))
  {
    status = NO_ERROR;
  }
  assert_int_equal(status,NO_ERROR);
}

/****************************************************************************
* test_dll_insert_first() - Function to test if linked list function
* can handle adding data at the first position
* Input parameter - For this we call the add_node function with index 1
* Expected Result - Receive a SUCCESS enum from the function
****************************************************************************/
void test_dll_insert_first(void **state)
{
  nodeStruct *first_node = NULL;
  linkList_state status = add_node(&first_node,data[0],1);
  assert_int_equal(status,SUCCESS);
}

/****************************************************************************
* test_dll_insert_middle() - Function to test if linked list function
* can handle adding data in the middle of the list
* Input parameter - For this we add data at the first 2 positions and then
* add data between them.
* Expected Result - Receive a SUCCESS enum from the function
****************************************************************************/
void test_dll_insert_middle(void **state)
{
  nodeStruct *first_node = NULL;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[1],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
  }
  assert_int_equal(status,SUCCESS);
}

/****************************************************************************
* test_dll_insert_last() - Function to test if linked list function
* can handle adding data at the last position
* Input parameter - For this we add data at the beginning and then add data
* at the second(last) position
* Expected Result - Receive a SUCCESS enum from the function
****************************************************************************/
void test_dll_insert_last(void **state)
{
  nodeStruct *first_node = NULL;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],2);
  }
  assert_int_equal(status,SUCCESS);
}

/****************************************************************************
* test_dll_add_invalid_position() - Function to test if linked list function
* can handle adding data at wrong index
* Input parameter - For this we add data at first node and then try to add
* data at the 5th node
* Expected Result - Receive a FAILURE enum from the function
****************************************************************************/
void test_dll_add_invalid_position(void **state)
{
  nodeStruct *first_node = NULL;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],5);
  }
  assert_int_equal(status,FAILURE);
}

/****************************************************************************
* test_dll_remove_first() - Function to test if linked list function
* can handle removind data from the first position
* Input parameter - For this we add some values to the list and then remove
* data from the first position
* Expected Result - Receive a SUCCESS enum from the function
****************************************************************************/
void test_dll_remove_first(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t data_val;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
    if(status == SUCCESS)
    {
      status = remove_node(&first_node,1,&data_val);
    }
  }
  if( data_val != data[0])
  {
    status = FAILURE;
  }
  assert_int_equal(status,SUCCESS);
}

/****************************************************************************
* test_dll_remove_last() - Function to test if linked list function
* can handle removing data from the last position
* Input parameter - For this we add data some data to the list and then
* remove data from the last position
* Expected Result - Receive a SUCCESS enum from the function
****************************************************************************/
void test_dll_remove_last(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t data_val;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
    if(status == SUCCESS)
    {
      status = remove_node(&first_node,3,&data_val);
    }
  }
  if(data_val != data[2])
  {
    status = FAILURE;
  }
  assert_int_equal(status,SUCCESS);
}

/****************************************************************************
* test_dll_remove_middle() - Function to test if linked list function
* can handle removing data from the middle of the list
* Input parameter - For this we add some data to the list and then remove
* data from the middle
* Expected Result - Receive a SUCCESS enum from the function
****************************************************************************/
void test_dll_remove_middle(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t data_val;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
    if(status == SUCCESS)
    {
      status = remove_node(&first_node,2,&data_val);
    }
  }
  if(data_val != data[1])
  {
    status = FAILURE;
  }
  assert_int_equal(status,SUCCESS);
}

/****************************************************************************
* test_dll_remove_invalid_remove_position() - Function to test if linked list
* function can handle removing data from the wrong index
* Input parameter - For this we add some data to the list and then try to
* remove data from an invalid index
* Expected Result - Receive a FAILURE enum from the function
****************************************************************************/
void test_dll_remove_invalid_remove_position(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t data_val;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
    if(status == SUCCESS)
    {
      status = remove_node(&first_node,6,&data_val);
    }
  }
  assert_int_equal(status,FAILURE);
}

/****************************************************************************
* test_dll_remove_invalid_list() - Function to test if linked list function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* remove_node function
* Expected Result - Receive a NULL_PTR enum from the function
****************************************************************************/
void test_dll_remove_invalid_list(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t data_val;
  linkList_state status = remove_node(&first_node,0,&data_val);
  assert_int_equal(status,NULL_PTR);
}

/****************************************************************************
* test_dll_destroy_invalid_list() - Function to test if linked list function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* destroy_list function
* Expected Result - Receive a NULL_PTR enum from the function
****************************************************************************/
void test_dll_destroy_invalid_list(void **state)
{
  nodeStruct *first_node = NULL;
  linkList_state status = destroy_list(&first_node);
  assert_int_equal(status,NULL_PTR);
}

/****************************************************************************
* test_dll_destroy_valid() - Function to test if linked list function
* can destroy a valid list
* Input parameter - For this we add some data and call the destroy_list
* function
* Expected Result - Receive a SUCCESS enum from the function
****************************************************************************/
void test_dll_destroy_valid(void **state)
{
  nodeStruct *first_node = NULL;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[1],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
  }
  if(status == SUCCESS)
  {
    status = destroy_list(&first_node);
  }
  assert_int_equal(status,SUCCESS);
}

/****************************************************************************
* test_dll_size_invalid() - Function to test if linked list function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* list_size function
* Expected Result - Receive a NULL_PTR enum from the function
****************************************************************************/
void test_dll_size_invalid(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t value = list_size (&first_node);
  assert_int_equal(value,0);
}

/*******************************************************************************
* test_dll_size_valid() - Function to test if linked list function can find
* the size of a valid list
* Input parameter - For this we add some data and call the list_size
* function
* Expected Result - Successful if return value equal to the count of data added
*******************************************************************************/
void test_dll_size_valid(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t value;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[1],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
  }
  if(status == SUCCESS)
  {
    value = list_size(&first_node);
  }
  assert_int_equal(value,3);
}

/****************************************************************************
* test_dll_search_invalid_list() - Function to test if linked list function
* can handle invalid pointer(NULL)
* Input parameter - For this we send the a NULL pointer to the
* search_node function
* Expected Result - Receive a NULL_PTR enum from the function
****************************************************************************/
void test_dll_search_invalid_list(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t value;
  linkList_state status = search_node (&first_node,data[0],&value);
  assert_int_equal(status,NULL_PTR);
}

/*******************************************************************************
* test_dll_search_valid_data() - Function to test if linked list function can
* find the data from a valid list
* Input parameter - For this we add some data and call the search_node
* function
* Expected Result - Successful if index value equal to the position of data
* added
*******************************************************************************/
void test_dll_search_valid_data(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t index;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
    if(status == SUCCESS)
    {
      status = search_node(&first_node,data[1],&index);
    }
  }
  if((status == FAILURE) || index != 2)
  {
    status = FAILURE;
  }
  assert_int_equal(status,SUCCESS);
}

/*******************************************************************************
* test_dll_search_invalid_data() - Function to test if linked list function can
* handle an invalid data
* Input parameter - For this we add some data and call the search_node
* function to find data that is not present in the list
* Expected Result - Successful if function returns FAILURE
*******************************************************************************/
void test_dll_search_invalid_data(void **state)
{
  nodeStruct *first_node = NULL;
  uint32_t index;
  linkList_state status = add_node(&first_node,data[0],1);
  if(status == SUCCESS)
  {
    status = add_node(&first_node,data[2],2);
    if(status == SUCCESS)
    {
      status = add_node(&first_node,data[1],2);
    }
    if(status == SUCCESS)
    {
      status = search_node(&first_node,data[6],&index);
    }
  }
    assert_int_equal(status,FAILURE);
}

/*Function testing the unit tests by passing
* function pointers to cmoka_unit_test*/
int main(int argc, char **argv)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_circbuff_allocate_free),
    cmocka_unit_test(test_circbuff_add_invalid_pointer),
    cmocka_unit_test(test_circbuff_add_over_fill),
    cmocka_unit_test(test_circbuff_add_available),
    cmocka_unit_test(test_circbuff_buff_full),
    cmocka_unit_test(test_circbuff_full_invalid_pointer),
    cmocka_unit_test(test_circbuff_full_available),
    cmocka_unit_test(test_circbuff_remove_invalid_pointer),
    cmocka_unit_test(test_circbuff_remove_over_remove),
    cmocka_unit_test(test_circbuff_remove_available),
    cmocka_unit_test(test_circbuff_empty_invalid_pointer),
    cmocka_unit_test(test_circbuff_empty_available),
    cmocka_unit_test(test_circbuff_add_wrap),
    cmocka_unit_test(test_circbuff_remove_wrap),
    cmocka_unit_test(test_dll_insert_first),
    cmocka_unit_test(test_dll_insert_middle),
    cmocka_unit_test(test_dll_insert_last),
    cmocka_unit_test(test_dll_add_invalid_position),
    cmocka_unit_test(test_dll_remove_first),
    cmocka_unit_test(test_dll_remove_last),
    cmocka_unit_test(test_dll_remove_middle),
    cmocka_unit_test(test_dll_remove_invalid_remove_position),
    cmocka_unit_test(test_dll_remove_invalid_list),
    cmocka_unit_test(test_dll_destroy_invalid_list),
    cmocka_unit_test(test_dll_destroy_valid),
    cmocka_unit_test(test_dll_size_valid),
    cmocka_unit_test(test_dll_size_invalid),
    cmocka_unit_test(test_dll_search_invalid_list),
    cmocka_unit_test(test_dll_search_valid_data),
    cmocka_unit_test(test_dll_search_invalid_data),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
