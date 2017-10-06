#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutexLock;
pthread_cond_t condVal;

char valueArray[100];

void *readerFunction(void *t)
{
  /*pthread_self - Thre function returns the thread ID of the calling thread*/
  pthread_t tid = pthread_self();
  printf("Reader Thread ID - %ld\n",tid);

  /*pthread_mutex_lock - The function is used to acquire a lock on a mutex
    variable. If the mutex is already locked then this function blocks till
    the mutex variable is unlocked by the other blocking thread.
    Takes pointer to mutex thread object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_mutex_lock(&mutexLock);

  /*pthread_cond_wait - The function is used to block the thread until a
    specified condition is signalled. It is called after locking a mutex.
    It automatically releases the mutex after being called. Once it is
    signalled back the mutex is locked again automatically
    Takes pointer to condition variable and mutex thread object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_cond_wait(&condVal, &mutexLock);

  printf("String added by user - %s\n", valueArray);

  pthread_mutex_unlock(&mutexLock);
  pthread_exit(NULL);
}

void *writerFunction(void *t)
{
  pthread_t tid = pthread_self();
  printf("Writer Thread ID - %ld\n",tid);

  int stat=1;

  while(stat!=0){
  /*pthread_mutex_trylock - The function attempts to acquire lock on a mutex
    variable. If the mutex is already locked then this function will return
    with a busy error code. It helps preventing deadlock as it is non blocking
    Takes mutex thread object as input parameter
    Returns 0 for success or an error number to indicate error*/
    stat=pthread_mutex_trylock(&mutexLock);
    printf("Writer\n");
    sleep(1);
  }
  printf("Enter a string - \n");
  fgets(valueArray,50,stdin);

  /*pthread_cond_signal - The function is used to signal another thread waiting
    on a condition variable. It should be called inside a mutex, and must unlock
    mutex after calling for the pthread_cond_wait routine to complete
    Takes the pointer to condition variable as input
    Takes the pointer to attribute object and stackSize variable as input
    Returns 0 for success or an error number to indicate error*/
  pthread_cond_signal(&condVal);

  /*pthread_mutex_unlock - The function is used to unlock the mutex if called
    by the same owner of the lock. It is called when the owner thread is done
    processing on the shared data. It returns error if the mutex is already
    locked or if it is locked by another thread
    Takes mutex thread object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_mutex_unlock(&mutexLock);

  pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  pthread_attr_t attr;
  pthread_t writeThread,readThread;
  size_t stackSize;

  /*pthread_mutex_init - The function is used to dynamically initialize the
    mutex with mutex object attribures. Passing a NULL results in default
    attributes to be used. The optional attributes that can be set are -
    Protocol, Prioceiling and Process-sharing
    Takes pointer to mutex thread object and mutex attribure object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_mutex_init(&mutexLock, NULL);

  /*pthread_cond_init - The function is used to dynamically initialize the
    condition variable with condition object attribures. Passing a NULL
    results in default attributes to be used. The only attribute that can
    be set is process-shared
    Takes pointer to condition variable and mutex thread object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_cond_init (&condVal, NULL);

  /*pthread_attr_init - This function initializes all the thread attributes
    with default value used by an implementataion.
    This newly created attribute object can then be used by thread create
    It takes the pointer to the thread attribute object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_attr_init(&attr);

  /*pthread_attr_getstacksize - This call is made to get the default stack
    size associated with thread
    Takes the pointer to attribute object and stackSize variable as input
    Returns 0 for success or an error number to indicate error*/
  pthread_attr_getstacksize(&attr,&stackSize);
  printf("Default Stack Size - %ld\n",stackSize);
  stackSize = 9000000;

  /*pthread_attr_setstacksize - This call is made to set the stack size
   attribute of the thread
   Takes the pointer to attribute object and stackSize variable as input
   Returns 0 for success or an error number to indicate error*/
  pthread_attr_setstacksize(&attr,stackSize);

  /*pthread_attr_getstacksize - This call is made to get the stack size
    associated with thread after modification
    Takes the pointer to attribute object and stackSize variable as input
    Returns 0 for success or an error number to indicate error*/
  pthread_attr_getstacksize(&attr,&stackSize);
  printf("Set Stack Size - %ld\n",stackSize);

  /*pthread_create - This function is used create a new thread and it's
    execution is defined in the start routine parameter passed along
    with the function call
    Takes pointer to thread object, attribute object, start routine and
    input argument as inout parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_create(&writeThread, &attr, writerFunction, NULL);
  pthread_create(&readThread, &attr, readerFunction, NULL);

  /*pthread_join - This is a blocking function which is used to wait for
    the specified thread to terminate, for this to work the thread needs
    to be created with a PTHREAD_CREATE_JOINABLE detach state attribute
    Takes the thread object and double pointer to a value which is returned
    by the terminating thread as input
    Returns 0 for success or an error number to indicate error*/
  pthread_join(readThread, NULL);
  pthread_join(writeThread, NULL);

  /* Clean up and exit */
  /*pthread_attr_init - This function is used to free the attribute object.
    It takes the pointer to the thread attribute object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_attr_destroy(&attr);

  /*pthread_mutex_destroy - This function is used to free a mutex that is
    no longer needed
    Takes pointer to mutex thread object as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_mutex_destroy(&mutexLock);

  /*pthread_cond_destroy - The function is used to free a condition variable
    that is no longer needed
    Takes pointer to condition variable as input parameter
    Returns 0 for success or an error number to indicate error*/
  pthread_cond_destroy(&condVal);

  /*pthread_exit - This function is used to terminate the calling thread.
    It does not perform any cleanup so any open file will remain open
    even after thread terminates
    Takes pointer to a value variable as input parameter*/
  pthread_exit (NULL);
}
