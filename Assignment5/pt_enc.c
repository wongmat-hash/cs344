//CS344 Winter 2021
//Author: Matthew Wong
//Assignment5 One Time Pads
//Purpose: This is the client side for plain text. It will be needed to send the plain text to the server where it will then use a keygen
//the keygen will then encrypt the text and send it down the pipeline
//get the size of plaintext file from user input or file to open
//get the key from keygen
//compare size of plaintext to the size of key from keygen
//save the plaintext to a string storage
//check the plaintext if its all caps
//send the plaintext to server side

#include <time.h>
//#include <sys/sendfile.h>
#include <sys/types.h>                                                          //for mac we need to use: https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/sendfile.2.html
#include <sys/socket.h>
#include <sys/uio.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define LENGTH 512

//this function is used by our main to check if there are newlines, If there are newlines we replace them with null
void nullCheck(char *inputString)
{
  size_t len = strlen(inputString) - 1;                                         //we set the size_t var to the length of the input so the index size of array see: https://stackoverflow.com/questions/2550774/what-is-size-t-in-c
  if (inputString[len] == '\n')                                                 //if that value is a newline
  {
    inputString[len] = '\0';                                                   //we simply replace that value with a null instead of newline
  }
}

//test function to try out something fancy
void F_Function(FILE *fp, long Size, char *pt_String)
{
  fseek(fp, 0, SEEK_END);
  Size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  pt_String = malloc(Size + 1);
  fread(pt_String, Size, 1, fp);
}

//this requirement checks that the key size is the same as the ciphe text size. If its not then there is an issue
void check_keyLength(long key_Size, long c_Size, char *key_Name)
{
  if (key_Size < c_Size)                                                        //if the keysize is at all smaller than the ciphe text then we got an issue
  {
    printf("Error: key %s is too short\n", key_Name);                           //print out our error validation and exit our program
    exit(1);
  }
}

//function to check for any out of bounds chars
void char_Boundscheck(char *inputString, int inputLength)
{
  static const char bounds[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";                   //our library to check against all capital 26 and space char
  for (int i = 0; i < inputLength; i++)                                         //loop through the entire string char by char to end
  {
    if (strchr(bounds, inputString[i]) == 0)                                    //check if the input matches what we have in allowed dictionary
    {
      char error[] = "otp_enc error: input contains bad characters";            //if we find one display error message and exit
      fprintf(stderr, "%s\n", error);
      exit(1);
    }
  }
}

void send_Server(char *port_string, char *c_txt, char *key_txt)                 //work cited from oregon state provided server side c code https://repl.it/@cs344/83clientc?lite=true#client.c
{
  int sockfd, port_Num;
  struct sockaddr_in remote_addr;
  port_Num = atoi(port_String);
  char response_to_server[2];
  bzero(response_to_server);
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    printf("Error: failed to obtain socket descriptor\n");
    exit(2);
  }
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port =htons(port_Num);
  inet_pton(AF_INET,)
}

int main(int argc, char *argv[])
{
  if (argc!= 4)
  {
    printf("usage: pt_enc plaintext key port\n");                               //error handling if the user did not specify enough arguments
    exit(1);
  }

  FILE *fp = fopen(argv[1], "rb");                                              //our file pointer for opening user text
  if (fp == 0)
  {
    printf("Plaintext file does not exist\n");
    exit(1);
  }
  //fseek(fp, 0, SEEK_END);                                                       //fseek to the end of file this chunk will find the size of the file specified
  //long pt_Size = ftell(fp);                                                     //use ftell to figure out the length of the file see: https://www.tutorialspoint.com/ftell-in-c
  //fseek(fp, 0, SEEK_SET);                                                       //use fseek to return to the start of the file see: https://www.tutorialspoint.com/c_standard_library/c_function_fseek.htm
  //char *pt_String = malloc(pt_Size + 1);                                        //this is our storage var for what we read from the file need to allocate memory
  //fread(pt_String, pt_Size, 1, fp);                                             //get the string from the file by fread into the char we just made one line above

  long pt_Size;                                                                 //TESTING TESTING TESTING
  char *pt_String;                                                              //TESTING TESTING TESTING
  F_Function(fp, pt_Size, pt_String);                                            //TESTING TESTING TESTING

  fclose(fp);                                                                   //close our input stream
  pt_String[pt_Size] = 0;                                                       //add '\0' to the end of our stream so we know its the end
  nullCheck(pt_String);                                                         //make sure there are no new lines in the file (we delete those and replace with '\n'

  fp = fopen(argv[2], "rb");                                                    //open the 2nd argument which should be the keygen
  if (fp == 0)
  {
    printf("key file does not exist error\n");                                  //error handling
    exit(1);
  }
  //fseek(fp, 0, SEEK_END);                                                       //send our pointer to the end of our file
  //long key_length = ftell(fp);                                                  //store the length of the key in our key_length storage var
  //fseek(fp, 0, SEEK_SET);                                                       //point our pointer to the end of file
  //char *key_String = maloc(key_length +1);                                      //same steps above now but for keylength
  //fread(key_String, key_length, 1, fp);                                         //get the string from the file into our storage var

  long key_length;                                                              //TESTING TESTING TESTING
  char *key_String;                                                             //TESTING TESTING TESTING
  F_Function(fp, key_length, key_String);                                       //TESTING TESTING TESTING

  fclose(fp);                                                                   //close our input stream
  key_String[key_length] = 0;                                                   //add our own null terminator to the string

  //functions we need to build out check lengths, check char, send to server
  nullcheck(key_String);                                                        //make sure there are no new lines in the file
  check_keyLength(key_Size, pt_Size, argv[2]);                                  //we need to check that the sizes match and we are good to go still
  char_Boundscheck(pt_String, pt_Size);                                         //check the plain text for out of bounds char
  char_Boundscheck(key_String, key_Size);                                       //check our key for out of bounds chars

  send_Server(argv[3], argv[1], argv[2]);

  return 0;
}
