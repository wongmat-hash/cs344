#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//this file is for testing methods
#define SIZE 50
#define MAX_SIZE 50
#define END_MARKER -1

//buffer 1 shared between user input and line seperator thread
char buffer_1[SIZE];
//number of items in the buffer
int count_1 = 0;
//index where input thread will put the next items
int prod_idx_1 = 0;
//index where the line seperator thread will pick up the next items
int con_idx_1 = 0;
//init the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
//init the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

//buffer 2 shared between user line seperator and test thread
char buffer_2[SIZE];
//number of items in the buffer
int count_2 = 0;
//index where input thread will put the next items
int prod_idx_2 = 0;
//index where the line seperator thread will pick up the next items
int con_idx_2 = 0;
//init the mutex for buffer 1
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
//init the condition variable for buffer 1
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

//scan for input function ACCOUNTED
char scan_for_input()
{
  printf("\nIN SCAN FOR INPUT\n");
  char entry;                                                                   //char for the current char it scans
  scanf("%c", &entry);                                                          //scans for it and stores it in the char
  //fgets(entry, MAX_SIZE, scanf)
  return entry;                                                                 //returns it to the calling function
}

//function to put the first grabbed char into the buffer ACCOUNTED
void put_buff_1(char item)
{
  printf("\nIN PUT BUFF 1\n");
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_1);
  // Put the item in the buffer
  buffer_1[prod_idx_1] = item;
  // Increment the index where the next item will be put.
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;
  printf("count_1 in put buff: %d", count_1);
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_1);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}

void *get_input(void *args) // ACCOUNTED
{
  printf("\nIN GET INPUT\n");
  for (int i = 0; i < MAX_SIZE; i++)
  {
    //get the input from scan
    char currentChar = scan_for_input();
    printf("get input is calling put buff 1\n");
    put_buff_1(currentChar);                                                    //store the current char in the buffer
  }
  return NULL;
}

//ACCOUNTED
//get the next item from the previous buffer [CONSUMER PORTION] PULLS FROM FIRST BUFFER INPUT
char get_buff_1()
{
  printf("\nIN GET BUFF 1\n");
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  printf("count_1 value in get buff1: %d\n", count_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  char currentChar = buffer_1[con_idx_1];
  // Increment the index from which the item will be picked up
  con_idx_1 = con_idx_1 + 1;
  count_1--;
  printf("count_1: %d\n", count_1);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
  // Return the item
  return currentChar;
}

//function for line seperator to put char buff for next function PUTS INTO 2nd BUFFER
void put_buff_2(char item)
{
  printf("\nIN PUT BUFF 2\n");
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_2);
  // Put the item in the buffer
  buffer_2[prod_idx_2] = item;
  // Increment the index where the next item will be put.
  prod_idx_2 = prod_idx_2 + 1;
  count_2++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_2);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}
//ACCOUNTED
//line seperator function for now it will just seperate lines we need way to check for STOP
void *lineSeperator(void *args)
{
  printf("\nIN LINE SEPERATOR\n");
  char currentChar;                                                             //stores the current char value
  //for (int i = 0; i < MAX_SIZE; i++)
  //{
    printf("line sep is calling getBuff 1\n");
    currentChar = get_buff_1();                                                  //grab the char from the input buffer
    //printf("%c", currentChar);                                                  //simple test to check for the grab
    //if (currentChar == "\n")                                                    //if we see a new line
    if (strcmp(&currentChar, "\n") == 0)
    {
      //printf("IS THIS WHERE I BREAK?\n");
      currentChar = ' ';                                                        //make it a space
      //break;
    }
    //printf("is this where i break\n");
    put_buff_2(currentChar);                                                    //now put it into the next buffer
  //}
  //printf("back in line seperator\n");
  return NULL;
}

//get buff 2 pulls from 2nd buffer created by line seperator function above
char get_buff_2()
{
  printf("\nIN GET BUFF 2\n");
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_2);
  sleep(20);
  while (count_2 == 0)
  printf("buffer 2 is empty\n");
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_2, &mutex_2);
  char currentChar = buffer_2[con_idx_2];
  // Increment the index from which the item will be picked up
  con_idx_2 = con_idx_2 + 1;
  count_2--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
  // Return the item
  return currentChar;
}


void *write_output(void *args) //[CALL TO CONSUMER PORTION BUFFER ]
{
  printf("\nIN WRITE OUTPUT\n");
  char currentChar;
  for (int i = 0; i < MAX_SIZE; i++)                                            //loop through our entire stored buffer to max size and write the char
  {
    printf("write Output is trying to call get buff 2\n");
    currentChar = get_buff_2();
    printf("%c", currentChar);
  }
  printf("\n");
  return NULL;
}


int main(void)
{
  //create the pthread vars
  pthread_t input_t, lineSep_t, output_t;                                       //calls for the input thread
  //create a single thread
  printf("in main creating thread 1\n");
  pthread_create(&input_t, NULL, get_input, NULL);                              //create the thread for grabbing input
  printf("in main creating thread 2\n");
  pthread_create(&lineSep_t, NULL, lineSeperator, NULL);                        //create the thread for line sepeartor
  printf("in main creating thread 3\n");
  pthread_create(&output_t, NULL, write_output, NULL);                          //create the thread for output
  //wait for the threads to terminate
  pthread_join(input_t, NULL);
  pthread_join(lineSep_t, NULL);
  pthread_join(output_t, NULL);
  return EXIT_SUCCESS;
}
