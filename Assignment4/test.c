#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//this file is for testing methods
#define SIZE 130
#define MAX_SIZE 130

//buffer 1 shared between user input and line seperator thread
char buffer_1[SIZE];
//number of items in the buffer
char count_1 = 0;
//index where input thread will put the next items
char prod_idx_1 = 0;
//index where the line seperator thread will pick up the next items
char con_idx_1 = 0;
//init the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
//init the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

//scan for input function
char scan_for_input()
{
  char entry;                                                                   //char for the current char it scans
  scanf("%c", &entry);                                                          //scans for it and stores it in the char
  //fgets(entry, MAX_SIZE, scanf)
  return entry;                                                                 //returns it to the calling function
}

//function to put the first grabbed char into the buffer
void put_buff_1(int item)
{
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_1);
  // Put the item in the buffer
  buffer_1[prod_idx_1] = item;
  // Increment the index where the next item will be put.
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_1);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}

void *get_input(void *args)
{
  for (int i = 0; i < MAX_SIZE; i++)
  {
    //get the input from scan
    char currentChar = scan_for_input();
    put_buff_1(currentChar);                                                    //store the current char in the buffer
  }
  return NULL;
}

//get the next item from the previous buffer [CONSUMER PORTION]
char get_buff_2()
{
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  char currentChar = buffer_1[con_idx_1];
  // Increment the index from which the item will be picked up
  con_idx_1 = con_idx_1 + 1;
  count_1--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
  // Return the item
  return currentChar;
}


void *write_output(void *args) //[CALL TO CONSUMER PORTION BUFFER ]
{
  char currentChar;
  for (int i = 0; i < MAX_SIZE; i++)                                            //loop through our entire stored buffer to max size and write the char
  {
    currentChar = get_buff_2();
    printf("%c", currentChar);
  }
  return NULL;
}


int main(void)
{
  //create the pthread vars
  pthread_t input_t, output_t;                                                            //calls for the input thread
  //create a single thread
  pthread_create(&input_t, NULL, get_input, NULL);                               //create the thread for grabbing input
  pthread_create(&output_t, NULL, write_output, NULL);
  //wait for the threads to terminate
  pthread_join(input_t, NULL);
  pthread_join(output_t, NULL);
  return EXIT_SUCCESS;
}
