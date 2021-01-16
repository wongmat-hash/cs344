//Class CS344 Winter 2021
//Matthew Wong 01/15/21


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for movies
struct movie
{
  char title[100];                                                              //title
  int year;                                                                     //year
  char language[100];                                                           //language
  double rating;                                                                //rating value
  struct movie *next;                                                           //pointer to next movie
};

//function to print our linked list for TESTING
void printList(struct movie *m)
{
  while(m !=NULL)
  {
    printf(" %s: ", m->title);
    printf(" %d: ", m->year);
    printf(" %s: ", m->language);
    printf(" %f: ", m->rating);
    printf("\n");
    m = m->next;
  }
}
int main()
{

  struct movie* head = malloc(sizeof(struct movie));
  struct movie* second = malloc(sizeof(struct movie));
  struct movie* third = malloc(sizeof(struct movie));

  strcpy(head->title, "The Incredible Hulk");                                   //need strcpy
  head->year = 2008;
  strcpy(head->language, "[English, Portuguese, Spanish]");                     //need strcpy
  head->rating = 6.8;
  head->next = second;

  strcpy(second->title, "Sherlock Holmes");                                     //need strcpy
  second->year = 2009;
  strcpy(second->language, "[English, French]");                                //need strcpy
  second->rating = 7.6;
  second->next = third;

  strcpy(third->title, "Iron Man");                                             //need strcpy
  third->year = 2008;
  strcpy(third->language, "[English, Persian, Urdu, Arabic, Hungarian]");       //need strcpy
  third->rating = 7.9;
  third->next = NULL;

  printList(head);
return 0;
}
