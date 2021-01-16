//Class CS344 Winter 2021
//Matthew Wong 01/15/21


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie
{
  char *title;                                                                  //title
  int year;                                                                     //year
  char *language;                                                               //language
  float *rating;                                                                //rating value
  struct mMovie *next;                                                          //pointer to next movie
};

//function to create movies
movie *createMovie(char *t, int y, char *l, float *r)
{
  movie *m = malloc(sizeof(struct movie));                                      //allocate memory for the pointers themselves in the struct
  m->title = strdup(t);                                                         //set the title that is passed in by duplicating a string of char
  m->year = y;                                                                  //since this is an int we can simply set it
  m->language = strdup(l);                                                      //set the language by copying the string using strdup
  m->rating = r;                                                                //since this is a float we can simply copy it
  return m;                                                                     //return the newly created movie struct with correct values
}

int main()
{
  //create a linked list of movies
  //populate them and then
}
