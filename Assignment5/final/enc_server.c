#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//this function will kick off the enc server side

static const char library[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";                  //our library of in bounds allowed chars for us to check

void encode(char*, char*, char*, int);                                          //function declarations for below
void decode(char*, char*, char*, int);

int otp_d(char*, char*);

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
    out[i] = library[in[i]];                                                    //the out array index is set to library based on input index
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s plaintext keyport\n", argv[0]);
    exit(1);
  }

  otp_d(argv[1], "otp_enc");
  return 0;
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
int otp_d(char* port_arg, char* enc_dec)
{
  int var = 0;
  int listenSocketFD, establishedConnectionFD, port_num, charL, status;
  socklen_t clientInfo;
  char buffer[512], inputFileName[256], keyFileName[256], inputText[80000], key[80000], outputText[80000];
  struct sockaddr_in serverA, clientA;
  pid_t pid, sid;

  memset((char *)&serverA, '\0', sizeof(serverA));                              //set up and clear out the struct for our address
  port_num = atoi(port_arg);                                                    //use atoi to convert from string to int for our port num from user input
  serverA.sin_family = AF_INET;                                                 //set up network protocols
  serverA.sin_port = htons(port_num);                                           //save the port number we passed in
  serverA.sin_addr.s_addr = INADDR_ANY;                                         //allow address's to connect to here

  listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);                             //prepare our socket
  if (listenSocketFD < 0)                                                       //create and set up socket procedures
  {
    error("ERROR opening socket");
  }
  if (bind(listenSocketFD, (struct sockaddr *)&serverA, sizeof(serverA)) < 0)   //set the socket using the port
  {
    error("ERROR on binding");
  }
  listen(listenSocketFD, 5);                                                    //turn on the socket to accept connections
  while(1)                                                                      //set the size of address to the client
  {
    clientInfo = sizeof(clientA);                                               //accept the connection to address
    establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientA, &clientInfo);
    if (establishedConnectionFD < 0)
    {
      error("ERROR on accept");
    }
    pid = fork();                                                               //fork here
    switch(pid)
    {
      case -1: ;                                                                //if we have issues with our fork display error
        error("Error creating fork");
        exit(0);
        break;
      case 0: ;                                                                 //if we have no errors in our fork
        memset(buffer, '\0', sizeof(buffer));                                   //process client message recieved from socket
        charL = recv(establishedConnectionFD, buffer, sizeof(buffer)-1, 0);     //process the message passed from the socket
        if (charL < 0)
        {
          error("ERROR reading from socket");
        }
        const char newline[2] = {'\n', '\0'};

        char *tkn = strtok(buffer, newline);                                    //this is our plaintext data
        strcpy(inputFileName, tkn);

        tkn = strtok(NULL, newline);                                            //use this to store our key data
        strcpy(keyFileName, tkn);

        tkn = strtok(NULL, newline);                                             //checker for file check
        if (strcmp(enc_dec, tkn))
        {
          fprintf(stderr, "ERROR %s cannot use %s_d.\n", tkn, enc_dec);
          var = 1;
        }

        if (!var)                                                               //process our file and conver them into processable strings
        {
          FILE *inFP = fopen(inputFileName, "r");
          fgets(inputText, 80000, inFP);
          fclose(inFP);
          inputText[strcspn(inputText, "\n")] = '\0';                           //delete the unsuable /n

          FILE *kFP = fopen(keyFileName, "r");
          fgets(key, 80000, kFP);
          fclose(kFP);
          key[strcspn(key, "\n")] = '\0';

          if (!strcmp(enc_dec, "otp_enc"))                                      //pass to the encoding function for encoding
          {
            encode(key, inputText, outputText, strlen(inputText));
          }
          else if (!strcmp(enc_dec, "otp_dec"))
          {
            decode(key, inputText, outputText, strlen(inputText));               //pass to the decoder function for decoding
          }
        }
        int uniquePid = getpid();
        char uniqueFile[32];
        sprintf(uniqueFile, "%s_f.%d", enc_dec, uniquePid);

        FILE *uniqueFD = fopen(uniqueFile, "w+");                               //pass our processed data into fopen to write to a new file
        if (var)
        {
          fprintf(uniqueFD, "");
        }
        else
        {
          fprintf(uniqueFD, "%s", outputText);
        }
        fclose(uniqueFD);
        charL = send(establishedConnectionFD, uniqueFile, strlen(uniqueFile), 0);
        if (charL < 0)                                                          //write the data to the scoket to pass it forward
        {
          error("Error writing to socket");
        }
        close(establishedConnectionFD);                                         //close everything that we open
        establishedConnectionFD = -1;
        exit(0);
        break;
      default: ;
    }
    close(establishedConnectionFD);                                             //more closing
    establishedConnectionFD = -1;
    wait(NULL);
  }
  close(listenSocketFD);                                                        //even more closing
}
