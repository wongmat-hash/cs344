#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef enum { false, true } bool;

#define MAX_AGR_NUMBER 512
#define MAX_INPUT_LENGTH 2048



/*************************************************************************************
**Function: int getArgList(char**, char*)
**Description: Parses a string into an array of words separated by white spaces or \n
**Arguments:
	(char**) an array of strings that hold the parsed words
	(char*) string to be parsed
**Returns: (int) the number of parsed words
*************************************************************************************/
int getArgList(char** argList, char* input)
{
	int numberOfArg = 0;
	memset(argList, '\0', MAX_AGR_NUMBER);
	char* token = strtok(input, " \n");
	while(token != NULL)
	{	// Seperate each arguement and store them into argList
		argList[numberOfArg++] = strdup(token);
		token = strtok(NULL, " \n");
	}
	return numberOfArg;
}

/*************************************************************************************
**Function: void convertPID
**Description: Iterates a string and converts the first "$$" to process id
**Arguments: (char*) string to be iterated
**Returns: none
*************************************************************************************/
void convertPID(char* input)
{
	char* buffer = strdup(input);
	int i;
	for (i = 0; i < strlen(buffer); i ++)
	{
		// Change $$ to %d, then sprintf
		if ((buffer[i] == '$') && (i + 1 < strlen(buffer)) && (buffer[i + 1] == '$'))
		{
			buffer[i] = '%';
			buffer[i + 1] = 'd';
		}
	}
	sprintf(input, buffer, getpid());
	free(buffer);
}

/*************************************************************************************
**Function: int getUserCommand(char**)
**Description: Gets user command and validates its input.
**Arguments: (char**) store the command into array of strings
**Returns: (int) number of arguments
*************************************************************************************/
int getUserCommand(char** argList)
{
	int numberOfArg;
	char* input = malloc(MAX_INPUT_LENGTH * sizeof(char));
	do
	{
		printf(": ");
		fgets(input, MAX_INPUT_LENGTH, stdin);
		input[strcspn(input, "\n")] = 0; // Remove trailing newline character from fgets
	}while(strlen(input) < 1 || strlen(input) > MAX_INPUT_LENGTH || input[0] == '#');

	convertPID(input); // Convert $$ to process id
	numberOfArg = getArgList(argList, input); // Parse user input into array of arguments
	free(input);

	return numberOfArg;
}

/****************************************************************
**Function: void cleanUpMemory(char**, int)
**Description: Clean up arguments list after it has been called
**Arguments:
	(char**) list of arguments
	(int) number of arguments
**Returns: none
*****************************************************************/
void cleanUpMemory(char** argList, int numberOfArg)
{
	int i;
	for (i = 0; i < numberOfArg; i++)
	{
		free(argList[i]);
	}
}

/**************************************************************************************
**Function: void checkForFileRedirection(char**, int, bool)
**Description:
	Checks through command list and see if there are redirection arguments.
	If there is, then redirect to correct file
**Arguments:
		(char**) list of arguments
		(int) number of arguments
		(bool) checks if command is ran in background or foreground
**Returns: none
**************************************************************************************/
void checkForFileRedirection(char** argList, int numberOfArg, bool ranOnBackground)
{
	int targetFD;
	char* fileName = NULL;
	bool hasRedirection = false;

	int i;
	for (i = 1; i < numberOfArg; i++)
	{	// Check for any redirections
		if ((strcmp(argList[i], "<") == 0) || (strcmp(argList[i], ">") == 0))
		{
			hasRedirection = true;
			fileName = strdup(argList[i + 1]);
			if (ranOnBackground)
			{	// Background process does not display in shell - pass all output and input to /dev/null
				targetFD = open("/dev/null", O_RDONLY);
				if (dup2(targetFD, STDIN_FILENO) == -1)
				{
					fprintf(stderr, "Error redirecting");
					exit(1);
				};
				if (dup2(targetFD, STDOUT_FILENO) == -1)
				{
					fprintf(stderr, "Error redirecting");
					exit(1);
				};
			}
			else
			{	// Non-background processes
				if ((strcmp(argList[i], "<") == 0))
				{
					targetFD = open(fileName, O_RDONLY);
					if (targetFD == -1)
					{
						fprintf(stderr, "Cannot open %s for input\n", fileName);
						exit(1);
					}
					if (dup2(targetFD, STDIN_FILENO) == -1)
					{
						fprintf(stderr, "Error redirecting");
						exit(1);
					}
				}
				else
				{
					targetFD = open(fileName, O_CREAT | O_RDWR | O_TRUNC, 0644);
					if (targetFD == -1)
					{
						fprintf(stderr, "Cannot open %s for ouput\n", fileName);
						exit(1);
					}
					if (dup2(targetFD, STDOUT_FILENO) == -1)
					{
						fprintf(stderr, "Error redirecting");
						exit(1);
					};
				}
			}
			close(targetFD);
			free(fileName);
		}
	}
	// If there was file redirection, remove all arguments except for first command so that we can call execvp later
	if (hasRedirection)
	{
		for(i = 1; i < numberOfArg; i++)
		{
			argList[i] = NULL;
		}
	}
}

