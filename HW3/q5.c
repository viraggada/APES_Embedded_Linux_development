/*************************************************************************************************
* Authors : Virag Gada
* Date : 09/30/2017
*
* File : q5.c
*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>

#define TOTAL_WORDS (100)
#define DATA_TO_READ_PER_LINE (1000)

/* Global variables ad array used in the program */
pthread_t processThread,reportThread;
pthread_mutex_t mapMutexLock,dataMutexLock,threadMutexLock;
pthread_cond_t condVal;
pthread_attr_t attr;
FILE *fptr;
uint8_t quit_request = 0;
uint8_t active_count = 0;
uint32_t uniqueWordCount=0;
int32_t pthreadStatus = 0;

/* Global struct storing our file data */
struct stats{
  volatile uint32_t linecount;
  volatile uint32_t wordcount;
  volatile uint32_t charcount;
}fileStats;

/* Global arrays storing our mapped words and their count */
char words[TOTAL_WORDS][20]={0};
uint32_t word_count[TOTAL_WORDS]={0};
long int last_position = 0, new_position = 0;

/* Function Prototypes */
void map_word(char *str);
void closingHandler(int mySignal);
void processHandler(void);
void *processFunction(void *t);
void reportHandler(void);
void *reportFunction(void *t);

/* Function used to map data onto an array and store its occurence count */
void map_word(char *str)
{
  uint32_t i;
  for(i = 0;i<TOTAL_WORDS;i++)
  { /* If the array location is empty the work is not present so add it to
        our array */
    if(word_count[i]==0)
    {
      strcpy(words[i],str);
      word_count[i]++;
      break;
    }/* Increment words\ count based on insensitive case comparison */
    else if(strcasecmp(words[i],str) == 0)
    {
      word_count[i]++;
      break;
    }
  }
  //printf("Word %d - %s, count - %d\n", i+1, words[i],word_count[i]);
}

void closingHandler(int mySignal)
{
  printf("\nReceived closing signal - %d\n",mySignal);
  if((pthreadStatus = pthread_mutex_lock(&threadMutexLock))!=0){
    printf("%s %d\n","Thread Mutex lock error - ",pthreadStatus);
  }

  /* Set the global variable for all the other threads to act based on it */
  quit_request = 1;

  /* If any of the handlers are running then wait till their processing is done */
  while(active_count>0)
  {
    if((pthreadStatus = pthread_cond_wait(&condVal, &threadMutexLock))!=0){
      printf("%s %d\n","Thread condition wait error - ",pthreadStatus);
    }
  }

  if((pthreadStatus = pthread_mutex_unlock(&threadMutexLock))!=0){
    printf("%s %d\n","Thread Mutex unlock error",pthreadStatus);
  }

  /* Send signals to our thread so they can check the global variable */
  if((pthreadStatus = pthread_kill(processThread,SIGUSR1))!=0){
    printf("%s %d\n","process thread signal error",pthreadStatus);
  }
  /* Wait for the threads to join */
  printf("%s\n","Waiting for process thread to close");
  if((pthreadStatus = pthread_join(processThread, NULL))!=0){
    printf("%s %d\n","Process Thread join error",pthreadStatus);
  }
  /* Send signals to our thread so they can check the global variable */
  if((pthreadStatus = pthread_kill(reportThread,SIGUSR2))!=0){
    printf("%s %d\n","report thread signal error",pthreadStatus);
  }
  /* Wait for the threads to join */
  printf("%s\n","Waiting for report thread to close");
  if((pthreadStatus = pthread_join(reportThread, NULL))!=0){
    printf("%s %d\n","Report Thread join error",pthreadStatus);
  }

  /* Close file and destroy the mutex, attributes and conditions variavles*/
  fclose(fptr);
  if((pthreadStatus = pthread_attr_destroy(&attr))!=0){
    printf("%s %d\n","Attribute destroy error",pthreadStatus);
  }
  if((pthreadStatus = pthread_mutex_destroy(&mapMutexLock))!=0){
    printf("%s %d\n","Map Mutex destroy error",pthreadStatus);
  }
  if((pthreadStatus = pthread_mutex_destroy(&dataMutexLock))!=0){
    printf("%s %d\n","Data Mutex destroy error",pthreadStatus);
  }
  if((pthreadStatus = pthread_mutex_destroy(&threadMutexLock))!=0){
    printf("%s %d\n","Thread Mutex destroy error",pthreadStatus);
  }
  if((pthreadStatus = pthread_cond_destroy(&condVal))!=0){
    printf("%s %d\n","Thread condition error",pthreadStatus);
  }
  printf("%s\n","Adios Amigos!");
  exit(1);
}

