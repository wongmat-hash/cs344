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
#include <time.h>
#include <fcntl.h>
#define PREFIX "movies_"

//struct for movies DONE
struct movie
{
  char *title;                                                                  //title
  char *year;                                                                   //year
  char *language;                                                               //language
  char *rating;                                                                 //rating value
  struct movie *next;                                                           //pointer to next movie
};
//function to delete our temp linked lists DONE
void deleteList(struct movie** head_ref)
{
  struct movie* current = *head_ref;
  struct movie* next;

  while (current != NULL)
  {
    next = current->next;
    free(current);
    current = next;
  }
  *head_ref = NULL;
}
//function to print our linked list DONE 1/18
void printList(struct movie *m)
{
  while(m !=NULL)
  {
    printf(" %s, ", m->title);
    printf("%s, ", m->year);
    printf("%s, ", m->language);
    printf("%s ", m->rating);
    printf("\n");
    m = m->next;
  }
}
//to create a new node DONE
struct movie *createMovie(char *currLine)
{
  int yea, rat;
  struct movie *currMovie = malloc(sizeof(struct movie));
  char *saveptr;
  //copy the title
  char *token = strtok_r(currLine, ",", &saveptr);
  currMovie->title = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currMovie->title, token);
  token = strtok_r(NULL, ",", &saveptr);
  currMovie->year = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currMovie->year, token);
  token = strtok_r(NULL, ",", &saveptr);
  currMovie->language = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currMovie->language, token);
  token = strtok_r(NULL, "\n", &saveptr);
  currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currMovie->rating, token);
  currMovie->next = NULL;
  return currMovie;
}

//function to swap our movie node data
void swapper(struct movie *a, struct movie *b)
{
  //declare a temp node
  struct movie tmp;
  tmp.title = a->title;
  tmp.year = a->year;
  tmp.language = a->language;
  tmp.rating = a->rating;
  a->title = b->title;
  a->year = b->year;
  a->language = b->language;
  a->rating = b->rating;
  b->title = tmp.title;
  b->year = tmp.year;
  b->language = tmp.language;
  b->rating = tmp.rating;
}
//function to sort by year then rating
void sorting(struct movie *head)
{
  int swapped, i;                                                               //storage variable
  struct movie *ptr1;
  struct movie *lptr = NULL;
  if (head == NULL)
  {
    return;
  }
  do
  {
    swapped = 0;                    //triger when its greater or equal not less                                                                 //initiate our counter for swap
    ptr1 = head;                                                                //set our pointer to the head list
    //ptr1 = ptr1->next;              //skip the first line since its our titles
    while (ptr1->next != lptr)                                                  //while our crwaler is not equal to null for next
    {
      //skip the first line since its our title and random fields
      int p1, p2;
      p1 = atoi(ptr1->year);
      p2 = atoi(ptr1->next->year);
      if(p1 > p2)
      {
        swapper(ptr1, ptr1->next);                                              //swap them
        swapped = 1;                                                            //set our counter
      }
      else if (p1 == p2)
      {
        float f1, f2;
        f1 = atof(ptr1->rating);  //convert our char to floats
        f2 = atof(ptr1->next->rating);
        if (f1 > f2)
        {
          swapper(ptr1, ptr1->next);
          swapped = 1;
        }
      }
      ptr1=ptr1->next;                                                          //set the next value so crwaler keeps going
    }
    lptr = ptr1;                                                                //move the next value to the crawler
  }while(swapped);
  ptr1 = head->next;                  //put our iterator back to head to restart
  do
  {
    int x1, x2;
    x1 = atoi(ptr1->year);
    x2 = atoi(ptr1->next->year);
    //if (ptr1->year != ptr1->next->year)
    if (x1 != x2)
    { int a;
      float b;
      a = atoi(ptr1->year);
      b = atof(ptr1->rating);
    }
    ptr1 = ptr1->next;
  }while(ptr1->next !=NULL);
  int a;
  float b;
    a = atoi(ptr1->year);
    b = atof(ptr1->rating);
}
//processing the read file DONE
struct movie *processFile(char *filePath)
{
  FILE *movieFile = fopen(filePath, "r");
  char *currLine = NULL;
  size_t len = 0;
  ssize_t nread;
  char *token;
  int count= -1;

