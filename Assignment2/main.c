//CS344 01/21/21
//Matthew Wong
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

struct stat statbuffer;
struct dirent *entry;
if(S_ISREG(statbuffer.st_size)
{
  int i = sizeof(entry->d_name);
  printf("%d\n", i);
}

void findLargest(char *dir)                                                     //this function will find the largest file within a directory and return it
{
  //work cited: http://developerweb.net/viewtopic.php?pid=17081 found an example here of working with file sizes in a directory
  DIR *dp;
  struct dirent *entry;
  struct stat statbuf;
  struct stat statbuffer;
  int i = 0;
  char *name;

  if ((dp == opendir(dir))== NULL)                                              //our error msg if the directory cannot be found
  {
    fprint(stderr, "Cannot open director: %s\n", dir);
    return;
  }
  chdir(dir);
  while((entry=readdir(dp)) != NULL)
  {
    lstat(entry->d_name, &statbuf);
    {
      lstat(entry->d_name, &statbuffer);

      if(statbuffer.st_size >i)
      {
        i = (statbuffer.st_size);
        name = (entry->d_name);
        [b]                                                                     //names display fine here[/b]
        [b]printf("%s\n", name);[/b]
      }
    }
  }
  [b]//name will display here[/b]
  [b]printf("The largest file in the current direcotry is %s\n", name);[/b]
  printf("It is %i bytes\n", i);

  closedir(dp);
}

int menuA()                                                                     //first menu prompt
{
  int userChoice;
  printf("1. Select file to process\n");                                        //menu prompt
  printf("2. Exit the program\n");
  printf("Enter a choice 1 or 2: ");
  scanf(" %d", &userChoice);                                                    //takes in user input and stores it in userChoice
  printf("\n");
  return userChoice;
}

int menuB()                                                                     //second menu prompt
{
  int userOption;
  printf("Which file do you want to process?\n");
  printf("Enter 1 to pick the largest file\n");
  printf("Enter 2 to pick the smallest file\n");
  printf("Enter 3 to specify the name of a file\n");
  printf("Enter a choice from 1 to 3: ");
  scanf(" %d", &userOption);
  printf("\n");
  return userOption;
}

int main()
{
  int userChoice, userOption;
  do
  {                                                                             //our do while loop helps our error validation
    userChoice = menuA();                                                       //set the return value to what the menu prompts for userChoice 1-2
    if (userChoice == 1)                                                        //if the user enters 1 we need to go to secondary menu
    {
      do
      {                                                                         //secondary menu which will prompt user 1-3
        userOption = menuB();
        if (userChoice == 1)
        {
          //make function for largest file in directory
        }
        if (userChoice == 2)
        {
          //make a function for smallest file in directory
        }
        if (userChoice == 3)
        {
          //makes a function for user entry file in directory
        }
        else                                                                    //reprompt user which loops
        {
          printf("Please enter a valid menu entry\n");
          //userOption = menuB();                                                 //reprompt so menuB is displayed again
        }
      }while (userOption !=1 || userOption !=2 || userOption != 3 || userOption != 4);
    }
    if (userChoice == 2)
    {                                                                           //our exit statement
      exit(0);
    }
    else
    {
      printf("Please enter a valid menu entry\n");                              //reprompt user so MenuA is displayed again
      //userChoice = menuA();
    }
  }while(userChoice != 1 || userChoice != 2);
return 0;
}
