//Class CS344 Winter 2021
//Matthew Wong 01/15/21


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

//struct for movies DONE
struct movie
{
  char *title;//[100];                                                              //title
  char *year;                                                                     //year
  char *language;//[100];                                                           //language
  char *rating;                                                                 //rating value
  struct movie *next;                                                           //pointer to next movie
};


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
  //tit = calloc(strlen(token) + 1, sizeof(char));
  //strcpy(currMovie->title, tit);
  //copy the year
  token = strtok_r(NULL, ",", &saveptr);
  currMovie->year = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currMovie->year, token);
  //yea = calloc(strlen(token) +1, sizeof(char));
  //printf("YEA TEST: %s\n", yea);
  //currMovie->year = atoi(yea);
  //copy the language
  token = strtok_r(NULL, ",", &saveptr);
  currMovie->language = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currMovie->language, token);
  //lang = calloc(strlen(token) + 1, sizeof(char));
  //strcpy(currMovie->language, lang);
  //copy the rating
  token = strtok_r(NULL, "\n", &saveptr);
  currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currMovie->rating, token);
  //rat = calloc(strlen(token) + 1, sizeof(char));
  //printf("RAT TEST: %s\n", rat);
  //currMovie->rating = atof(rat);

  currMovie->next = NULL;

  return currMovie;
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
  return head;
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

//function to copy our linked list to create a temp list DONE
struct movie* copyList(struct movie *head)                                      //takes the head in then will make a copy to sort
{
  //TEST TO SHOW THAT FUNCTION IS WORKING
  //printf("TEST: Now inside COPYLIST Function\n");
  //now create a temp which will be a copy of the entire list
  if(head==NULL)                                                                //checks our function to see if its NULL or not
  {
    printf("ERROR list passed into COPYLIST is NULL\n");
    return NULL;                                                                //not possible so return to main
  }
  else
  {
    //printf("attempting to copy\n");
    struct movie *newHead = NULL;                                               //declare our temp list
    struct movie **newIt = &newHead;
    while(head!=NULL)
    {
      *newIt = malloc(sizeof(struct movie));
      memcpy(*newIt, head, sizeof(struct movie));
      head = head->next;
      newIt = &((*newIt)->next);
    }                                                                          //recursivly now continue and copy the rest of the list until the end
    return newHead;                                                           //returns the temp movie list now to whatever function needs it
  }
}

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

//function that will sort by the year by making a copy passing it to copyList DONE
void byYear(struct movie *head, int y)
{
  //convert our y into a string
  char *year;
  sprintf(year, "%d", y);
  //printf("TESTING YEAR %s: \n", year);
  int flag = 0;                                                                 //this will trigger our warning msg if 0 still
  //TESTING
  //printf("TEST: NOW I AM IN byYear FUNCTION \n");
  struct movie* dupMovie = (struct movie*)malloc(sizeof(struct movie));
  //printf("DUPLICATE LIST\n");
  dupMovie = copyList(head);                                      //first pass the original list to the copy list and return a temp list that we can use to sort
  //TESTING print out our duplicate copied list
  //printList(dupMovie);
  //use the year to find matching movies and print those
  while(dupMovie!=NULL)                                                         //while we are not at the end of the linked list
  {
    //printf("inside our while function\n");
    //int x = atoi(dupMovie->year);
    //if (x == y)                                                    //if we find a value equal to the year we entered
    if (strcmp(dupMovie->year, year)==0)
    {
      printf("%s \n", dupMovie->title);                                         //print the title of the movie
      flag = 1;
      dupMovie = dupMovie->next;                                                //iterate to the next set
    }
    else                                                                        //else just skip over non needed
    {
      dupMovie = dupMovie->next;                                                //iterate to next
    }
  }
  if (flag == 0)                                                                //if we parsed the entire list and our flag was 0 means we found nothing
  {
    printf("\nNo data about movies released in year %d\n", y);                  //print out that we found nothing in relation to it
  }

  deleteList(&dupMovie);                                                        //delete the temp list
  printf("\n");                                                                 //line for formatting
}

//function to swap our movie node data
void swapper(struct movie *a, struct movie *b)
{
  //declare a temp node
  struct movie *tempNode = NULL;
  struct movie **tempTemp = &tempNode;
  //copy contents of a to tempNode
  *tempTemp = malloc(sizeof(struct movie));
  memcpy(*tempTemp, a, sizeof(struct movie));

  printf("Inside swapper function\n");
    ///char tempTitle[100];
    ///strcpy(tempTitle, a->title);
  //int tempYear = atoi(a->year);
    ///char tempYear[100];
    ///strcpy(tempYear, a->year);
    ///char tempLanguage[100];
    ///strcpy(tempLanguage, a->language);
  //float tempRating = atof(a->rating);
    ///char tempRating[100];
    ///strcpy(tempRating, a->rating);
  //now set a to b and b to temp

  //now use memcpy to copy b > a
  memcpy(a, b, sizeof(struct movie));
  //now memcpy temp > b
  memcpy(b, *tempTemp, (sizeof(struct movie)));

    ///strcpy(a->title, b->title);
    ///strcpy(a->year, b->year);
    ///strcpy(a->language, b->language);
    ///strcpy(a->rating, b->rating);

    ///strcpy(b->title, tempTitle);
  //sprintf(b->year, "%d", tempYear);
    ///strcpy(b->year, tempYear);
  //b->year = tempYear;
    ///strcpy(b->language, tempLanguage);
  //sprintf(b->rating, "%f", tempRating);
    ///strcpy(b->rating, tempRating);
  //b->rating = tempRating;
}

