#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
  srandom(time(NULL));                                                          //to start our random number generator
  char *randomized_String;                                                      //this is char to store randomzed string
  if (argc != 2)
  {
    printf("please enter a longer string\n");
    exit(1);
  }
  if (argc == 2)
  {
  int string_Length;
  string_Length = atoi(argv[1]);                                              //convert the string to numbers
  randomized_String = (char*)malloc(sizeof(char)*(string_Length+1));
  static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  for (int i = 0; i < string_Length; i++)
  {
    randomized_String[i] = alphabet[rand() % (sizeof(alphabet) - 1)];           //loop through the array and put random character
  }
  randomized_String[string_Length] = 0;
  printf("%s\n", randomized_String)
  }
  free(randomized_String);                                                      //free up the string
  return 0;
}
