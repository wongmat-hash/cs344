//Class CS344 Winter 2021
//Matthew Wong 01/15/21


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie
{
  char title[1000];
  int year;
  char lang[1000];
  float rating;
  struct movie *next;
};
