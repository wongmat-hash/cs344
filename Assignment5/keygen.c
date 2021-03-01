//CS344 Winter 2021
//Author: Matthew Wong
//Assignment5 One Time Pads
//Purpose: This keygen program will randomly create a key file depending on the user input length.
//Requirements: We are allowed to use 26 characters and space capital chars
//if there are more than 2 arguments then we need to create a keyfile
//then create a file of char keylength long with unix randomization
//allowed char of 26 char of alphabet and space
//do not add spaces between chars

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])                                                //to allow user input arguments
{
  static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";                 //library bank of all legal characters allowed to use including space
  srandom(time(NULL));                                                          //to start our random number generator for our random key
  char *rando;                                                                  //this is char to store randomzed key string generated
  if (argc != 2)                                                                //need to make sure our argument is long enough
  {
    printf("please enter a longer string\n");
    exit(1);                                                                    //exit out
  }
  if (argc == 2)                                                                //if the arguments are legal and enough
  {
    int input_length;                                                           //stores the length of the string calculated out
    input_length = atoi(argv[1]);                                               //convert the string to int using atoi() see https://www.tutorialspoint.com/c_standard_library/c_function_atoi.htm
    rando = (char*)malloc(sizeof(char)*(input_length+1));                       //allocate the memory needed for our randomized string
    //static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";                 //library bank of all legal characters allowed to use including space
    for (int i = 0; i < input_length; i++)                                      //loop from the beginning to the end of input length
    {
      rando[i] = alphabet[rand() % (sizeof(alphabet) - 1)];                     //loop through the array and put random character generated to the length of string
    }
    rando[input_length] = 0;                                                    //make the last element null terminated '\0' so we know its end of file
    printf("%s\n", rando);                                                      //print out the randomized string we generated
  }
  free(rando);                                                                  //free up the string
  return 0;
}
