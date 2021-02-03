//****************************
// Program title: smallsh
// CS344 Winter 2021
// Matthew Wong
// Program objective: In this assignbment you will write smallsh your own shell in C.
// Smallsh will implement a subset of features of well-known shells, such as bash.
// Your program will:
// 1. Provide a prompt for running commands
// 2. Handle blank lines and comments, which are lines beginning with the # character
// 3. Provide expansion for the varaible $$
// 4. Execute 3 commands exit, cd, and status via code built into the shell 02/01/21 COMPLETE
// 5. Execute other commands by creating new processes using a function from the exec family of functions
// 6. Support input and ouput redirection
// 7. Support running commands in foreground and background processes
// 8. Implement custom handler for 2 signals SIGINT, SIGTSTP
//****************************
#include <stdio.h>                                                              //for printf, perror
#include <string.h>
#include <stdlib.h>                                                             //for exit
#include <signal.h>
#include <unistd.h>                                                             //for execv, getpid, getppid, fork, execlp
#include <sys/types.h>                                                          //for pid_t
#include <sys/wait.h>                                                           //for waitpid
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

typedef enum {false, true} bool;                                                //typedef for our boolean false and true

#define MAX_ALLOWED 512
#define MAX_USER_INPUT 2048

//**************************
// PID converter
// since bash uses $$ we need to convert it into process ID
// https://unix.stackexchange.com/questions/291570/what-is-in-bash
//**************************
void pidConverter(char* Userinput)
{
  char* storage;                                                                //declare a storage char
  storage = strdup(Userinput);                                                  //use strdup to copy the user input into the storage
  for (int i; i < strlen(storage); i++)                                         //use a loop to the length of the char we just copied
  {
    if ((storage[i] == '$') && (i+1 < strlen(storage)) && (storage[i+1] == '$'))//if the first inputs are $ check second for $
    {
      storage[i] = '%';                                                         //set the first to %
      storage[i+1] = 'd';                                                       //set second to d so we can print an char
    }
  }
  sprintf(Userinput, storage, getpid());                                        //use sprintf to convert char to int
  free(storage);                                                                //free up the array
}
//**************************
// Argument input functions
// this function will take in our user input
// we need to store user input in an array to parse
//**************************
int userArg(char** argList, char* input)
{
  int counter = 0;                                                              //initialize a counter storage int
  memset(argList, '\0', MAX_ALLOWED);                                           //flush the input stream
  char* token;                                                                  //declare a token buffer see: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
  token = strtok(input, "\n");                                                  //get the first token
  while(token != NULL)                                                          //walk through other tokens and check them
  {
    argList[counter++] = strdup(token);                                         //store the token into the array we pass in using strdup to duplicate: https://stackoverflow.com/questions/252782/strdup-what-does-it-do-in-c
    token = strtok(NULL, "\n");                                                 //in the loop continue the token until new line is detected
  }
  return counter;                                                               //returns our counter
}

//**************************
// User input which passes to userArg to convert to argument
// will pass to the userArg for parsing
//**************************
int userEntry(char** argList)
{
  int counter;                                                                  //declare a counter var;
  char* entry = malloc(MAX_USER_INPUT * sizeof(char));                          //declare a char and allocate memory size of the max defined above
  do
  {
    printf(": ");                                                               //for our bash display
    fgets(entry, MAX_USER_INPUT, stdin);                                        //read and store into the entry
    entry[strcspn(entry, "\n")] = 0;                                            //use strcspn to scan entry and look for new line
  }while(strlen(entry) < 1 || strlen(entry) > MAX_USER_INPUT || entry[0] == '#');

  pidConverter(input);                                                          //pass to our PID converter we made above
  counter = userArg(argList, entry);                                            //set the counter we initialized to the return value of the userArg function
  free(entry);                                                                  //free the char pointer

  return counter;                                                               //returns the pointer back up
}
//**************************
// we need a function to clear out our char array
// it needs to loop through and clean each element to clear memory
//**************************
void freeMemory(char** argList, int counter)
{
  for (int i; i < counter; i++)                                                 //use a simple for loop
  {
    free(argList[i]);                                                           //for each element in the array it frees up the memory
  }
}
//**************************
// argument redirection functions
// work cited: https://stackoverflow.com/questions/26070346/c-how-do-you-accept-a-command-line-argument-via-redirection
//**************************
void redirectionArgs(char** argList, int counter, bool bgStatus)
{
  int targetFD;
  char* file = NULL;
  bool redirectYes = false;

  for (int i = 1; i < counter; i++)                                             //loop through with for loop to check for redirection arguments
  {
    if ((strcmp(argList[i], "<") == 0) || (strcmp(argList[i], ">")== 0))
    {
      redirectYes = true;
      file = strdup(argList[i + 1]);
      if (bgStatus)
      {
        targetFD = open("/dev/null", 0_RDONLY);
        if (dup2(targetFD, STDIN_FILENO) == -1)                                 //work cited: https://stackoverflow.com/questions/12902627/the-difference-between-stdout-and-stdout-fileno
        {
          fprintf(stderr, "ERROR redirecting...");
          exit(1);
        };
        if (dup2(targetFD, STDOUT_FILENO) == -1)
        {
          fprintf(stderr, "ERROR redirecting...");
          exit(1);
        };
      }
      else
      {
        if ((strcmp(argList[i], "<") == 0))
        {
          targetFD = open(fileName, 0_RDONLY);
          if (targetFD == -1)
          {
            fprint(stderr, "cannot open %s for input\n", file);
            exit(1);
          }
          if (dup2(targetFD, STDIN_FILENO) == -1)
          {
            fprint(stderr, "ERROR redirecting...");
            exit(1);
          }
        }
        else
        {
          targetFD = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
          if (targetFD == -1)
          {
            fprintf(stderr, "Cannot open %s for output\n", file);
            exit(1);
          }
          if (dup2(targetFD, STDOUT_FILENO) == -1)
          {
            fprint(stderr, "ERROR redirecting...");
            exit(1);
          }
        }
      }
      close (targetFD);
      free(file);
    }
  }
  if (redirectYes)
  {
    for (int x = 1; x < counter; x++)
    {
      argList[x] = NULL;
    }
  }
}
//**************************
// termination message for child process
//**************************
void childStatus(int status)
{
  if (WEFEXITED(status))
  {
    printf("exit Value %d\n", WEXITSTATUS(status));
  }
  else if (WIFSIGNALED(status))
  {
    printf("terminated by signal %d\n", WTERMSIG(status));
  }
}
