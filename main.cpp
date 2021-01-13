#include <stdio.h>                    //standard input/output
#include <stdlib.h>                   //standard library
#include <string.h>                   //this allows us to use strings in c

//we first need a struct defined to store the movies in for our linked list
//according to the csv there needs to be a title/ year/ languages/ rating value
struct movie
{
  char title[1000];         //we can store the title as a char
  float rating;             //we can store the rating as a float
  int year;                 //we can store the year as an int
  char lang[1000];          //we can store the language as a char
};

//we need a sort function to compare and sort out our movies released by year
//first build out a comparison function to check for the year
int comparator(const void *a, const void *b)  //we take in two voids as pointers
{
  if (((struct movie *)a)-> year == ((struct movie*)b)->year)   //if statement to cheeck if the movie *a pointer this year is equal to b
    {
      return ((struct movie)*a)->rating < ((struct movie)*b)->rating; //if it is we return that the ratings
    }
  else
  {
    return ((struct movie *)a)->year < ((struct movie *)b)-> year;  //otherwise return that the year is less so it compares
  }
}