  struct movie *head = NULL;
  struct movie *tail = NULL;

  while ((nread = getline(&currLine, &len, movieFile)) != -1)
  {
    struct movie *newNode = createMovie(currLine);

    if (head == NULL)
    {
      head = newNode;
      tail = newNode;
      count++;
    }
    else
    {
      tail->next = newNode;
      tail = newNode;
      count++;
    }
  }
  printf("Processed file %s and parsed data for %d movies\n", filePath, count);
  printf("\n");
  free(currLine);
  fclose(movieFile);
  sorting(head);                                                                //pass head to sorting to sort by year
  return head;
}
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

//this function will process our file
void processing(struct movie *head, char f[])
{
  time_t t;
  srand((unsigned) time(&t));
  int random = rand() % 99999;                                                       //generate a random number between 1-99999 work cited: https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
  char rando[16];
  sprintf(rando, "%d", random);
  char pathname[40] = "wongmat.movies.";
  strcat(pathname, rando);
  //TESTING RANDO
  //printf("random number: %d\n", r);
  printf("Now processing the chosen file named %s\n\n", f);
  //1. create a new directory and print a message with the name of the directory that has been made
  printf("Created directory with name: %s\n", pathname);
  //int mkdir(char*pathname, mode_t mode);                                  //from lecture: https://oregonstate.instructure.com/courses/1798831/pages/exploration-directories?module_item_id=20163866
  //work cited for below: https://www.geeksforgeeks.org/create-directoryfolder-cc-program/
  int check;
  //2. the permissions of the directory must be set to rwxr-x--
  check = mkdir(pathname, 0750);                                                //work cited for permission: http://www.unixmantra.com/2013/04/unix-linux-file-permissions.html
  if (check != 1)
  {
    printf("TEST Directory created\n");
    //open said directory
    struct dirent *pDirent;
    char strPath[100] = "./";
    strcat(strPath, pathname);
    printf("test pathway: %s\n", strPath);
    DIR *pDir = opendir(strPath);
    head = head->next;
    if (pDir == NULL)
    {
      printf("cannot open this directory for some reason\n");
      exit(1);
    }
    //printList(head);
    while ((pDirent = readdir(pDir)) !=NULL)
    {
      chdir(strPath);                                                           //work cited: //work cited: https://stackoverflow.com/questions/21236508/to-create-all-needed-folders-with-fopen
      //create a loop to create files for every thing in the struct until end of struct
      while(head!=NULL)
      {
        int a, b;
        a = atoi(head->year);
        b = atoi(head->next->year);

        if (a < b)                                                              // if the second comapred year is greater just print one and iterate
        {
          if (head->next->next !=NULL)
          {
            printf("the second compared year is greater so we just create 1 file\n");
            char *year = head->year;                                            //store the filename as the year
            char *newYear = ".txt";
            strcat(year, newYear);
            printf("testing our filename: %s\n", year);                            //testing that our strcat copied correctly
            FILE *fp = fopen(year, "w");                                         //open that file with that year title
            //TEST
            //FILE *fp = fopen("2018.txt", "w");
            fputs(head->title, fp);
            fputs("\n", fp);
            head = head->next;
            fclose(fp);
          }
          else
          {
            //first create the current file
            printf("the last year is single so we just create 2 for current and next file\n");
            char *year = head->year;                                            //store the filename as the year
            char *newYear = ".txt";
            strcat(year, newYear);
            printf("testing our filename: %s\n", year);                            //testing that our strcat copied correctly
            FILE *fp = fopen(year, "a+");                                         //open that file with that year title
            //TEST
            //FILE *fp = fopen("2018.txt", "w");
            fputs(head->title, fp);
            fputs("\n", fp);
            head = head->next;
            printf("now creating last file\n");
            char *y = head->year;
            strcat(y, newYear);
            printf("testing our last filename: %s\n", y);
            FILE *f = fopen(y, "a+");
            fputs(head->title, f);
            fputs("\n", f);
            fclose(f);
            fclose(fp);
          }
        }
        else if (a == b)                                                        //if the years are the same
        {
          if (head->next->next !=NULL)
          {
            printf("the years are the same so keep storing and iterating until not the same\n");
            char *year = head->year;                                            //store the filename as the year
            char *newYear = ".txt";
            strcat(year, newYear);
            printf("testing our filename: %s\n", year);                            //testing that our strcat copied correctly
            FILE *fp = fopen(year, "a+");                                         //open that file with that year title
            //FILE *fp = fopen("2019.txt", "w");
            do
            {
              printf("in my do while loop\n");
              printf(" %s\n, ", head->title);
              fputs(head->title, fp);
              fputs("\n", fp);
              head = head->next;
            }while(strcmp(head->year, head->next->year) == 0);                       //check if the years are equal)
            printf("outside of my do while loop\n");
            printf(" %s\n, ", head->title);
            fputs(head->title, fp);                                               //put the last year before next is not equal
            fputs("\n", fp);
            fclose(fp);
            head = head->next;
          }
          else
          {
            printf("we are on the last items on the list\n");
            char *year = head->year;                                            //store the filename as the year
            char *newYear = ".txt";
            strcat(year, newYear);
            printf("testing our filename: %s\n", year);                            //testing that our strcat copied correctly
            FILE *fp = fopen(year, "a+");                                         //open that file with that year title
            fputs(head->title, fp);
            fputs("\n", fp);
            head = head->next;
            printf("now creating last file\n");
            char *y = head->year;
            strcat(y, newYear);
            printf("testing our last filename: %s\n", y);
            FILE *f = fopen(y, "a+");
            fputs(head->title, f);
            fputs("\n", f);
            fclose(f);
            fclose(fp);
          }
        }
      }
    closedir(pDir);
    }
  }
  else if (check == 1)
  {
    printf("TEST Directory was not created\n");
    exit(1);
  }
  //3. prase the data in the chosen file to find out the movies released in each year

  //5. within the file for a year write the titles for all the movies released in that year one on each line
}

