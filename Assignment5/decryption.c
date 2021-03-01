#include <time.h>
#include <sys/sendfile.h>
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

void connect_To_Server(char* port_String, char *ciphertext_File, char *key_File)
{
  int sockfd;
  struct socaddr_in remote_addr;
  int port_Number = atoi(port_String);
  char response_to_Handshake[2];
  bzero(response_to_Handshake, 2);

  //get the socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    printf("Error: failed to obtain socket descriptor.\n");
    exit(2);
  }
  //fill in the address data  for struct
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port =htons(port_Number);
  inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr);
  bzero(&(remote_addr.sin_zero), 8);
  //connect
  if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockadddr)) == -1)
  {
    printf("Error: could not contact otp_dec_d onport %s\n", port_String);
    exit(2);
  }
  request_server(sockfd);
  response_server(sockfd, response_server);

  if (strcmp(response_server, "R") == 0)
  {
    fprintf(stderr, "Error could not contact server on port %s\n", port_String);
    exit(1);
  }
  else if (strcmp(response_to_Handshake, "T") == 0)
  {
    printf("Error: server rejected the client due to lack of space\n");
    exit(1);
  }

}



int main(int argc, char *argv[])
{
  if (argc!= 4)                                                                 //check if there are enough arguments
  {
    printf("Please enter command in this format XXXX\n");
    exit(1);
  }
  //cipher text is receieved
  //figure out length of cipher
  //compare sizes to the size of the key
  //save the cipher text into the string
  //check the ciphertext from out of bounds chars
  //send the text to the server
  FILE *fp = fopen(argv[1], "rb");
  if (fp == 0)
  {
    printf("text file does not exist\n");
    exit(1);
  }
  fseek(fp, 0, SEEK_END);                                                       //goes to the end of the file https://www.tutorialspoint.com/c_standard_library/c_function_fseek.htm
  long cipher_size = ftell(fp);                                                 //figure out the total size and store the file size in bytes: https://www.tutorialspoint.com/c_standard_library/c_function_ftell.htm
  fseek(fp, 0, SEEK_SET)                                                        //find the end of file

  char *ciphertext_string = malloc(ciphertext + 1);
  fread(ciphertext_string, cipher_size, 1, fp);
  fclose(fp);
  ciphertext_string[cipher_size] = 0;
  swap_Null(ciphertext_string);

  //get size of key
  //compare size of key to the size of ciphertext;
  //save key as string
  //analyze a key string from out of bounds

  fp = fopen(argv[2], "rb");
  if (fp == 0)
  {
    printf("key file does not exist\n");
    exit(1);
  }
  fseek(fp, 0, SEEK_END)                                                        //goes to end of file with the pointer
  long key_size = ftell(fp);                                                    //find the size of the file
  fseek(fp, 0 ,SEEK_SET);                                                       //allocate mem for key string
  //allocate memory for the key string
  char *key_String = malloc(key_Size + 1);
  //get the key string from the file
  fread(key_String, key_Size, 1, pointer_to_File);
  //close the file
  fclose(fp);
  //add a null char to the end of the string
  key_String[key_Size] = 0;
  //check for the new line if its there it should be a null
  swap_New_Line_With_Null_Characters(key_String);
  //validate the length of the key it should not be shortened shorter than the original text
  validate_Key_Length(key_Size, ciphetext_Size, argv[2]);
  //check for invalid characters
  invalid_Characters(ciphertext_String, ciphetext_Size);
  invalid_Characters(keyString, keySize);

  //connect to server and send plaintext and key
  //wait for server response
  connect_To_Server(argv[3], argv[1], argv[2]);
  //free string
  free(ciphertext_String);
  free(key_String);
  return 0;
}
