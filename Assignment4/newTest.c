#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000;

int sizeof_stdin;
//ALL CITED FROM EXAMPLE PROGRAM
char buffer_1[1000];
int count_1 = 0;
int prod_idx_1 = 0;
int con_idx_1 = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

//put buff function for get input (stores the stdin input)                      ALL CITED FROM EXAMPLE PROGRAM
void put_buff_1(char item)
{
  pthread_mutex_lock(&mutex_1);
  buffer_1[prod_idx_1] = item;
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;
  pthread_cond_signal(&full_1);
  pthread_mutex_unlock(&mutex_1);
}

//get input reads stdin by char and stores input into buffer array
//calls put buff 1 to store into array needs to +1 index when add 1
void *get_input(void *args)
{
  //char currentChar;
  //sizeof_stdin = lengthof(stdin);
  //fgets(&currentChar, sizeof(stdin), stdin);
  while(1)                                                                      //https://www.tutorialspoint.com/c_standard_library/c_function_feof.htm
  {
    char currentChar = fgetc(stdin);                                            //uses fgetc to read from stdin until eof
    if (feof(stdin))
    {
      return NULL;
    }
    sizeof_stdin = sizeof_stdin+1;
    //printf("size of inside getinput: %d\n", sizeof_stdin);
    put_buff_1(currentChar);
  }
  return NULL;
}

//get buff function for line seperator (pulls from buffer 1)
char get_buff_1()                                                               //ALL CITED FROM EXAMPLE PROGRAM
{
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
  {
    pthread_cond_wait(&full_1, &mutex_1);
  }
  char currentChar = buffer_1[con_idx_1];
  con_idx_1 = con_idx_1 + 1;
  count_1--;
  pthread_mutex_unlock(&mutex_1);
  return currentChar;
}
//line seperator will pull from buff1 array with get buff 1
//needs to adjust buffer 1 to +1 index so it knows to check next element
//put buff 2 will be called and it will be stored into 2nd buffer array

//plus sign thread will pull from buff 2 array with get buff 2
//needs to process and adjust buffer 1 to +1 index so it knows to check next element
//put buff 3 will be called and it will store into 3rd buffer array


//output thread will be called and pull from buff 3
//needs to print and adjust buffer to +1 so it knows to check for next element
void *write_output(void *args)
{
  //printf("in write output\n");
  char currentChar;
  printf("size of size: %d\n", sizeof_stdin);
  for (int i = 0; i < sizeof_stdin; i++)
  {
    currentChar = get_buff_1();
    printf("%c", currentChar);
  }
  printf("\n");
  return NULL;
}

int main(void)
{
  //create all 4 threads
  pthread_t input_t, linesep_t, plus_sign_t, output_t;

  pthread_create(&input_t, NULL, get_input, NULL);                              //create our thread for get Input


  pthread_join(input_t, NULL);                                                  //join the threads
  printf("in MAIN this is sizeof: %d\n", sizeof_stdin);
  pthread_create(&output_t, NULL, write_output, NULL);                          //create our thread for write output
  pthread_join(output_t, NULL);
  return EXIT_SUCCESS;
}