//Class CS344 Winter 2021
//Matthew Wong 01/15/21


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this is our struct for our student. It will house a bunch of data and point to another student as a linked list
struct movie
{
  char title[1000];                                                             //title stored in an array
  int year;                                                                     //year stored as int
  char lang[1000];                                                              //language stored as an array
  float rating;                                                                 //rating stored as a float
  struct movie *next;                                                           //next movie pointer
} *head = NULL;

//struct movie *head = NULL;
//struct movie *current = NULL;

//first we need to parse our csv and store all the data in new movie structs that point to next
//struct movie *addMovie(struct movie *head, char *t, int *y, char *l, float *r)  //we pass in all our pointers for each item in our struct
//{
  //struct movie *newMovie = (struct movie*)malloc(sizeof(struct movie));         //now we allocate the memory required for our new movie struct
  //newMovie->title = *t;                                                         //set our newMoview Title
  //newMovie->year = *y;                                                          //set our new movie year
  //newMovie->lang = *l;                                                          //set our new movie lang
  //newMovie->rating = *r;                                                        //set our new movie rating

  //newMovie->next = head;                                                        //point the next to old first node if its only it works also
  //head = newMovie;                                                              //the head should point to newMovie
//}

//compare function to sort our linked list how we want it to display

//function for searching for movies in a specific year

//function to show movies released with certain language

//function to show highest rated movies of year

//main
int main(int argc, char** argv)
{
  FILE *fp = fopen("test.csv", "r");
  head = NULL;
  if (fp == NULL)
  {
    printf("\n error opening file");
    return -1;
  }
  while((line=fgets(buf,sizeof(buf),fp))!=NULL)
  {
    word = strtok(line,",");
    head = add(head, word);

    while(word!=NULL)
    {
      word=strtok(NULL,",");
      head=add(head,word);
    }
    display(head);
    printf("\n");
  }
  return 0;
}