int main()
{
  int userChoice, userOption;                                                   //user storage options
  do
  {                                                                             //our do while loop helps our error validation
    userChoice = menuA();                                                       //set the return value to what the menu prompts for userChoice 1-2
    if (userChoice == 1)                                                        //if the user enters 1 we need to go to secondary menu
    {
      do
      {                                                                         //secondary menu which will prompt user 1-3
        userOption = menuB();
        if (userOption == 1)                                                    //work cited: https://www.geeksforgeeks.org/c-program-find-size-file/
        {
                                                                                //work cited using portion from: https://oregonstate.instructure.com/courses/1798831/pages/exploration-directories?module_item_id=20163866 to open directory
          DIR* currDir = opendir(".");                                          //opens the current directory
          struct dirent *aDir;                                                  //pointer for dirent
          struct stat dirStat;                                                  //delcaration of stat struct for files
          struct stat dirStat2;                                                 //used for 2nd file comparison
          char entryName[256];                                                  //stores our largest filename
          int i = 0;                                                            //for our comparison between files
          FILE *fp;
          while((aDir = readdir(currDir)))                                       //otherwise go through all the entries
          {
            //look only at the prefix student files
            if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX))==0)
            {
              stat(aDir->d_name, &dirStat);                                     //get metadata for current entry
              printf("filename: %s\n", aDir->d_name);                           //prints the file name
                                                                                //work cited: https://stackoverflow.com/questions/29548013/loop-through-a-file-and-print-file-attributes-in-c
              printf("total size, in bytes: %lld\n", dirStat.st_size);          //display the file information for testing
              printf("\n");
              if (i == 0)
              {
                //set the entryName to the current file being looked at because its the ONLY movies prefix csv
                memset(entryName, '\0', sizeof(entryName));
                strcpy(entryName, aDir->d_name);
                stat(aDir->d_name, &dirStat2);                                  //store it in dirStat2
              }
              if (i !=0)
              {
                //that means we have a prefix file with movie already so we need to compare the file size and see which one is greater
                if (dirStat.st_size > dirStat2.st_size)
                {
                  //the new stat is greater than the previous so we need it to take over the stat2 otherwise stat2 remains
                  memset(entryName, '\0', sizeof(entryName));
                  strcpy(entryName, aDir->d_name);
                  stat(aDir->d_name, &dirStat2);
                }
                if (dirStat.st_size < dirStat2.st_size)
                {
                  continue; //it means the file sizes are the greater before
                }
              }
              i++;                                                              //means we looked through 1 file with prefix movie
            }
          }
          closedir(currDir);
          printf("The largest file with prefix: \"%s\" in the current directory is: %s\n", PREFIX, entryName);

          struct movie *head = processFile(entryName);

          printList(head);

          processing(head, entryName);
          printf("\n");
          deleteList(&head);
          break;
        }
        if (userOption == 2)
        {
                                                                                //work cited using portion from: https://oregonstate.instructure.com/courses/1798831/pages/exploration-directories?module_item_id=20163866 to open directory
          DIR* currDir = opendir(".");                                          //opens the current directory
          struct dirent *aDir;                                                  //pointer for dirent
          struct stat dirStat;                                                  //delcaration of stat struct for files
          struct stat dirStat2;                                                 //used for 2nd file comparison
          char entryName[256];                                                  //stores our largest filename
          int i = 0;                                                            //for our comparison between files
          FILE *fp;
          while((aDir = readdir(currDir)))                                       //otherwise go through all the entries
          {
            //look only at the prefix student files
            if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX))==0)
            {
              stat(aDir->d_name, &dirStat);                                     //get metadata for current entry
              printf("filename: %s\n", aDir->d_name);                           //prints the file name
                                                                                //work cited: https://stackoverflow.com/questions/29548013/loop-through-a-file-and-print-file-attributes-in-c
              printf("total size, in bytes: %lld\n", dirStat.st_size);          //display the file information for testing
              printf("\n");
              if (i == 0)
              {
                //set the entryName to the current file being looked at because its the ONLY movies prefix csv
                memset(entryName, '\0', sizeof(entryName));
                strcpy(entryName, aDir->d_name);
                stat(aDir->d_name, &dirStat2);                                  //store it in dirStat2
              }
              if (i !=0)
              {
                //that means we have a prefix file with movie already so we need to compare the file size and see which one is greater
                if (dirStat.st_size < dirStat2.st_size)
                {
                  //the new stat is greater than the previous so we need it to take over the stat2 otherwise stat2 remains
                  memset(entryName, '\0', sizeof(entryName));
                  strcpy(entryName, aDir->d_name);
                  stat(aDir->d_name, &dirStat2);
                }
                if (dirStat.st_size > dirStat2.st_size)
                {
                continue; //it means the file sizes are the greater before
                }
              }
            i++;                                                              //means we looked through 1 file with prefix movie
            }
          }
          closedir(currDir);
          printf("The smallest file with prefix: \"%s\" in the current directory is: %s\n", PREFIX, entryName);
          struct movie *head = processFile(entryName);
          printList(head);
          processing(head, entryName);
          printf("\n");
          deleteList(&head);
          break;
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
          do
          {
            printf("Please enter a fileName to open (example: movies): ");          //user prompt
            scanf("%s", fileName);                                                  //looks for the file and stores it in our fileName
            strcat(fileName, ".csv");                                               //append with csv
            fp = fopen(fileName, "r");
          } while(fp == NULL);
        }
        if (fp !=NULL)
        {
          //send to processing
          struct movie *head = processFile(fileName);
          printList(head);
          processing(head, fileName);
          deleteList(&head);
        }
        fclose(fp);                                                             //close our file
        break;
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
