//CS344 01/21/21
//Matthew Wong
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#define MAX 100


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

long int findSize(char file_name[])
{
  //open the file and read
  FILE* fp = fopen(file_name, "r");

  //check if the file exists
  if (fp == NULL)
  {
    printf("File Not Found!\n");
    return -1;
  }

  fseek(fp, 0L, SEEK_END);
  //calculate the size of the file after reading to end
  long int res = ftell(fp);

  fclose(fp);
  //returns the value back to main
  return res;
}

void processing(FILE *fp, char f[])
{
  printf("Now processing the chosen file named %s\n\n", f);
}


int main()
{

  int userChoice, userOption;                                                   //user storage options
  do
  {                                                                             //our do while loop helps our error validation
    userChoice = menuA();                                                       //set the return value to what the menu prompts for userChoice 1-2
    if (userChoice == 1)                                                        //if the user enters 1 we need to go to secondary menu
    {
      //printf("IN USER CHOICE 1a\n");
      do
      {                                                                         //secondary menu which will prompt user 1-3
        userOption = menuB();
        if (userOption == 1)                                                    //work cited: https://www.geeksforgeeks.org/c-program-find-size-file/
        {
          //printf("IN USER CHOICE 1b\n");
          char file_name[] = {"movies_sample_1.csv"};
          long int res = findSize(file_name);
          if (res != -1)
          {
            printf("Size of file is %ld bytes \n", res);
            printf("\n");
          }
        }
        if (userOption == 2)
        {
          //printf("IN USER CHOICE 2b\n");
          //make a function for smallest file in directory
        }
        if (userOption == 3)                                                    //work cited: https://stackoverflow.com/questions/19338929/how-to-get-user-input-in-fopen
        {
          //printf("IN USER CHOICE 3b\n");
          //makes a function for user entry file in directory
        char fileName[255];
        FILE *fp;
        printf("Please enter a fileName to open (example: movies): ");          //user prompt
        scanf("%s", fileName);                                                  //looks for the file and stores it in our fileName
        strcat(fileName, ".csv");                                               //append with csv
        fp = fopen(fileName, "r");
        if (fp == NULL)
        {
          printf("There was an error trying to open your file. Please try again\n");
          printf("\n");                                                         //error validation for user specified file
        }
        else if (fp !=NULL)
        {
          //send to processing
          processing(fp, fileName);
        }
        fclose(fp);                                                             //close our file
        }
        else                                                                    //reprompt user which loops
        {
          printf("Please enter a valid menu entry\n");
          //userOption = menuB();                                                 //reprompt so menuB is displayed again
        }
      }while (userOption !=1 || userOption !=2 || userOption !=3 || userOption !=4);
    }
    if (userChoice == 2)
    {                                                                           //our exit statement
      //printf("IN USER CHOICE 2a\n");
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
