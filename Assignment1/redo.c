//Class CS344 Winter 2021
//Matthew Wong 01/15/21


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for movies DONE
struct movie
{
  char title[100];                                                              //title
  int year;                                                                     //year
  char language[100];                                                           //language
  float rating;                                                                 //rating value
  struct movie *next;                                                           //pointer to next movie
};

//function to print our linked list for TESTING
void printList(struct movie *m)
{
  while(m !=NULL)
  {
    printf("Title: %s ,", m->title);
    printf("Year: %d ,", m->year);
    printf("Language(s): %s ,", m->language);
    printf("Rating: %.1f ", m->rating);
    printf("\n");
    m = m->next;
  }
}

//function to copy our linked list to create a temp list
void copyList(struct movie **head)                                              //takes the head in then will make a copy to sort
{
  //TEST TO SHOW THAT FUNCTION IS WORKING
  printf("TEST: Now inside byYear Function\n");
  //now create a temp which will be a copy of the entire list
  if(head==NULL)                                                                //checks our function to see if its NULL or not
  {
    printf("ERROR list passed into byYear is NULL\n");
    return NULL;                                                                //not possible so return to main
  }
  else
  {
    struct Movie *tempMovie = (struct Movie*)malloc(sizeof(struct movie));      //declare our temp list
    tempMovie->title = head->title;                                             //pass all our data now 
    tempMovie->year = head->year;
    tempMovie->language = head->language;
    tempMovie->rating = head->rating;
    tempMovie->next = copyList(head->next);
  }

int main()
{
  //build out file reading which will create its own linked list


  //allocate all the the memories
  struct movie* head = malloc(sizeof(struct movie));
  struct movie* second = malloc(sizeof(struct movie));
  struct movie* third = malloc(sizeof(struct movie));
  struct movie* fourth = malloc(sizeof(struct movie));
  struct movie* fifth = malloc(sizeof(struct movie));
  struct movie* sixth = malloc(sizeof(struct movie));
  struct movie* seventh = malloc(sizeof(struct movie));
  struct movie* eigth = malloc(sizeof(struct movie));
  struct movie* ningth = malloc(sizeof(struct movie));
  struct movie* tenth = malloc(sizeof(struct movie));
  struct movie* eleventh = malloc(sizeof(struct movie));
  struct movie* twelveth = malloc(sizeof(struct movie));
  struct movie* thirteenth = malloc(sizeof(struct movie));
  struct movie* fourteenth = malloc(sizeof(struct movie));
  struct movie* fifteenth = malloc(sizeof(struct movie));
  struct movie* sixteenth = malloc(sizeof(struct movie));
  struct movie* seventeenth = malloc(sizeof(struct movie));
  struct movie* eighteenth = malloc(sizeof(struct movie));
  struct movie* nineteenth = malloc(sizeof(struct movie));
  struct movie* twentieth = malloc(sizeof(struct movie));
  struct movie* twentyone = malloc(sizeof(struct movie));
  struct movie* twentytwo = malloc(sizeof(struct movie));
  struct movie* twentythree = malloc(sizeof(struct movie));
  struct movie* twentyfour = malloc(sizeof(struct movie));

  strcpy(head->title, "The Incredible Hulk");                                   //need strcpy
  head->year = 2008;
  strcpy(head->language, "[English;Portuguese;Spanish]");                       //need strcpy
  head->rating = 6.8;
  head->next = second;

  strcpy(second->title, "Sherlock Holmes");                                     //need strcpy
  second->year = 2009;
  strcpy(second->language, "[English;French]");                                 //need strcpy
  second->rating = 7.6;
  second->next = third;

  strcpy(third->title, "Iron Man");                                             //need strcpy
  third->year = 2008;
  strcpy(third->language, "[English;Persian;Urdu;Arabic;Hungarian]");           //need strcpy
  third->rating = 7.9;
  third->next = fourth;

  strcpy(fourth->title, "Iron Man 2");                                           //need strcpy
  fourth->year = 2010;
  strcpy(third->language, "[English;French;Russian]");                          //need strcpy
  fourth->rating = 7;
  fourth->next = fifth;

  strcpy(fifth->title, "Iron Man 3");                                           //need strcpy
  fifth->year = 2013;
  strcpy(third->language, "[English]");                                         //need strcpy
  fifth->rating = 7.2;
  fifth->next = sixth;

  strcpy(sixth->title, "Thor: Ragnarok,2017");                                  //need strcpy
  sixth->year = 2013;
  strcpy(third->language, "[English]");                                         //need strcpy
  sixth->rating = 7.9;
  sixth->next = seventh;

  strcpy(seventh->title, "The Avengers");                                         //need strcpy
  seventh->year = 2012;
  strcpy(third->language, "[English;Russian;Hindi]");                           //need strcpy
  seventh->rating = 8.1;
  seventh->next = eigth;

  strcpy(eigth->title, "Doctor Strange");                                       //need strcpy
  eigth->year = 2016;
  strcpy(third->language, "[English]");                                         //need strcpy
  eigth->rating = 7.5;
  eigth->next = ningth;

  strcpy(ningth->title, "Avengers: Infinity War");                               //need strcpy
  ningth->year = 2018;
  strcpy(third->language, "[English]");                                         //need strcpy
  ningth->rating = 8.5;
  ningth->next = tenth;

  strcpy(tenth->title, "Avengers: Age of Ultron");                              //need strcpy
  tenth->year = 2015;
  strcpy(third->language, "[English;Korean]");                                  //need strcpy
  tenth->rating = 7.4;
  tenth->next = eleventh;

  strcpy(eleventh->title, "Thor");                                                 //need strcpy
  eleventh->year = 2011;
  strcpy(third->language, "[English]");                                         //need strcpy
  eleventh->rating = 7.4;
  eleventh->next = twelveth;

  strcpy(twelveth->title, "Thor: The Dark World");                                 //need strcpy
  twelveth->year = 2013;
  strcpy(third->language, "[English]");                                         //need strcpy
  twelveth->rating = 7;
  twelveth->next = thirteenth;

  strcpy(thirteenth->title, "Spider-Man: Homecoming");                                 //need strcpy
  thirteenth->year = 2017;
  strcpy(third->language, "[English;Spanish]");                                         //need strcpy
  thirteenth->rating = 7.5;
  thirteenth->next = fourteenth;

  strcpy(fourteenth->title, "Captain America: The First Avenger");                                 //need strcpy
  fourteenth->year = 2011;
  strcpy(third->language, "[English;Norwegian;French]");                                         //need strcpy
  fourteenth->rating = 6.9;
  fourteenth->next = fifteenth;

  strcpy(fifteenth->title, "Captain America: Civil War");                                 //need strcpy
  fifteenth->year = 2016;
  strcpy(third->language, "[English;German;Russian;Romanian;Hindi]");                                         //need strcpy
  fifteenth->rating = 7.8;
  fifteenth->next = sixteenth;

  strcpy(sixteenth->title, "Ant-Man");                                 //need strcpy
  sixteenth->year = 2015;
  strcpy(third->language, "[English]");                                         //need strcpy
  sixteenth->rating = 7.3;
  sixteenth->next = seventeenth;

  strcpy(seventeenth->title, "Captain America: The Winter Soldier");                                 //need strcpy
  seventeenth->year = 2014;
  strcpy(third->language, "[English;French]");                                         //need strcpy
  seventeenth->rating = 7.8;
  seventeenth->next = eighteenth;

  strcpy(eighteenth->title, "Mary Queen of Scots");                                 //need strcpy
  eighteenth->year = 2018;
  strcpy(third->language, "[English]");                                         //need strcpy
  eighteenth->rating = 6.9;
  eighteenth->next = nineteenth;

  strcpy(nineteenth->title, "Revolting Rhymes Part One");                                 //need strcpy
  nineteenth->year = 2016;
  strcpy(third->language, "[English;Swedish]");                                         //need strcpy
  nineteenth->rating = 7.7;
  nineteenth->next = twentieth;

  strcpy(twentieth->title, "The Glass Castle");                                 //need strcpy
  twentieth->year = 2017;
  strcpy(third->language, "[English]");                                         //need strcpy
  twentieth->rating = 7.2;
  twentieth->next = twentyone;

  strcpy(twentyone->title, "Free Fire");                                 //need strcpy
  twentyone->year = 2016;
  strcpy(third->language, "[English]");                                         //need strcpy
  twentyone->rating = 6.4;
  twentyone->next = twentytwo;

  strcpy(twentytwo->title, "Right on Track");                                 //need strcpy
  twentytwo->year = 2003;
  strcpy(third->language, "[English]");                                         //need strcpy
  twentytwo->rating = 6.6;
  twentytwo->next = twentythree;

  strcpy(twentythree->title, "Rise of the Guardians");                                 //need strcpy
  twentythree->year = 2012;
  strcpy(third->language, "[English;French]");                                         //need strcpy
  twentythree->rating = 7.3;
  twentythree->next = twentyfour;

  strcpy(twentyfour->title, "Anna Karenina");                                 //need strcpy
  twentyfour->year = 2012;
  strcpy(third->language, "[English]");                                         //need strcpy
  twentyfour->rating = 8.1;
  twentyfour->next = NULL;


  printList(head);
  printf("TEST COMPLETE\n");
  printf("\n");

  //build out menu options for user to display
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
        byYear(&head);                                                 //passes it to the function we created above
        break;
      }
      case 2:
      {
        //show_highest_rated(m, count);                                         //passes and shows the user the highest rated sorted movie list
        break;
      }
      case 3:
      {
        printf("Enter the language for which you want to see movies: ");         //asks the user for what language input
        scanf("%s", userLanguage);                                              //scans for user input on the language
        //show_movies_bylang(m, count, lang);                                   //passes it to the function we created above
        break;
      }
      case 4:
      {
        exit(0);
      }
    }
  }

return 0;
}
