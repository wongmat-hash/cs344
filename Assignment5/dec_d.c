#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//includes opt
static const char library[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";                  //our library of in bounds allowed chars

//function to display error messages
void error(const char *msg)
{
  perror(msg);                                                                  //displays error message
  exit(1);
}

//this function converts char to int
void convertChar_Int(char in[], int out[], int size)
{
  for (int i = 0; i < size; i++)                                                //loop through the entire size of the array
  {
    for (int y = 0; y < 27; y++)                                                //loop to the 27 possible chars that are legal
    {
      if (in[i] == library[y])                                                  //if one of the chars matches up with in our bounds
      {
        out[i] = y;                                                             //the output is assigned a number as an int
        break;                                                                  //break
      }
    }
  }
}

//this function converts an int to char opposite of above
void convertInt_Char(int in[], char out[], int size)
{
  for (int i = 0; i < size; i++)                                                //loop through the entire array
  {
    out[i] = library[in[i]];                                                       //the out array index is set to library based on input index
  }
}

//this function is called to encode a string
//takes in 1) key, 2) string, 3) output storage for string, 4) size of string in length
void encode(char key[], char in[], char out[], int size)
{
  int string_size[size], key_size[size], outputInt[size];                       //create storage for string size, and key size, and output
  //convert what is passed in to ints using function above
  convertChar_Int(in, string_size, size);                                       //pass the string to encode, array size of string, and size
  //convert what is passed in as key to int
  convertChar_Int(key, key_size, size);                                         //pass the key, array for key, and size of index
  //create the output as an int
  for (int i = 0; i < size; i++)
  {
    outputInt[i] = (string_size[i] + key_size[i]) % 27;                         //encode as part of the requirements with modulo 27 operations
  }
  //change the output to chars now
  convertInt_Char(outputInt, out, size);                                        //pass the output chars, out that was passed in and size
  out[size] = '\0';
}

//this function is a decoder for strings that are passed in
//takes in 1) key, 2) string to decode, 3) output storage for string, 4) size of the string in length
void decode(char key[], char in[], char out[], int size)
{
  int string_size[size], key_size[size], outputInt[size];                       //create storage for string size, and key size, and output
  //convert what is passed in to ints using function above
  convertChar_Int(in, string_size, size);                                       //pass the string to decode, array size of string, and size
  //convert what is passed in as a key to int
  convertChar_Int(key, key_size, size);                                         //pass the key, array for key, and size of index
  //create the output as int
  for (int i = 0; i < size; i++)
  {
    outputInt[i] = string_size[i] - key_size[i];
    if (outputInt[i] < 0)
    {
      outputInt[i] += 27;                                                       //reverse our modulo from above encode to decode properly
    }
  }
  //convert the output to chars now
  convertInt_Char(outputInt, out, size);                                        //pass the output chars, out that was passed in and size
  out[size] = '\0';
}

//function to perform all the encoding decoding
void otp(char* pt, char* key, char* port, char* enc)
{
  int socketFD, portNumber, charsWritten, charsRead;
  struct sockaddr_in serverAddress;
  struct hostent* serverHostInfo;
  char buffer[512];
  int port_x = atoi(port);

  FILE *fp = fopen(pt, "r");
  char text_plaintext[80000];
  fgets(text_plaintext, 80000, fp);
  fclose(fp);
  text_plaintext[strcspn(text_plaintext, "\n")] = '\0';

  FILE *kp = fopen(key, "r");
  char text_key[80000];
  fgets(text_key, 80000, kp);
  fclose(kp);
  text_key[strcspn(text_key, "\n")] = '\0';

  int textLength = strlen(text_plaintext);
  int keyLength = strlen(text_key);

  if (keyLength < textLength)
  {
    fprintf(stderr, "ERROR the key is shorter than the plaintext.\n");
    exit(1);
  }

  //plaintext checker
  for (int i = 0; i < textLength; i++)
  {
    for (int j = 0; i < 28; j++)
    {
      if (j == 27)
      {
        fprintf(stderr, "Error Plaintext contains invalid chars.\n");
        exit(1);
      }
      if (text_plaintext[i] == library[j])
      {
        break;
      }
    }
  }
  //key checker
  for (int i = 0; i < keyLength; i++)
  {
    for (int j = 0; j < 28; j++)
    {
      if (j == 27)
      {
        fprintf(stderr, "Key contains invalid chars.\n");
      }
      if (text_key[i] == library[j])
      {
        break;
      }
    }
  }

  //server setup
  memset((char*)&serverAddress, '\0', sizeof(serverAddress));
  portNumber = port_x;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(portNumber);
  serverHostInfo = gethostbyname("localhost");
  if (serverHostInfo == NULL)
  {
    fprintf(stderr, "CLIENT: ERROR, no such host exist\n");
    exit(0);
  }
  memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

  socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD < 0)
  {
    error("CLIENT: ERROR opening socket");
  }
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
  {
    error("CLIENT: Error connecting");
  }

  memset(buffer, '\0', sizeof(buffer));
  sprintf(buffer, "%s\n%s\n%s", pt, key, enc);

  charsWritten = send(socketFD, buffer, strlen(buffer), 0);
  if (charsWritten < 0)
  {
    error("CLIENT: Error writing to socket");
  }
  if (charsWritten < strlen(buffer))
  {
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  }
  memset(buffer, '\0', sizeof(buffer));

  charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
  if (charsRead < 0)
  {
    error("CLIENT: ERROR reading from socket");
  }

  FILE* recvFile = fopen(buffer, "r");
  char output[80000];
  fgets(output, 80000, recvFile);
  fclose(recvFile);
  remove(buffer);
  printf("%s\n", output);
  close(socketFD);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    fprintf(stderr, "Usage: %s plaintext keyport\n", argv[0]);
    exit(0);
  }

  otp(argv[1], argv[2], argv[3], "otp_dec");
  return 0;
}
