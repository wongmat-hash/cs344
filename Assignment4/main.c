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

#define MAX_SIZE 1000



//implement single threaded version 1x

//function to process file/ userinput
void userInput(char *arr, int size)                                             //work cited: https://www.programiz.com/c-programming/c-arrays-functions
{
  fgets(arr, MAX_SIZE, stdin);                                                  //work cited: https://beginnersbook.com/2014/01/c-passing-array-to-function-example/
  printf("this is your string in USERINPUT: %s\n", arr);                        //store user input up to 1000 chars in our array
}

//function called line seperator thread replcaes every line seperator in the input by a space
void lineSeperator(char *arr)
{
  
}
//function called plus sign thread replaces every pair of ++ with ^

//function called output thread that writes processed data to standard output as lines of exactly 80

int main(int argc, char* argv[])
{
  char ar[MAX_SIZE];                                                            //char array of 1000 that will store user input
  userInput(ar, MAX_SIZE);                                                      //pass the array into user Input to grab and store data

  //check that the user input is stored correctly TEST
  printf("this is your string in MAIN: %s\n", ar);

  //test our array buffer
  int x = sizeof(ar);
  printf("\nTEST\n");
  printf("\n%d\n", x);

  //pass the array into linesepeator functino which will reaplce every line seperator in the input by a space
  lineSepeator(ar);
  return 0;
}
