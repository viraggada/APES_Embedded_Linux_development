/*************************************************************************************************
* Authors : Virag Gada
* Date : 09/05/2017
*
* File : circbuff.c
* Description : Source file for Circular buffer functions
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
*************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "circbuff.h"

/*This function allocates heap memory to circular buffer structure and data buffer*/
cbuff_state cbuffer_memoryAllocate(circBuffStruct **buffStruct, uint32_t length)
{
	  cbuff_state state;
		/*First allocate memory to the circular buffer structure*/
	  *buffStruct = (circBuffStruct *) calloc(1,sizeof(circBuffStruct));
    if(*buffStruct != NULL)
		{
			/*Then allocate memory to the actual buffer*/
	    (*buffStruct)->base = (uint32_t *)calloc(length,sizeof(uint32_t));
      if((*buffStruct)->base != NULL)
			{
				state = NO_ERROR;
			}
	    else
			  state = ERROR;
		}
    else
		  state = ERROR;
		return state;
}

/*This function is used to free the heap memory allocate to buffer*/
cbuff_state cbuffer_destroy(circBuffStruct **buffStruct)
{
	if(*buffStruct == NULL)
	{
		return ERROR;
	}
	/*Free the circular buffer first then the structure*/
	free((void *)(*buffStruct)->base);
	(*buffStruct)->base = NULL;
	free((void *)*buffStruct);
	*buffStruct = NULL;
	/*After destroying memory return no error*/
  return NO_ERROR;
}

/*This function checks if buffer is full*/
cbuff_state cbuffer_full(circBuffStruct **buffStruct)
{
	cbuff_state state;
	/*Return Null pointer error, if buffStruct doesn't point to a valid location*/
	if(*buffStruct == NULL)
	{
    return NULL_POINTER;
	}
	/*If count value is equal to length, return circular buffer is full*/
    if((*buffStruct)->count == (*buffStruct)->length)
		{
        state=BUFFER_FULL;
    }
    else
		{
		/*If count is less than length, return circular buffer is available*/
       state=AVAILABLE;
    }
	/*Return circular buffer is full or available*/
    return state;
}

/*This function checks if buffer is empty*/
cbuff_state cbuffer_empty(circBuffStruct **buffStruct)
{
	cbuff_state state;
	/*return null pointer error,if *buffStruct doesn't point to a valid loaction*/
	if(*buffStruct == NULL)
	{
		return NULL_POINTER;
  }
	/*If Count=0, return circular buffer is empty*/
	if(((*buffStruct)->head==(*buffStruct)->tail) && (*buffStruct)->count==0)
	{
    state=BUFFER_EMPTY;
  }
  else
	{
		/*If count!=0 return buffer is available*/
    state=AVAILABLE;
  }
	/*Return whether buffer is empty or available*/
  return state;
}

/*This function is used to add elements to circular buffer*/
cbuff_state cbuffer_add(circBuffStruct **buffStruct, uint32_t * data)
{
	cbuff_state state;
	/*Return null pointer error if *buffStruct doesn't point to a valid loaction*/
	if(*buffStruct == NULL)
	{
		return NULL_POINTER;
	}

	if(((*buffStruct)->head==(*buffStruct)->tail) && ((*buffStruct)->count>(*buffStruct)->length))
	{
		(*buffStruct)->count=0;
	}

  /*Condition to check if buffer is not full*/
  if((*buffStruct)->count!=(*buffStruct)->length)
	{
	  /*Put the data at location pointed by head, then increment head pointer and count value*/
    *(*buffStruct)->head=*data;
    (*buffStruct)->head++;
    (*buffStruct)->count++;

		/*Wrap around, if head is pointing to last location of buffer*/
		if((*buffStruct)->head>((*buffStruct)->base+(*buffStruct)->length-1))
		{
		  (*buffStruct)->head=(*buffStruct)->base;
		}
		/*Return available after adding element in buffer*/
    state = AVAILABLE;
  }
  else
	{
  /*Return buffer full if values to add is not zero but length is equal to count*/
    state = BUFFER_FULL;
  }
	/*Return available or buffer full*/
	return state;
}

/*This function is used to check the value present at a particular
  location of circular buffer*/
cbuff_state cbuffer_dump(circBuffStruct **buffStruct)
{
	/*Return null pointer error, if *buffStruct doesn't point to a valid location*/
	if(*buffStruct == NULL)
	  return NULL_POINTER;
  uint32_t i;
	for(i=0;i<(*buffStruct)->count;i++)
	{
    printf("Value at %d is %d\n",i+1,*((*buffStruct)->tail + i));
	}
	return NO_ERROR;
}

/*This function is used to remove elements from circular buffer*/
cbuff_state cbuffer_remove(circBuffStruct **buffStruct)
{
	uint32_t state;
	/*Return Null pointer error if *buffStruct doesn't point to a valid location*/
	if(*buffStruct == NULL)
	{
		return NULL_POINTER;
	}

  /*Count value checks if buffer is empty*/
  if((*buffStruct)->count!=0)
	{
		*(*buffStruct)->tail=0;

		/*After removing the element from buffer, increment tail pointer and decrement count value*/
		(*buffStruct)->tail++;
		(*buffStruct)->count--;

		/*Wrap around if tail points at last location*/
		if((*buffStruct)->tail>((*buffStruct)->base+(*buffStruct)->length-1))
		{
		  (*buffStruct)->tail=(*buffStruct)->base;
		}
		/*Return no error after removing element*/
		state = NO_ERROR;
  }
  else
	{
	  /* If values to remove is not 0 but buffer is empty, return buffer empty*/
    state = BUFFER_EMPTY;
  }
  return state;
}

uint32_t cbuffer_size(circBuffStruct **buffStruct)
{
	/*Return Null pointer error if *buffStruct doesn't point to a valid location*/
	if(*buffStruct == NULL)
	{
		return 0;
	}
	return (*buffStruct)->count;
}

/*This function is used to initialize circular buffer, to make head,tail
and buffer point at first location of buffer and initialize count to 0*/
void cbuffer_init(circBuffStruct **buffStruct, uint32_t length)
{
	(*buffStruct)->head=(*buffStruct)->base;
	(*buffStruct)->tail=(*buffStruct)->base;
	(*buffStruct)->length = length;
	(*buffStruct)->count = 0;
}
