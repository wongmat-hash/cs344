//****************************
// Program title: smallsh
// CS344 Winter 2021
// Matthew Wong
// Program objective: In this assignbment you will write smallsh your own shell in C.
// Smallsh will implement a subset of features of well-known shells, such as bash.
// Your program will:
// 1. Provide a prompt for running commands 02/02/21 COMPLETE
// 2. Handle blank lines and comments, which are lines beginning with the # character 02/01/21 COMPLETE
// 3. Provide expansion for the varaible $$ 02/01/21 COMPLETE
// 4. Execute 3 commands exit, cd, and status via code built into the shell 02/01/21 COMPLETE
// 5. Execute other commands by creating new processes using a function from the exec family of functions
// 6. Support input and ouput redirection 02/02/21 COMPLETE
// 7. Support running commands in foreground and background processes
// 8. Implement custom handler for 2 signals SIGINT, SIGTSTP 02/02/21 COMPLETE
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

  pidConverter(entry);                                                          //pass to our PID converter we made above
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
// background process checker
//**************************
void bgStatus(int pid, int status)
{
  do
  {
    pid = waitpid(-1, &status, WNOHANG);                                        //wait for PID but return immediately if no child process is ready
    if (pid > 0)                                                                //if return value is anything above 0
    {
      if (WIFEXITED(status)!= 0)                                                //tell user that background process is done and show value
      {
        printf("background pid %d is done: exit value %d\n", pid, WEXITSTATUS(status));
      }
      else if (WIFSIGNALED(status) != 0)                                        //tell user that the background process was terminated and show msg
      {
        printf("background pid %d is done: terminated by signal %d\n", pid, WTERMSIG(status));
      }
    }
  }while (pid > 0);
}
//**************************
// termination message for child process
//**************************
void childStatus(int status)
{
  if (WIFEXITED(status))
  {
    printf("exit Value %d\n", WEXITSTATUS(status));
  }
  else if (WIFSIGNALED(status))
  {
    printf("terminated by signal %d\n", WTERMSIG(status));
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
    if ((strcmp(argList[i], "<") == 0) || (strcmp(argList[i], ">")== 0))        //if we find a char in our array matching < > as true
    {
      redirectYes = true;                                                       //flip our boolean value since its true
      file = strdup(argList[i + 1]);                                            //set our file buffer to the value after the redirection
      if (bgStatus)                                                             //if the boolean passed in is true
      {
        targetFD = open("/dev/null", O_RDONLY);                                 //pass output and input to /dev/null
        if (dup2(targetFD, STDIN_FILENO) == -1)                                 //work cited: https://stackoverflow.com/questions/12902627/the-difference-between-stdout-and-stdout-fileno
        {
          fprintf(stderr, "ERROR redirecting...");                              //print error message and exit loop
          exit(1);
        };
        if (dup2(targetFD, STDOUT_FILENO) == -1)
        {
          fprintf(stderr, "ERROR redirecting...");                              //print error message and exit loop
          exit(1);
        };
      }
      else                                                                      //if our boolean is anything but true we have no bg processes
      {
        if ((strcmp(argList[i], "<") == 0))                                     //check for redirections in our char array
        {
          targetFD = open(file, O_RDONLY);                                      //pass input and ouput to the file
          if (targetFD == -1)
          {
            fprintf(stderr, "cannot open %s for input\n", file);                //display our error message and exit loop
            exit(1);
          }
          if (dup2(targetFD, STDIN_FILENO) == -1)
          {
            fprintf(stderr, "ERROR redirecting...");                            //display error message and exit loop
            exit(1);
          }
        }
        else                                                                    //otherwise final case is no redirection
        {
          targetFD = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);              //open file with 0644 permissions -rw-r--r-- http://www.filepermissions.com/file-permission/0644
          if (targetFD == -1)
          {
            fprintf(stderr, "Cannot open %s for output\n", file);               //print error message and exit loop
            exit(1);
          }
          if (dup2(targetFD, STDOUT_FILENO) == -1)
          {
            fprintf(stderr, "ERROR redirecting...");                            //print error message and exit loop
            exit(1);
          }
        }
      }
      close(targetFD);                                                          //close our targetFD
      free(file);                                                               //close the fstream file
    }
  }
  if (redirectYes)                                                              //if we do have a redireciton (boolean triggered)
  {
    for (int x = 1; x < counter; x++)                                           //use a loop and set our array to null
    {
      argList[x] = NULL;
    }
  }
}
//**************************
// run smallsh function
//**************************
void runsmallSh(char** argList, int counter)
{
  struct sigaction SIGINT_action = {0};
  SIGINT_action.sa_handler = SIG_IGN;                                           //set the sa handler to ignore any SIGINT
  sigaction(SIGINT, &SIGINT_action, NULL);

  static int status;
  bool backgroundStatus = false;                                                //set our boolean to false

  if ((strcmp(argList[counter - 1], "&") == 0))                                 //check if we have a process running in the background first
  {
    backgroundStatus = true;                                                    //if we do then set our boolean trigger
    argList[counter - 1] = NULL;
  }

  if (strcmp(argList[0], "exit") == 0)                                          //if we see the exit command in input
  {
    freeMemory(argList, counter);                                               //free our argList and counter to freeMemoery function
    exit(0);                                                                    //exit program
  }
  else if (strcmp(argList[0], "cd") == 0)                                       //if we see a cd command in our input
  {
    if (counter < 2)                                                            //check if there are other arguments besides CD in input
    {
      chdir(getenv("HOME"));                                                    //none so navigate to the home directory
    }
    else                                                                        //otherwise change directory to the user input via argList
    {
      chdir(argList[1]);                                                        //navigate using CD to the user specified argList
    }
  }
  else if (strcmp(argList[0], "status") == 0)                                   //if user inputted status we need to display status of process runnig
  {
    childStatus(status);                                                        //call our childStatus function to check for any child process running
  }
  else                                                                          //no other user inputs besides CD/ STATUS/ EXIT so fork a child process
  {
    pid_t pid = fork();                                                         //declare our pid and fork
    if (pid == -1)                                                              //if a -1 returns display standard error message for fork process
    {
      fprintf(stderr, "Error creating child\n"); exit(1);                       //error message is displayed
    }
    if (pid == 0)                                                               //if 0 is returned we can fork
    {
      if (!backgroundStatus)                                                    //check our boolean if we need to terminate foreground processes
      {
        SIGINT_action.sa_handler = SIG_DFL;                                     //termination of forground processes
        sigaction(SIGINT, &SIGINT_action, NULL);
      }
      redirectionArgs(argList, counter, backgroundStatus);                      //check for any redirection arguments using our function
      execvp(argList[0], argList);                                              //execute the argList see: https://linux.die.net/man/3/execvp
      fprintf(stderr, "%s: command not found\n", argList[0]);                   //if unsuccessful exec then we need to display our prompt
      exit(1);                                                                  //exit
    }
    else                                                                        //otherwise we are in our parent process
    {
      if (backgroundStatus)                                                     //check for boolean for foreground process
      {
        printf("background pid is %d\n", pid);                                  //print our background process pid
      }
      else                                                                      //otherwise we need to use waitpid to wait for child process to complete
      {
        waitpid(pid, &status, 0);                                               //wait for our child process
        if (WTERMSIG(status) != 0)                                              //if user input signal then we need to check if it was terminated
        {
          printf("terminated by signal %d\n", WTERMSIG(status));                //print the message
        }
        bgStatus(pid, status);                                                  //use our background process check function to check that it was correctly terminated
      }
    }
  }
  freeMemory(argList, counter);                                                 //user our free memory function to clear memory
}


int main()
{
  int counter;
  char* argList[MAX_ALLOWED];

  while(1)
  {
    counter = userEntry(argList);
    runsmallSh(argList, counter);
  }
  return 0;
}
