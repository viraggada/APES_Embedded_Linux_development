/************************************************************************
* Authors: Virag Gada
* Date: 09/14/2017
* File: test_circbuff.c
* Description : Source file for testing circular buffer functions
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
*
* Tester : Virag Gada, MS in Embedded Systems
* Testing environment : To test the functions we need to install the
* Cmocka framework libraries in our folder path and make changes to
* the test Makefile to include our source files and test files.
*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "circbuff.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

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
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
