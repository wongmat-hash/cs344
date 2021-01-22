//CS344 01/21/21
//Matthew Wong
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
  int userChoice, userOption;                                                   //stores the user choice variable
  while (1)
  {
    printf("1. Select file to process\n");                                      //menu prompt
    printf("2. Exit the program\n");
    printf("Enter a choice 1 or 2: ");
    scanf("%d", &userChoice);                                                   //takes in user input and stores it in userChoice
    printf("\n");
    //test to see userChoice was stored correctly
    //printf("UserChoice: %d\n", userChoice);

    switch (userChoice)                                                         //switch for user based input from menu
    {
      case 1:                                                                   //if the user selects a file to process
      {
        //need to build out process to take in file
        printf("Which file do you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");
        printf("Enter a choice from 1 to 3: ");
        scanf("%d", &userOption);
        printf("\n");
        //test to see that userOption was stored correctly
        //printf("UserOption: %d\n", userOption);

        switch(userOption)                                                      //switch for UserOption
        {
          case 1:                                                               //picks the largest file in the directory
          {
            //build out largest function
          }
          case 2:                                                               //picks the smallest file in the directory
          {
            //build out smallest file
          }
          case 3:                                                               //specifies a name that the user can open
          {
            //build out user specified file

          }
        }
      }
      case 2:                                                                   //exits the program
      {
        exit(0);                                                                //exits the program
      }
    }
  }


  return 0;
}
