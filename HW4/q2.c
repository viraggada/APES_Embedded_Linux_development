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

#define BUFFER_SIZE (50)
#define SERVER_IP   ("192.168.7.2")
#define SERVER_PORT (6001)
#define DELIMITER   (" ")
/* Prompt for user on using the commands */
void promptUser(void){
  printf("\nCommands available to use:\n");
  printf("    led [on/off]\n");
  printf("    period [value in ms]\n");
  printf("    duty [value in percent]\n");
  printf("    read [all/state/period/duty]\n");
}

int main(){
  int socket_fd;
  struct sockaddr_in remoteAddr;
  char command[BUFFER_SIZE] = {(int)'\0'};
  char commandCopy[BUFFER_SIZE] = {(int)'\0'};
  char receivedData[BUFFER_SIZE] = {(int)'\0'};
  int val;
  char *token[2];

  /* Create client socket */
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Client failed to create socket\n");
    exit(1);
  }

  memset(&remoteAddr,(int)'\0',sizeof(struct sockaddr_in));
  remoteAddr.sin_family = AF_INET;
  remoteAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  remoteAddr.sin_port = htons(SERVER_PORT);

  /* Connect to server */
  if(connect(socket_fd,(struct sockaddr*)&remoteAddr,sizeof(struct sockaddr))<0){
    perror("Error Connection:");
    exit(1);
  }else{
    printf("Connected to server\n");
  }


  while(1){

    /* Prompt user for each command */
    promptUser();

    fgets(command,BUFFER_SIZE,stdin);

    /* Add NULL terminator to user input*/
    if ((strlen(command)>0) && (command[strlen (command) - 1] == '\n'))
      command[strlen (command) - 1] = '\0';

    strcpy(commandCopy,command);

    token[0] = strtok(commandCopy,DELIMITER);

    /* Send command to driver */
    val = write(socket_fd,command,strlen(command));
    if(val<0){
      perror("Error sending: ");
    }
    //printf("Command - %s\n",token[0]);

    /* Do a receive for read command */
    if(strncmp(token[0],"read",4)==0){
      if((val = recv(socket_fd,receivedData,BUFFER_SIZE,0))<0){
        printf("%s\n","Error Receiving");
      }
      if(strcmp("error",receivedData) == 0){
        printf("Error Reading File\n");
      }
      else{
        printf("Requested data - %s\n", receivedData);
      }
    }
    memset(command,(int)'\0',BUFFER_SIZE);
    memset(receivedData,(int)'\0',BUFFER_SIZE);
    memset(commandCopy,(int)'\0',BUFFER_SIZE);
  }
}
