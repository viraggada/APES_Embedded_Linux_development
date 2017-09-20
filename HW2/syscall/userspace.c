#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SYSCALLNUM (333)

int main()
{
  size_t len = 256;
  int32_t arry[len];
  int32_t result[len];
  size_t i = 0;
  int32_t status;  

  /*Generate random values*/
  srand(time(NULL));
  
  printf("Input random values are: ");
  for(i = 0;i<len;i++)
  {
    *(arry+i)=(int32_t)((rand()%512)+1);
    printf(" %d ",*(arry+i));
  }

  status = syscall(SYSCALLNUM,arry,len,result);
  
  switch(status)
  {
    case 14: printf("\nBad address sent\n");
             break;
    
    case 22: printf("\nInvalid length value\n");
             break;

    case 12: printf("\nError copying\n");
             break;

    default:
             printf("\n\nSorted Values are : ");
             for(i = 0;i<len;i++)
             {
               printf(" %d ",*(result+i));
             }
             break;
  }

  printf("\n");  
  return 0;
}
