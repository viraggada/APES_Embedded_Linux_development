/**************************************************************************************************
* Authors : Virag Gada
* Date : 09/05/2017
*
* File : circbuff.h
* Description : Header file for Circular buffer functions
*               -cbuff_state cbuffer_full(circBuffStruct **buffStruct)
*               -cbuff_state cbuffer_empty(circBuffStruct **buffStruct)
*               -cbuff_state cbuffer_add(circBuffStruct **buffStruct, uint32_t * data)
*               -void cbuffer_dump(circBuffStruct **buffStruct)
*               -cbuff_state cbuffer_remove(circBuffStruct **buffStruct)
*               -cbuff_state cbuffer_memoryAllocate(circBuffStruct **buffStruct, uint32_t length)
*               -cbuff_state cbuffer_destroy(circBuffStruct **buffStruct)
*               -uint32_t cbuffer_size(circBuffStruct **buffStruct)
*               -void cbuffer_init(circBuffStruct **buffStruct, uint32_t lenght)
* Tool : Compiler - GCC, Linker - GDB
* Code modified from - https://github.com/viraggada/ese-repo/blob/master/project4/SOURCES/circbuff.h
***************************************************************************************************/

#include <stdint.h>

#ifndef SOURCES_CIRCBUFF_H_
#define SOURCES_CIRCBUFF_H_

/*Since value is to be modified by programmer only not user*/
#define BUFFER_SIZE (3)

/*Enum used for returning the buffer state*/
typedef enum buffer_states{
  NO_ERROR,
	ERROR,
	BUFFER_FULL,
  BUFFER_EMPTY,
	NULL_POINTER,
	NO_LENGTH,
	BUFFER_ALLCATION_FAILURE,
  AVAILABLE
}cbuff_state;

/*Circular buffer structure*/
typedef struct CircBuf_t{
  volatile uint32_t * base;
  volatile uint32_t * head;
  volatile uint32_t * tail;
  volatile uint32_t length;
  volatile uint32_t count;
}circBuffStruct;

/******************************************************************************
* This function is used to check if circular buffer is full
* The return value can be full or available depending on count value
* circBuffStruct **buffStruct - Pointer which points to circular buffer
* cbuff_state(return) - Status Enumeration
*******************************************************************************/
__attribute__((always_inline)) static inline cbuff_state inline_cbuffer_full(circBuffStruct **buffStruct)
{
  cbuff_state state;
	((*buffStruct)->count == (*buffStruct)->length)?(state=BUFFER_FULL):(state=AVAILABLE);
	return state;
}

/*******************************************************************************
* This function is used to check if circular buffer is empty
* The return value can be empty or available depending on count value
* circBuffStruct **buffStruct - Pointer which points to circular buffer
* cbuff_state(return) - Status Enumeration
*******************************************************************************/
__attribute__((always_inline)) static inline cbuff_state inline_cbuffer_empty(circBuffStruct **buffStruct)
{
  cbuff_state state;
	((*buffStruct)->count == 0)?(state=BUFFER_EMPTY):(state=AVAILABLE);
	return state;
}

/*******************************************************************************
* This function is used to allocate heap memory to circular buffer
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* uint_32t length - This variable gives the length of circular buffer
* cbuff_state(return) - Status Enumeration
*******************************************************************************/
cbuff_state cbuffer_memoryAllocate(circBuffStruct **buffStruct, uint32_t length);

/******************************************************************************
* This function is used to free the heap memory allocated to circular buffer
* structure and buffer
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* cbuff_state(return) - Status Enumeration
******************************************************************************/
cbuff_state cbuffer_destroy(circBuffStruct **buffStruct);

/******************************************************************************
* This function is used to check if circular buffer is full
* The return value can be full or available depending on count value
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* cbuff_state(return) - Status Enumeration
*******************************************************************************/
cbuff_state cbuffer_full(circBuffStruct **buffStruct);


/*******************************************************************************
* This function is used to check if circular buffer is empty
* The return value can be empty or available depending on count value
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* cbuff_state(return) - Status Enumeration
*******************************************************************************/
cbuff_state cbuffer_empty(circBuffStruct **buffStruct);


/******************************************************************************
* This function is used to add elements to circular buffer
* The return value can be full or available
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* uint8_t * data - Pointer which points to array which contains data
                   to be stored on buffer
* cbuff_state(return) - Status Enumeration
********************************************************************************/
cbuff_state cbuffer_add(circBuffStruct **buffStruct, uint32_t * data);


/*******************************************************************************
* This function is to check the value at a particular location of buffer
* It returns the value present at that loaction
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* cbuff_state(return) - Status Enumeration
*******************************************************************************/
cbuff_state cbuffer_dump(circBuffStruct **buffStruct);


/*******************************************************************************
* This function is used to remove elements from circular buffer
* The return value can be empty or available depending on count value
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* cbuff_state(return) - Status Enumeration
*******************************************************************************/
cbuff_state cbuffer_remove(circBuffStruct **buffStruct);


/*****************************************************************************
* This function returns the size of the circular buffer
* circBuffStruct **buffStruct - Pointer to the circular buffer structure
* uint32_t(return) - The size of the circular buffer, zero if invalid pointer
*****************************************************************************/
uint32_t cbuffer_size(circBuffStruct **buffStruct);


/*****************************************************************************
* This function is used to initialize circular buffer
* It makes head, tail, buffer point to same location and make count 0
*****************************************************************************/
void cbuffer_init(circBuffStruct **buffStruct, uint32_t length);

#endif /* SOURCES_CIRCBUFF_H_ */
