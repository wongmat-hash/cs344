//CS344 01/21/21
//Matthew Wong
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#define PREFIX "movies_"

//menu A prompts
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

//menu loop prompts
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

//this function finds the size of the files
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

//this function will process our file
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
                                                                                //work cited using portion from: https://oregonstate.instructure.com/courses/1798831/pages/exploration-directories?module_item_id=20163866 to open directory
          DIR* currDir = opendir(".");                                          //opens the current directory
          struct dirent *aDir;                                                  //pointer for dirent
          struct stat dirStat;                                                  //delcaration of stat struct for files
          char entryName[256];                                                  //stores our largest filename
          int i = 0;                                                            //for our comparison between files
          if (aDir == NULL)                                                     //if our file was null then proceed with error message
          {
            printf("There was an error opening the directory\n");
            return EXIT_FAILURE;
          }
          while((aDir = readdir(currDir))!= NULL)                               //otherwise go through all the entries
          {
            //look only at the prefix student files
            if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX))==0)
            {
              printf("filename: %s\n", aDir->d_name);                           //prints the file name
              //TEST                                                            work cited: https://stackoverflow.com/questions/29548013/loop-through-a-file-and-print-file-attributes-in-c
              //display the file information for testing
              printf("total size, in bytes: %lld\n", dirStat.st_size);
              printf("\n");
              if ((stat(aDir->d_name, &dirStat)) == -1)
              {
                printf("SOME ERROR HAPPENED IN THE WHILE LOOP\n");
              }
              if (i == 0)
              {
                //set the entryName to the current file being looked at because its the ONLY movies prefix csv
                memset(entryName, '\0', sizeof(entryName));
                strcpy(entryName, aDir->d_name);
              }
              if (i !=0)
              {
                //that means we have a prefix file with movie already so we need to compare the file size and see which one is greater

              }
              i++;                                                              //means we looked through 1 file with prefix movie 
            }
          }
          closedir(currDir);
          printf("The largest file with prefix: \"%s\" modified in the current directory is: %s\n", PREFIX, entryName);
          printf("\n");
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
