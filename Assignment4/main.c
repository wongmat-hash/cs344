//Assignment4 Multi-Threaded Producer Consumer Pipeline
//Matthew Wong CS344 Winter 2021
// Write a program that creates 4 threads to process input from standard input as follows:
//1. Thread 1: called the input thread reads in lines of characters from the standard input
//2. Thread 2: called the line separator thread replaces every line separator in the input by a space
//3. Thread 3: callled the plus sign thread replcaes every pair of plus signs, ie "++" by a "^"
//4. thread 4: called the output thread, write the processed data to standard output as lines of exactly 80 characters
// The threads must communicate with each other using the producer-consumer approach

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//init our mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//init our conditional vars
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

//function for our producer WORK CITED LECTURE CONDITION VARIABLES
void *producer(void *args)
{
    for (int i = 0; i < num_items + 1; i++)
    {
      // Produce the item outside the critical section
      int value = produce_item(i);
      // Lock the mutex before checking where there is space in the buffer
      pthread_mutex_lock(&mutex);
      while (count == SIZE)
        // Buffer is full. Wait for the consumer to signal that the buffer has space
        pthread_cond_wait(&empty, &mutex);
      put_item(value);
      // Signal to the consumer that the buffer is no longer empty
      pthread_cond_signal(&full);
      // Unlock the mutex
      pthread_mutex_unlock(&mutex);
      // Print message outside the critical section
      printf("PROD %d\n", value);
    }
    return NULL;
}

//function that consumer thread will run. Get item from buffer if the buffer is not empty WORK CITED LECTURE CONDITION VARIABLES 
void *consumer(void *args)
{
  int value = 0;
  //continue consuming until the END_MARKER is seen
  while (value != END_MARKER)
  {
    pthread_mutex_lock(&mutex);
    while (count == 0)
    {
      //buffer is empty and we need to wait for the producer to signal that the buffer has data
      pthread_cond_wait(&full, &mutex);
    }
    value = get_item();
    //signal to the producer that the buffer has space since its empty
    pthread_cond_signal(&empty);
    //unlock the mutex
    pthread_mutex_unlock(&mutex);
    //print the message outside of the critical section
    printf("CONSUMER: %d\n", value);
  }
  return NULL;
}