/* Handler to process our file data */
void processHandler(void)
{
  char file_char;
  char file_word[20];
  fileStats.charcount = 0;
  fileStats.wordcount = 0;
  fileStats.linecount = 0;
  uint32_t i,j;
  char temp[1]= {0};
  uint32_t temp_count;
  active_count++;

  /* Put lock before we start reading data */
  if((pthreadStatus = pthread_mutex_lock(&dataMutexLock))!=0){
    printf("%s %d\n","Data Mutex lock error - ",pthreadStatus);
  }
  printf("%s\n","Processing Data");
  /* Start processing from the last processed location */

  fseek(fptr,last_position-new_position,SEEK_CUR);
  if (fptr)
  {
    if((pthreadStatus = pthread_mutex_lock(&mapMutexLock))!=0){
      printf("%s %d\n","Map Mutex lock error - ",pthreadStatus);
    }
    /* Read each new word from the file and map it */
    while (fscanf(fptr,"%s",file_word) != EOF)
    {
      map_word(file_word);
    }

    i = 0;
    uniqueWordCount = 0;

    /* Get the number of unique words that are actually added to the mapping
       array */
    while(word_count[i]!=0)
    {
      uniqueWordCount++;
      i++;
    }

    /* Rearranging the words in a descending order so that we get the words
       with high occurence at the initial positions in the array */
    for (i = 0; i < uniqueWordCount; i++)
    {
      for (j = 0; j < uniqueWordCount-i-1; j++)
      {
        if (word_count[j+1] > word_count[j])
        {
          strcpy(temp,words[j]);
          strcpy(words[j],words[j+1]);
          strcpy(words[j+1],temp);
          temp_count = word_count[j];
          word_count[j] = word_count[j+1];
          word_count[j+1] = temp_count;
        }
      }
    }
    if((pthreadStatus = pthread_mutex_unlock(&mapMutexLock))!=0){
      printf("%s %d\n","Map Mutex unlock error - ",pthreadStatus);
    }
    /* Start file processing from the beginning */
    fseek(fptr,0,SEEK_SET);
    while((file_char=getc(fptr)) != EOF)
    {
      //Increment character count if it is not a new line
      if (file_char != '\n')
      {
        fileStats.charcount++;
      } // Increment line count if new line character is encountered
      else
      {
        fileStats.linecount++;
      }
      // Increment word count if we encounter a new line or space character
      if (file_char == ' ' || file_char == '\n')
      {
        fileStats.wordcount++;
      }
    }
  }
  else
  {
    printf("Failed to open the file\n");
  }

  /* Store the last location till the file was processed */
  last_position = ftell(fptr);
  if((pthreadStatus = pthread_mutex_unlock(&dataMutexLock))!=0){
    printf("%s %d\n","Data Mutex unlock error - ",pthreadStatus);
  }

  active_count--;
  //printf("%s\n","Processed");
  /* Send signal to the SIGINT handler if the signal was Received
     while processing */
  if(quit_request == 1)
  {
    if((pthreadStatus = pthread_cond_signal(&condVal))!=0){
      printf("%s %d\n","Pthread condition signal error - ",pthreadStatus);
    }
  }
}

/* Thread used to receive our SIGUSR1 signal and process file data */
void *processFunction(void *t)
{
  pthread_t tid = pthread_self();
  printf("Processing Thread ID - %ld\n",tid);
  sigset_t signalToCatch;
  sigemptyset(&signalToCatch);
  /* Add the signal we want to catch for this thread */
  sigaddset(&signalToCatch, SIGUSR1);
  uint32_t signalCaught;
  while(1)
  { /* Wait for the signal specified above */
    sigwait(&signalToCatch,(int *) &signalCaught);
    if(quit_request == 1)
    {
      break;
    }
    printf("%s\n","SIGUSR1 signal sent");
    /* Call the function we want this thread to handle */
    processHandler();
  }
  printf("%s\n","Processing break hua");
  pthread_exit (NULL);
  return NULL;
}

/* Handler to report file data to the terminal */
void reportHandler(void)
{
  int i;
  active_count++;
  printf("\n%s\n","Reporting Data");
  printf("Char count - %d\n", fileStats.charcount);
  printf("Word count - %d\n", fileStats.wordcount);
  printf("Line count - %d\n", fileStats.linecount);

  printf("\n%s\n","Top 5 words:");

  /* Lock to prevent the array from being arranged before printing */
  if((pthreadStatus = pthread_mutex_lock(&mapMutexLock))!=0){
    printf("%s %d\n","Map Mutex lock error - ",pthreadStatus);
  }
  for(i = 0; i < ((uniqueWordCount<5)?uniqueWordCount:5) ; i++)
  {
    printf("Word - %s, Count %d\n",words[i],word_count[i]);
  }
  if((pthreadStatus = pthread_mutex_unlock(&mapMutexLock))!=0){
    printf("%s %d\n","Map Mutex unlock error - ",pthreadStatus);
  }
  active_count--;
  /* Send signal to the SIGINT handler if the signal was Received
     while processing */
  if(quit_request == 1)
  {
    if((pthreadStatus = pthread_cond_signal(&condVal))!=0){
      printf("%s %d\n","Pthread condition signal error - ",pthreadStatus);
    }
  }
}

