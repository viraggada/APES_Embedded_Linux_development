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

