#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#define COMMAND_SIZE (50)
#define SERVER_PORT  (6001)
#define BACKLOG      (5)
#define DELIMITER    (" ")
#define LED_PATH     ("/dev/ledDriver")

int main()
{
  struct sockaddr_in *selfAddr;
  struct sockaddr_in fromAddr;
  socklen_t fromAddrSize;
  uint32_t fd;
  int sockfd;
  int clientfd;
  char clientCommand[COMMAND_SIZE] = {(int)'\0'};
  char copyCommand[COMMAND_SIZE] = {(int)'\0'};
  char readData[COMMAND_SIZE] = {(int)'\0'};
  char *token[2];
  uint8_t val;
  selfAddr = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));

  (*selfAddr).sin_family = AF_INET;           //address family
  (*selfAddr).sin_port = htons(SERVER_PORT); //sets port to network byte order
  (*selfAddr).sin_addr.s_addr = htonl(INADDR_ANY); //sets local address

  /*Create Socket*/
  if((sockfd = socket((*selfAddr).sin_family,SOCK_STREAM,0))< 0) {
    printf("Unable to create socket\n");
    exit(1);
  }
  printf("Socket Created\n");

  /*Call Bind*/
  if(bind(sockfd,(struct sockaddr *)selfAddr,sizeof(*selfAddr))<0) {
    printf("Unable to bind\n");
    exit(1);
  }
  printf("Socket Binded\n");

  /* Listen for incomming connections */
  if(listen(sockfd,BACKLOG)!=0){
    printf("%s\n","Listen Error");
    exit(1);
  }

  memset(&fromAddr,0,sizeof(fromAddr));

  fromAddrSize = sizeof(fromAddr);

  /*listen*/
  while(1) {
    printf("waiting for connections..\n");
    /*Accept*/
    if((clientfd = accept(sockfd,(struct sockaddr *)&fromAddr,&fromAddrSize)) < 0)
    {
      printf("Failed to accept connection\n");
      break;
    }else{

      do{
        printf("%s\n","Waiting for new command");
        if((val = recv(clientfd,clientCommand,COMMAND_SIZE,0))<=0){
          /* Exit on error */
          printf("Exiting client\n");
          break;
        }else if(strlen(clientCommand) == 0){
          /* Handling NULL data from client */
          printf("%s\n","NULL data");
        }else {
          printf("%s - %s\n","Command received", clientCommand);
          strcpy(copyCommand,clientCommand);
          token[0] = strtok(copyCommand,DELIMITER);
          //printf("Command - %s\n",token[0]);

          /* Open the device in read/write mode */
          fd = open(LED_PATH,O_RDWR,0);
          if(fd < 0){
            val = write(clientfd,"error",strlen("error"));
            if(val<0){
              printf("%s\n","Error sending data to client");
            }
          }

          val = write(fd, clientCommand, strlen(clientCommand));

          if(strncmp(token[0],"read",4)==0){
            //printf("%s\n","Reading data");
            val = read(fd,readData,COMMAND_SIZE);

            if(val < 0){
              printf("%s\n","Error Reading data");
              val = write(clientfd,"error",strlen("error"));
              if(val<0){
                printf("%s\n","Error sending data to client");
              }
            }else{ /* Send requested data */
              val = write(clientfd,readData,strlen(readData));
              if(val<0){
                printf("%s\n","Error sending data to client");
              }
              printf("Read data - %s\n",readData);
            }
          }
          close(fd);
        }
        memset(readData,(int)'\0',COMMAND_SIZE);
        memset(clientCommand,(int)'\0',COMMAND_SIZE);
        memset(copyCommand,(int)'\0',COMMAND_SIZE);
      }while(1);
    }
  }
  /*Close*/
  close(sockfd);
  return 0;
}