/* Thread used to receive our SIGUSR2 signal and report data */
void *reportFunction(void *t)
{
  pthread_t tid = pthread_self();
  printf("Reporting Thread ID - %ld\n",tid);
  sigset_t signalToCatch;
  sigemptyset(&signalToCatch);
  /* Add the signal we want to catch for this thread */
  sigaddset(&signalToCatch, SIGUSR2);
  uint32_t signalCaught;

  while(1)
  { /* Wait for the signal specified above */
    sigwait(&signalToCatch,(int *) &signalCaught);
    if(quit_request == 1)
    {
      break;
    }
    printf("%s\n","SIGUSR2 signal sent");
    /* Call the function we want this thread to handle */
    reportHandler();
  }
  printf("%s\n","Reporting break hua");
  pthread_exit (NULL);
  return NULL;
}

int main (int argc, char *argv[])
{
  sigset_t signalsToBlock;
  sigemptyset(&signalsToBlock);

  /* Add signals that we want to block for this and all threads that will be created */
  sigaddset(&signalsToBlock,SIGUSR1);
  sigaddset(&signalsToBlock,SIGUSR2);

  /* Block the signals using pthread_sigmask */
  pthread_sigmask(SIG_BLOCK, &signalsToBlock, NULL);
  char fileName[50] = {0};
  char fileData[DATA_TO_READ_PER_LINE] = {0};
  pthread_t tid = pthread_self();
  printf("Main Thread ID - %ld\n",tid);

  /* Get the name of the file from user before thread creation */
  printf("%s\n","Enter a file name to create:");
  fgets(fileName,50,stdin);

  /* Add NULL terminator to user input*/
  if ((strlen(fileName)>0) && (fileName[strlen (fileName) - 1] == '\n'))
    fileName[strlen (fileName) - 1] = '\0';
  fptr=fopen(fileName,"w+");
  printf("Starting location - %ld\n",ftell(fptr));

  /* Lock for mapping data */
  if((pthreadStatus = pthread_mutex_init(&mapMutexLock, NULL))!=0){
    printf("%s %d\n","Map Mutex init signal error - ",pthreadStatus);
  }
  /* Lock for data write and process */
  if((pthreadStatus = pthread_mutex_init(&dataMutexLock, NULL))!=0){
    printf("%s %d\n","Data Mutex init signal error - ",pthreadStatus);
  }

  /* Lock to be used for our condition variable used to signal by threads */
  if((pthreadStatus = pthread_mutex_init(&threadMutexLock, NULL))!=0){
    printf("%s %d\n","Thread Mutex init signal error - ",pthreadStatus);
  }

  /* Initialize attribute object and condition variable objects */
  if((pthreadStatus =  pthread_cond_init (&condVal, NULL))!=0){
    printf("%s %d\n","Thread condition variable init error - ",pthreadStatus);
  }

  if((pthreadStatus =  pthread_attr_init(&attr))!=0){
    printf("%s %d\n","Attribute init error - ",pthreadStatus);
  }

  /* Now create the child threads for processing and reporting */
  if((pthreadStatus =  pthread_create(&processThread, &attr, processFunction, NULL))!=0){
    printf("%s %d\n","Process thread create error - ",pthreadStatus);
  }

  if((pthreadStatus =  pthread_create(&reportThread, &attr, reportFunction, NULL))!=0){
    printf("%s %d\n","Report thread create error - ",pthreadStatus);
  }

  /* Add a signal handler for our SIGINT signal ( Ctrl + C )*/
  signal(SIGINT,closingHandler);

  printf("%s\n","Write data to file: ");

  /* Keep writing data to the file till we receive a SIGINT signal */
  while(quit_request==0)
  { /* Read user input */
    if(fgets(fileData,DATA_TO_READ_PER_LINE,stdin)==NULL){
		  printf("Error reading data\n");
		  break;
	  }

    /* Lock the mutex before writning to the file */
    if((pthreadStatus = pthread_mutex_lock(&dataMutexLock))!=0){
      printf("%s %d\n","Data Mutex lock error - ",pthreadStatus);
    }
    if(fwrite(fileData,strlen(fileData),1,fptr)<0)
    {
      printf("Error writting file\n");
    }
    if((pthreadStatus = pthread_mutex_unlock(&dataMutexLock))!=0){
      printf("%s %d\n","Data Mutex unlock error - ",pthreadStatus);
    }

    /* Store the value of the new file pointer position in a variable which is
       used in processing thread later */
    new_position = ftell(fptr);
    memset(fileData,0,sizeof(fileData));
    //printf("Writing location - %ld\n",ftell(fptr));

    /* If quit_request is set due to SIGINT handler then exit */
    if(quit_request == 1)
    {
      break;
    }
    /* Else keep adding new data */
    printf("%s\n","Accept new data!");
  }
  return 0;
}
