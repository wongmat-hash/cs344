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

#define MAX_SIZE 1000



//implement single threaded version 1x

//function to process file/ userinput
void userInput(char *arr)                                                       //work cited: https://www.programiz.com/c-programming/c-arrays-functions
{
  for (int i = 0; i < MAX_SIZE; i++)                                            //loop through to the size of our array and scanf into our arr work cited: https://www.geeksforgeeks.org/why-to-use-fgets-over-scanf-in-c/
  {
    scanf("%c", &arr[i]);                                                       //use scanf since we know buffer size 1000 and put it into our arr
  }

//OLD STUFF TO TEST//
  //do
  //{
  //  fgets(arr, MAX_SIZE, stdin);
  //}while (fgets(arr, sizeof(arr), stdin)!= NULL);


  //while (fgets(arr, sizeof(arr), stdin) != NULL)
  //{
  //  fgets(arr, MAX_SIZE, stdin);
  //}
  //int count = 80;
  //do
  //{
  //fgets(arr, MAX_SIZE, stdin);
  //  count--;
  //}while (count != 0);                                                        //work cited: //work cited: https://beginnersbook.com/2014/01/c-passing-array-to-function-example/
  //scanf("%255s", arr);
  printf("this is your string in USERINPUT:\n%s\n", arr);                       //store user input up to 1000 chars in our array
  //now the array has user input or user specified input from < when starting program
}

//function called line seperator thread replcaes every line seperator in the input by a space
void lineSeperator(char *arr)
{
  printf("in line seperator function\n");                                       //test that we are in the function
  for (int i = 0; i < MAX_SIZE; i++)                                            //use a for loop to iterate the through the array, find \n and then delete with space
  {
    if (arr[i] == '\n')                                                         //work cited: https://stackoverflow.com/questions/13106108/strcmp-and-new-line-characters-from-text-file
    {
      //arr[i] = arr[i+1];                                                      //delete that line seperator
      arr[i] = ' ';                                                             //error working with spaces above. Now without quotes it functions correctly work cited: https://stackoverflow.com/questions/30033582/what-is-the-symbol-for-whitespace-in-c
    }
  }

//OLD WORK//
  //size_t length = strlen(arr);                                                //now we have hte length of the array
  //printf("\nsize_t: %zu\n", length);
  //if (arr[length-1] == '\n')
  //{
  //  arr[--length]= '\0';                                                      //work cited: https://stackoverflow.com/questions/28429625/check-if-string-contains-new-line-character
  //}
}
//function called plus sign thread replaces every pair of ++ with ^
void plusplusSign(char *arr)
{
  printf("in plusplus sign function\n");
  for (int i = 0; i < MAX_SIZE; i++)
  {
    if ((arr[i] == '+') && (arr[i+1] == '+'))
    {
      for (int x = i; x < MAX_SIZE; x++)
      {
        arr[x] = arr[x+1];
      }
      arr[i] = '^';
    }
  }
  //OLD STUFF//
  //printf("testing this is MAX_SIZE: %d\n", MAX_SIZE);
  //for (int i = 0; i < MAX_SIZE; i++)
  //{
  //  printf("%s ", &arr[i]);
  //}
  printf("%s", &arr[0]);
}
//function called output thread that writes processed data to standard output as lines of exactly 80

int main(int argc, char* argv[])
{
  char ar[MAX_SIZE];                                                            //char array of 1000 that will store user input or standard argument
  userInput(ar);                                                                //pass the array into user Input to grab and store data

  //check that the user input is stored correctly TEST
  //printf("this is your string in MAIN:\n%s\n", ar);                             //TESTS to make sure our values are passing around correctly
  //printf("TESTING ARRAY: %s\n", &ar[594]);

  //test our array buffer
  int x = sizeof(ar);
  printf("\nTEST\n");
  printf("\n%d\n", x);

  //pass the array into linesepeator functino which will reaplce every line seperator in the input by a space
  lineSeperator(ar);
  printf("this is your string in MAIN2:\n%s\n", ar);
  x = sizeof(ar);
  printf("\nTEST\n");
  printf("\n%d\n", x);

  //now pass into our plusplusSign function which will remove ++ and replace with ^
  plusplusSign(ar);
  printf("\n\nnow back in main after plusplusSign\n");
  printf("AR array[0] test: %c\n", ar[0]);
  //printf("test:\n");
  //for (int i = 0; i < 262; i++)
  //{
  //  printf("%c\n", ar[i]);
  //}
  return 0;
}
