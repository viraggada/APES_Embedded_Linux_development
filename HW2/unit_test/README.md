ECEN 5013-001: Advanced Practical Embedded Software Development: Homework 2

Author: Virag Gada

Date: 09/18/2017

Goal:
To perform unit tests on data structures such as Circular Buffer and Double Linked List.

File structure:
* 3rd-party: Consisting of Makefile and folders required to install and run unit tests in cmocka framework.

* Headers: Consisting of header files required for our project.
           1. circbuff.h - Header file for circular buffer functions
           2. linkedList.h - Header file for double linked list functions

* SOURCES: Consisting of source files required for our project.
           1. circbuff.c - Source file for circular buffer functions
           2. linkedList.c - Source file for double linked list functions
           3. test_circbuff.c - Source file for unit testing circular buffer functions using cmocka
           4. test_dll.c - Source file for unit testing double linked list functions using cmocka
           5. test_hw2.c - Source file for unit testing both circular buffer and double linked list funcitons using cmocks

* Makefile: Makefile required to run unit test on our source files.

Function info:

Circular Buffer test functions:

* test_circbuff_allocate_free() - Function to test cbuffer_memoryAllocate
     function and cbuffer_Destroy to check if buffer function can be used
     to allocate and free memory from heap
* test_circbuff_add_invalid_pointer() - Function to test if circbuff function
     can handle invalid pointer(NULL)
* test_circbuff_add_over_fill() - Function to test if circular buffer is full
    and return corresponding enum
* test_circbuff_add_available() - Function to test if circular buffer has
    space available and return corresponding enum
* test_circbuff_buff_full() - Function to test if circular buffer is full
    and return corresponding enum
* test_circbuff_full_invalid_pointer() - Function to test if circbuff function
    can handle invalid pointer(NULL)
* test_circbuff_full_available() - Function to test if circular buffer has
    space available and return corresponding enum
* test_circbuff_remove_invalid_pointer() - Function to test if circbuff function
    can handle invalid pointer(NULL)
* test_circbuff_remove_over_remove() - Function to test if circbuff function
    can handle removing more data than present
* test_circbuff_remove_available() - Function to test if circular buffer has
    space available and return corresponding enum
* test_circbuff_empty_invalid_pointer() - Function to test if circbuff function
    can handle invalid pointer(NULL)
* test_circbuff_empty_available() - Function to test if circular buffer has
    space available and return corresponding enum
* test_circbuff_add_wrap() - Function to test if circular buffer can wrap
    near the boundary when data is added to it
* test_circbuff_remove_wrap() - Function to test if circular buffer can
    wrap near the boundary when data is removed from it

Double Linked List test functions:

* test_dll_insert_first() - Function to test if linked list function can
    handle adding data at the first position
* test_dll_insert_middle() - Function to test if linked list function can
    handle adding data in the middle of the list
* test_dll_insert_last() - Function to test if linked list function
    can handle adding data at the last position
* test_dll_add_invalid_position() - Function to test if linked list function
    can handle adding data at wrong index
* test_dll_remove_first() - Function to test if linked list function
    can handle removind data from the first position
* test_dll_remove_last() - Function to test if linked list function can handle
    removing data from the last position
* test_dll_remove_middle() - Function to test if linked list function can
    handle removing data from the middle of the list
* test_dll_remove_invalid_remove_position() - Function to test if linked list
    function can handle removing data from the wrong index
* test_dll_remove_invalid_list() - Function to test if linked list function
    can handle invalid pointer(NULL)
* test_dll_destroy_invalid_list() - Function to test if linked list function
    can handle invalid pointer(NULL)
* test_dll_destroy_valid() - Function to test if linked list function can
    destroy a valid list
* test_dll_size_valid() -Function to test if linked list function can find
    the size of a valid list
* test_dll_size_invalid() - Function to test if linked list function
    can handle invalid pointer(NULL)
* test_dll_search_invalid_list() - Function to test if linked list function
    can handle invalid pointer(NULL)
* test_dll_search_valid_data() - Function to test if linked list function can
    find the data from a valid list
* test_dll_search_invalid_data() - Function to test if linked list function can
    handle an invalid data

Execution:
To run test for Circular Buffer - $make test TEST=CB
To run test for Double Linked List - $make test TEST=DLL
To run test for both structure functions - $make test TEST=ALL
Default "$make test" runs test for both the structures
