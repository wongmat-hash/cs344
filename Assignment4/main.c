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
#include <errno.h>                                                              //for processing if its a file to process or user input stdin: work cited: https://stackoverflow.com/questions/3495092/read-from-file-or-stdin
#include <math.h>

#define SIZE 1000                                                               //define a buffer for our buffer thread
#define MAX_SIZE 1000

//buffer 1 shared between user input and line seperator thread
char buffer_1[SIZE];
//number of items in the buffer
char count_1 = 0;
//index where input thread will put the next items
char prod_id_1 = 0;
//index where the line seperator thread will pick up the next items
char con_idx_1 = 0;
//init the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
//init the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

//buffer 2 shared resource between line seperator and plus sign thread
char buffer_2[SIZE];
//number of items in the buffer
char count_2 = 0;
//index where the line seperator will put the next item
char prod_idx_2 = 0;
//index where the plus sign will pick up the next item
char con_idx_2 = 0;
//init the mutex for buffer 2
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
//init the conditional var for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

//buffer 3 shared between the plus sign thread adn the output thread
char buffer_3[SIZE];
//number of items in the buffer
char count_3 = 0;
//index where the plus sign will put the next item
char prod_idx_3 = 0;
//index where the output will pick up the next item
char con_idx_3 = 0;
//init the mutex for buffer 3
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
//init the conditional var for buffer 3
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;



//implement single threaded version 1x COMPLETE 02/16

//THREAD 1 user input thread
//function to process file/ userinput
//.......void userInput(char *arr)                                                       //work cited: https://www.programiz.com/c-programming/c-arrays-functions
char userInput()
{
//------------------WORKING 02/16
  //for (int i = 0; i < MAX_SIZE; i++)                                            //loop through to the size of our array and scanf into our arr work cited: https://www.geeksforgeeks.org/why-to-use-fgets-over-scanf-in-c/
  //{
  //  scanf("%c", &arr[i]);                                                       //use scanf since we know buffer size 1000 and put it into our arr
  //}
  //printf("\nUSERINPUT:\n%s\n", arr);                                            //store user input up to 1000 chars in our array
//---------------------------------------------------
  char entry;                                                                   //store the char we are processing individually
  scanf("%c", &entry)
  return entry;                                                                 //return it so we can use it

  //now the array has user input or user specified input from < when starting program
}

