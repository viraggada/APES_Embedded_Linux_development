/**************************************
* Authors : Virag Gada
* Date : 10/3/2017
*
* File : forkProfile.c
***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "stdint.h"

int main(int argc, char *argv[])
{
  pid_t processID, childID;
  clock_t startClock,endParent,endChild;
  uint32_t parentForkTime, childForkTime;
  uint32_t status;

  /* Getting the time before fork */
  startClock = clock();
  if((processID = fork())==0)
  {
    printf("%s\n","This is child process");
    char *args[] = {"./child",NULL};
    if(execvp(args[0],args)<0){
      printf("%s\n","Error");
    }
  }else if(processID > 0)
  {
    printf("%s\n","This is parent process");
    /* Getting the time after fork in parent*/
    endParent = clock();
    parentForkTime = endParent - startClock;
    printf("Time to fork in parent %d clock cycles\n", parentForkTime);
    childID =waitpid(-1,&status,0);
    /* Getting the time after fork and return child process exec*/
    endChild = clock();
    printf("Status = %d - Child PID - %d \n",status,childID);
    childForkTime = endChild - startClock;
    printf("Time to fork in child %d clock cycles\n", childForkTime);
  }else
  {
    printf("%s\n","Failed to fork");
    exit(1);
  }
  return 0;
}
