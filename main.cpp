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
int comparator(const void *a, const void *b)                                    //we take in two voids as pointers
{
  if (((struct movie *)a)-> year == ((struct movie*)b)->year)                   //if statement to cheeck if the movie *a pointer this year is equal to b
    {
      return ((struct movie)*a)->rating < ((struct movie)*b)->rating;           //if it is we return that the ratings
    }
  else
  {
    return ((struct movie *)a)->year < ((struct movie *)b)-> year;              //otherwise return that the year is less so it compares
  }
}

//this function will display movies of specific year entered by the user
void show_movies_byyear(struct movie m[], int n, int year)                      //this function takes in the movie struct, and int representing array length and year we are looking for
{
  int flag = 0;                                                                 //we create a flag which will be our counter and set it to 0
  for (int i = 0; i < n; i++)                                                   //using a for loop we now parse through the array and look for an array that matches our user input
  {
    if (m[i].year == year)                                                      //if array index i (looping through) the year matches the year the user input
    {
      printf("%\n", m[i].title);                                                //we are going to print out the title
      flag = 1;                                                                 //now we set our flag to 1 (how many we found)
    }
  }
  //if we cannot find using our search then we need to tell the user that
  if (flag == 0)                                                                //if we parsed the entire list and our flag was 0 means we found nothing
  {
    printf("\nNo data about movies released in year %d\n\n".year);              //print out that we found nothing in relation to it
    print("\n");                                                                //new line for formatting
  }
}
