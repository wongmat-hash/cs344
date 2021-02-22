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

char scan_for_input()
{
  char *buffer;
  size_t bufsize = 1;
  size_t characters;

  buffer = (char *)malloc*(bufsize * sizeof(char));
  if (buffer == NULL)
  {
    perror("Unable to allocate buffer");
    exit(1);
  }
  characters = getline(&buffer, &bufsize, stdin);
}

void put_buf_1(char item)
{
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
  for (int i = 0; i < 49; i++)
  {
    char currentChar = scan_for_input();
    put_buff_1(currentChar);
  }
}

int main()
{
  pthread_t input_t;

  pthread_create(&input_t, NULL, get_input, NULL);

  pthread_join(input_t, NULL);
}