/***************************************************************************
**Function: void checkStatus(int)
**Description: Displays to the console how the last child was terminated
**Arguments: (int) status code
**Returns: none
****************************************************************************/
void checkStatus(int statusCode)
{
	if (WIFEXITED(statusCode))	// Terminated normally
	{
		printf("exit value %d\n", WEXITSTATUS(statusCode));
	}
	else if (WIFSIGNALED(statusCode))	// Killed by signal
	{
		printf("terminated by signal %d\n", WTERMSIG(statusCode));
	}
}

/***************************************************************************
**Function: void checkBackgroundProcess(int, int)
**Description:
	Loop to check if any of our background child processes has completed.
	If yes, then write to console if it was exited or signaled.
**Arguments:
	(int) process id
	(int) status code of terminated child process
**Returns: none
****************************************************************************/
void checkBackgroundProcess(int pid, int statusCode)
{
	do
	{ // Check if a child has terminated. However, do not wait for it.
	pid = waitpid(-1, &statusCode, WNOHANG);
		if (pid > 0)
		{ // waitpid returns the process id of terminated child
			if (WIFEXITED(statusCode) != 0) // Checks if exited
			{
				printf("background pid %d is done: exit value %d\n", pid, WEXITSTATUS(statusCode));
			}
			else if (WIFSIGNALED(statusCode) != 0) // If it was not exited, then it must have been killed by a signal
			{
				printf("background pid %d is done: terminated by signal %d\n", pid, WTERMSIG(statusCode));
			}
		}
	} while (pid > 0); // Loop stops once pid returns 0 (no child process has terminated).
}

/**************************************************************************************
**Function: void runSmallSh(char**, int)
**Description:
	Check command list for exit, cd, or status and executes. All other c
	commands are performed by a spanwed child process
**Arguments:
	(char**) list of arguments
	(int) number of arguments
**Returns: none
**************************************************************************************/
void runSmallSh(char** argList, int numberOfArg)
{

	struct sigaction SIGINT_action = {0};
	SIGINT_action.sa_handler = SIG_IGN; // Set to ignore SIGINT
	sigaction(SIGINT, &SIGINT_action, NULL);

	static int statusCode;
	bool ranOnBackground = false;

	if ((strcmp(argList[numberOfArg - 1], "&") == 0))
	{	// Check if process is ran in background
		ranOnBackground = true;
		argList[numberOfArg - 1] = NULL;
	}

	if (strcmp(argList[0], "exit") == 0)
	{ 	// User exits out of program
		cleanUpMemory(argList, numberOfArg);
		exit(0);
	}
	else if (strcmp(argList[0], "cd") == 0)
	{		// User changes directory
		if (numberOfArg < 2)  // If there are no other arguments other than cd, then go to home directory
		{
			chdir(getenv("HOME"));
		}
		else	// If there are more then 1 argument, the next argument must be the directory to go to
		{
			chdir(argList[1]);
		}
	}
	else if (strcmp(argList[0], "status") == 0)  	// User checks status of previous process
	{
		checkStatus(statusCode);
	}
	else
	{		// All other commands fork a child process
		pid_t pid = fork();
		if (pid == -1)
		{
			fprintf(stderr, "Error creating child\n");
			exit(1);
		} // Error forking
		if (pid == 0)
		{	// Child process
			if (!ranOnBackground)
			{	// child processes ran on foreground can be terminated
				SIGINT_action.sa_handler = SIG_DFL;
				sigaction(SIGINT, &SIGINT_action, NULL);
			}
			checkForFileRedirection(argList, numberOfArg, ranOnBackground);
			execvp(argList[0], argList);
			// If execvp was unsuccessful, then print out error and exit
			fprintf(stderr, "%s: command not found\n", argList[0]);
			exit(1);
		}
		else
		{ // Parent process
			if (ranOnBackground)
			{
				printf("background pid is %d\n", pid);
			}
			else
			{	// If ran in foreground, wait for child process to finish
				waitpid(pid, &statusCode, 0);
				if (WTERMSIG(statusCode) != 0)	// Check if child process was killed by signal
				{
					printf("terminated by signal %d\n", WTERMSIG(statusCode));
				}
				checkBackgroundProcess(pid, statusCode); // Check for any finished background processes
			}
		}
	}
	cleanUpMemory(argList, numberOfArg);
}

int main()
{
	int numberOfArg;
	char* argList[MAX_AGR_NUMBER];

	while (1)
	{
		// Prompt user command
		numberOfArg = getUserCommand(argList);
		// Execute command
		runSmallSh(argList, numberOfArg);
	}
	return 0;
}
