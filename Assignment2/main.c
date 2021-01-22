//CS344 01/21/21
//Matthew Wong
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>



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

int main(int argc, char *argv[])
{
  struct dirent *pDirent;
  DIR *pDir;
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
          //trying this out: https://stackoverflow.com/questions/3554120/open-directory-using-c
          if (argc!=2)
          {
            printf("Usage testprog <dirname>\n");
            return 1;
          }

          //test that we can open the directory
          pDir = opendir (argv[1]);
          if (pDir == NULL)
          {
            printf("Cannot open directory '%s'\n", argv[1]);
            return 1;
          }

          //process entries
          while((pDirent = readdir(pDir)) !=NULL)
          {
            printf("[%s]\n", pDirent->d_name);
          }

          //close the directory and exit
          closedir(pDir);
          break;
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
