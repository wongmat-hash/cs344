#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//this file is for testing methods

#define MAX_SIZE 1000

void userInput(char *arr)
{
  printf("enter an input:\n");
  gets(arr);
  printf("testing that stored correctly: %s\n", arr);
}

void stdInput(char *arr)
{
  printf("processing input\n");
}
int main(int argc, char* argv[])
{
  char array[MAX_SIZE];
  if (argc != 2)
  {
    userInput(array);
  }
  if (argc < 2)
  {
    stdInput(array);
  }
}
