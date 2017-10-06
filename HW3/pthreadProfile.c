#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void *printFunction(void *t)
{
  printf("%s\n","This is in child thread");
  pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  pthread_t threadID;
  clock_t startClock,endThread;

  /* Take time before pthread_create */
  startClock = clock();
  pthread_create(&threadID, NULL, printFunction, NULL);
  /* Take time after pthread_create */
  endThread = clock();

  pthread_join(threadID, NULL);

  /* The difference gives us the time for pthread_create function */
  printf("Time for child thread - %ld clock cycles\n", endThread - startClock);
  pthread_exit (NULL);
}