//put item from user input into the first buffer
void put_buff_1(char char)
{
  //lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_1);
  //put item in the buffer
  buffer_1[prod_idx_1] = char;
  //increment the index where the next item will be put
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;
  //signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_1);
  //unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}
//this function that the input thread will run get input from the user put it into the buffer shared with the line seperator
void *get_input(void *args)
{
  for (int i = 0; i < MAX_SIZE; i++)
  {
    char happy = userInput();                                                   //loop through and store the grabbed char and then put it into the buffer
    put_buff_1(happy);                                                          //store it into the buffer
  }
}
//get the next item from buffer 1
char get_buff_1
{
  //lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while(count_1 == 0)
  {
    //buffer is empty wait for the prod to signal that the buffer has data
    pthread_cond_wait(&full_1, & mutex_1);
    char item = buffer_1[con_idx_1];
    //increment the index from which the item will be picked up
    con_idx_1 = con_idx_1 + 1;
    count_1--;
    //unlock the mutex
    pthread_mutex_unlock(&mutex_1);
    //return the item
    return item;
  }
}




//THREAD 2 line seperator thread
//function called line seperator thread replcaes every line seperator in the input by a space
void lineSeperator(char *arr)
{
  //go through and search for STOP\n
  for (int i = 0; i < MAX_SIZE; i++)                                            //use a loop to find the STOP since we have to find before \n
  { //check for the \n first because if not it would see any STOP\n as STOP
    if (((arr[i]) == '\n') && ((arr[i+1]) == 'S') && ((arr[i+2]) == 'T') && ((arr[i+3]) == 'O') && ((arr[i+4]) == 'P') && ((arr[i+5]) == '\n'))
    {
      //everything after the STOP\n can be ignored because its uncessary
      for (int x = i; x < MAX_SIZE; x++)                                      //added a +1 becuase we were seeing unexpected symbols: work cited https://stackoverflow.com/questions/51523477/array-showing-random-characters-at-the-end
      {
        arr[x] = ' ';
      }
    }
  }

  //printf("LINE SEPERATOR FUNCTION:\n");                                       //test that we are in the function
  for (int i = 0; i < MAX_SIZE; i++)                                          //use a for loop to iterate the through the array, find \n and then delete with space
  {
    if (arr[i] == '\n')                                                         //work cited: https://stackoverflow.com/questions/13106108/strcmp-and-new-line-characters-from-text-file
    {
      //arr[i] = arr[i+1];                                                      //delete that line seperator
      arr[i] = ' ';                                                             //error working with spaces above. Now without quotes it functions correctly work cited: https://stackoverflow.com/questions/30033582/what-is-the-symbol-for-whitespace-in-c
    }
  }
}

//thread 3 plus sign removal thread
//function called plus sign thread replaces every pair of ++ with ^
void plusplusSign(char *arr)
{
  printf("PLUSPLUSSIGN:\n");
  for (int i = 0; i < MAX_SIZE; i++)                                            //use a for loop and conditionals to find our ++
  {
    if ((arr[i] == '+') && (arr[i+1] == '+'))                                   //check if they are double ++
    {
      for (int x = i; x < MAX_SIZE; x++)                                        //if they are we need to shift the array elements over by 1 work cited: https://stackoverflow.com/questions/879603/remove-an-array-element-and-shift-the-remaining-ones
      {
        arr[x] = arr[x+1];                                                      //shift the remaining elements in the array
      }
      arr[i] = '^';                                                             //now that its all been shifted we can delete the remaining + and replace with ^
    }
  }
  printf("%s", &arr[0]);
}

//thread 4 output thread
//function called output thread that writes processed data to standard output as lines of exactly 80 it also needs to know to stop if there are < 80 when it hits a STOP
void outPutThread(char *arr)
{
  //first we need to process into lines of 80
  int len = 0;                                                                  //stores the len of the array to calculate
  for (int i = 0; i < MAX_SIZE; i++)
  {
    if ((arr[i] == ' ') && (arr[i+1] == ' '))
    {
      break;
    }
    len++;
  }
  printf("LEN: %d\n", len);
  //take the length calculated and divide by 80
  len = len / 80;
  printf("NEW LEN: %d\n", len);
  //store that variable
  int z = len;                                                                  //this should be 4 in our tests
  //since len is an int we dont need to worry about decimal points it will automatically cut off
  //now we need to multiply this by 80 to get the amount to display up to:
  len = len * 80;                                                               //this should be 320 in our tests
  printf("NEW 2.0 LEN: %d\n", len);
  int counter = len;
  //now display 80 char chunks
  int eighty = 80;
  int bub = 0;
  int zebra = 2;
  for (int x = 0; x < z; x++)                                              //loop through the number of times possible 4 times in our test
  {
    //printf("x value: %d\n", x);
    for (int yolo = bub; yolo < eighty; yolo++)
    {
      printf("%c", arr[yolo]);
    }
    //printf("\nbub = %d", bub);
    //printf("\neighty = %d", eighty);
    bub = eighty;
    //printf("\nzebra = %d", zebra);
    eighty = 80*(zebra);
    zebra++;
    //printf("\nzebra = %d", zebra);
    //printf("\nbub = %d", bub);
    //printf("\neighty = %d", eighty);
    printf("\n");
  }
}




int main(int argc, char* argv[])
{
  //int bool = 0;                                                               //use this to find a STOP\n
  char ar[MAX_SIZE];                                                            //char array of 1000 that will store user input or standard argument
  userInput(ar);                                                                //pass the array into user Input to grab and store data

  //check that the user input is stored correctly TEST
  //printf("this is your string in MAIN:\n%s\n", ar);                           //TESTS to make sure our values are passing around correctly
  //printf("TESTING ARRAY: %s\n", &ar[594]);

  //test our array buffer                                                       //tests to make sure data is correct
  //int x = sizeof(ar);
  //printf("\nTEST\n");
  //printf("\n%d\n", x);

  //pass the array into linesepeator functino which will reaplce every line seperator in the input by a space
  lineSeperator(ar);
  //printf("\nBOOLEAN: %d\n", bool);                                            //testing to make sure our boolean was triggered correctly
  printf("\nSTRING AFTER LINE SEP:\n%s\n", ar);
  //x = sizeof(ar);
  //printf("\nTEST\n");
  //printf("\n%d\n", x);

  //now pass into our plusplusSign function which will remove ++ and replace with ^
  plusplusSign(ar);
  printf("\nnow back in main after plusplusSign\n");
  printf("\nAR array[0] test: %c\n", ar[0]);
  //printf("test:\n");
  //for (int i = 0; i < 500; i++)
  //{
  //  printf("%c\n", ar[i]);
  //}
  outPutThread(ar);
  return 0;
}
