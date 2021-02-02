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
// 4. Execute 3 commands exit, cd, and status via code built into the shell
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

//****************************
// BUILT IN FUNCTIONS
// This function will check the user input to see if it matches:
// exit, cd, status
// if it hits a match it will process and run that specific call
//****************************
int builtinCheck()
[
  if (strcmp(argArr[0], "cd")==0)                                               //we will use strcmp to string compare to see if we get a true for cd
  {
    builtIn = true;                                                             //trigger our boolean and set it to true
    backGround = false;                                                         //since its a built in no need to run it in the background
    builtinCD(argArr);                                                          //pass the argument to the CD function
    backGround = false;                                                         //background is still false
  }
  else if (strcmp(argArr[0], "status")==0)                                      //otherwise if our function returns true for status
  {
    builtIn = true;                                                             //trigger our boolean
    backGround = false;                                                         //since its built in no need to run it in the backGround
    if (WIFEXITED(currenState))                                                 //check if the child process terminated normally
    {
      printf("Exit Status: %d\n", WEXITSTATUS(currenState));                    //print the exit status of the child process
    }
    else if (WIFEXITED(backgroundStatus))                                       //check to see if the child process of the background process exited
    {
      printf("Exit Status: %d\n", WEXITSTATUS(backgroundStatus));;              //print the exit status of the child process
    }
    else if (WIFSIGNALED(currenState))                                          //check the exit status of the current process
    {
      if (currenState != -5)                                                    //if its been correctly initialized
      {
        printf("Terminated by signal: %d\n", WTERMSIG(currenState));            //print the exist status
      }
    }
  }
  else if (strcmp(argArr[0], "exit")==0)                                        //otherwise if our string compares and matches exit true
  {
    builtIn = true;                                                             //set our booleans for built in that way we dont run background
    backGround = false;                                                         //set background to false since we don't need to run
    if (numPids > 0)                                                            //if our counter for pids is greater than 0 then processes are running
    {
      killPids();                                                               //kill all processes running since we exiting
    }
    exit(0);
  }
  else if ()
]
