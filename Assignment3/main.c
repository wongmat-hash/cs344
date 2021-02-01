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