//WORKING HERE 01/16 AM
//function to sort by year then rating
void sorting(struct movie *head)
{
  printf("inside sorting function now\n");
  //run a simple bubble sort to sort out by year first
  int swapped, i;                                                               //storage variable
  struct movie *ptr1;
  struct movie *lptr = NULL;

  //check to see if our list is empty
  if (head == NULL)
  {
    return;
  }
  do
  {
    //printf("here is our list: \n");
    //printList(head);
    swapped = 0;                                                                //initiate our counter for swap
    ptr1 = head;                                                                //set our pointer to the head list
    while (ptr1->next != lptr)                                                  //while our crwaler is not equal to null for next
    {
      //skip the first line since its our title and random fields
      ptr1 = ptr1->next;              //skip
      int p1, p2;
      p1 = atoi(ptr1->year);
      p2 = atoi(ptr1->next->year);
      //if (ptr1->year > ptr1->next->year)                                        //if the crawler year is greater than the next value
      printf("this is our p1: %d\n", p1);
      printf("this is our p2: %d\n", p2);
      if(p1 > p2)
      {
        printf("inside our first conditional\n");
        swapper(ptr1, ptr1->next);                                              //swap them
        swapped = 1;                                                            //set our counter
      }
      //else if (ptr1->year == ptr1->next->year)                                  //if the years are the same we need to check the rating and if the ratings are out of order swap
      else if (p1 == p2)
      {
        float f1, f2;
        f1 = atof(ptr1->rating);
        f2 = atof(ptr1->next->rating);
        //if (ptr1->rating > ptr1->next->rating)
        if (f1 > f2)
        {
          swapper(ptr1,ptr1->next);
          swapped = 1;
        }
      }
      ptr1=ptr1->next;                                                          //set the next value so crwaler keeps going
    }
    lptr = ptr1;                                                                //move the next value to the crawler
  }
  while(swapped);
  //TESTING PRINTING OUT NEW LIST
  //printf("PRINTING OUR SORTED LIST BY YEAR->RATING\n");
  //printList(head);
  //printf("\n");
  ptr1 = head;
  do
  {
    int x1, x2;
    x1 = atoi(ptr1->year);
    x2 = atoi(ptr1->next->year);
    //if (ptr1->year != ptr1->next->year)
    if (x1 != x2)
    {
      printf("%s ", ptr1->year);
      printf("%s ", ptr1->rating);
      printf("%s ", ptr1->title);
      printf("\n");
    }
    ptr1 = ptr1->next;

  }while(ptr1->next !=NULL);
}

//WORKING HERE 01/16 AM DONE 1/17/21
//function to get the highest rated by year
void byHighestRated (struct movie *head)
{
  printf("inside highest rated function\n");
  struct movie *dupMovie = copyList(head);                                      //make a copy of our linked list and sort it
  //sort by years then ratings
  printf("going into sorting function\n");
  sorting(dupMovie);
  //test to make sure its been sorted correctly
  //printf("PRINTING DEMO:\n");
  //printList(dupMovie);
  deleteList(&dupMovie);                                                        //free up our memory

}

void byLang(struct movie *head, char language[])
{
  int flag;                                                                     //our boolean
  //make a temp for our list again
  struct movie* dupMovie = copyList(head);
  while(dupMovie!=NULL)
  {
    if (strcmp(dupMovie->language, language)==0)
    {
      printf("%s ", dupMovie->year);
      printf("%s ", dupMovie->title);
      printf("\n");
      flag = 1;
      dupMovie = dupMovie->next;
    }
    else
    {
      dupMovie = dupMovie->next;
    }
  }
  if (flag == 0)
  {
    printf("\nNo data about movies released in %s\n", language);
  }

}
int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    printf("Movie file not specified!\n");
    return EXIT_FAILURE;
  }

  struct movie *head = processFile(argv[1]);

    //printList(head);
    //printf("TEST COMPLETE\n");
    //printf("\n");


    //build out menu options for user to display DONE
    int userChoice, year;                                                         //user entry variable holder
    char userLanguage[100];                                                       //user entry for language
    while(1)                                                                      //use a loop to continue asking user for inputs
    {                                                                             //our menu system
      printf("1. Show movies released in the specified year\n");
      printf("2. Show highest rated movie for each year\n");
      printf("3. Show the title and year of release of all movies in a specific language\n");
      printf("4. Exit from the program\n");
      printf("Enter a choice from 1 to 4:\n");

      scanf("%d", &userChoice);                                                   //scan for user input and store into op

      switch(userChoice)                                                          //use a switch to display the secondary menu (submenu after first)
      {
        case 1:
        {
          printf("Enter the year for which you want to see movies: ");            //asks user for what year they which to look up
          scanf("%d", &year);                                                     //scans for user input on year
          //test to show user input saved correctly
          //printf("YEAR: %d", year);                                             //TEST PASS
          printf("\n");
          byYear(head, year);                                                     //passes it to the function we created above
          break;
        }
        case 2:
        {
          byHighestRated(head);                                                   //passes and shows the user the highest rated sorted movie list
          break;
        }
        case 3:
        {
          printf("Enter the language for which you want to see movies: ");        //asks the user for what language input
          scanf("%s", userLanguage);                                              //scans for user input on the language
          byLang(head, userLanguage);                                             //passes it to the function we created above
          break;
        }
        case 4:
        {
          exit(0);
        }
      }
    }

    return EXIT_SUCCESS;
return 0;
}
