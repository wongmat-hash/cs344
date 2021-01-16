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
  if (((struct movie*)a)-> year == ((struct movie*)b)->year)                   //if statement to cheeck if the movie *a pointer this year is equal to b
    {
      return ((struct movie*)a)->rating < ((struct movie*)b)->rating;           //if it is we return that the ratings
    }
  else
  {
    return ((struct movie*)a)->year < ((struct movie*)b)-> year;                //otherwise return that the year is less so it compares
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
      printf("%s\n", m[i].title);                                                //we are going to print out the title
      flag = 1;                                                                 //now we set our flag to 1 (how many we found)
    }
  }
  //if we cannot find using our search then we need to tell the user that
  if (flag == 0)                                                                //if we parsed the entire list and our flag was 0 means we found nothing
  {
    printf("\nNo data about movies released in year %d\n\n", year);             //print out that we found nothing in relation to it
    printf("\n");                                                               //new line for formatting
  }
}

//now we build out a function that can compare the langauges and find the one we are seeking
void show_movies_bylang(struct movie m[], int n, char l[])                      //we take in this function the struct for movie, an int and char array
{
  int flag = 0;                                                                 //create a flag int and set it to 0 for our counter
  for (int i = 0; i < n; i++)                                                   //now we parse the entire length of the user input
  {
    if(strcmp(m[i].lang, l) == 0)                                               //using the for loop as an index, we then use strcmp and go through the string and if its == 0 we have a match
    {
      printf("%d %s\n", m[i].year, m[i].title);                                 //print the year and title by the index for loop
      flag = 1;                                                                 //set the flag boolean to 1 so we know we found one and skips the conditional
    }
  }

                                                                                //if we cannot find a language matching what the user input
  if (flag == 0)                                                                //if our conditional was never flipped
  {
    printf("No data about movies released in language %s\n\n", l);              //print the string
    printf("\n");
  }
}

//now we need a function to find the highest rated movie for a given year
void show_highest_rated(struct movie m[], int n)
{
                                                                                //first display by year and if the years match then secondary sort by ratings
  qsort(m, n+1, sizeof(m[0]), comparator);                                      //pass to our comparator function above
  int curr_year = -1;                                                           //create an int that is the curr year and set it to negative 1
  for (int i = 0; i < n-1; i++)                                                 //use a for loop to interate through our movie
  {
    if (m[i].year != curr_year)                                                 //if the movie index year is not equal to the current year (-1)
    {
      printf("%d %2f %s\n", m[i].year, m[i].rating, m[i].title);                //then print out the year rating and title
    }
    curr_year = m[i].year;                                                      //now we set the current year to the year we just looked at and we continue on the loop
  }
  printf("\n");                                                                 //print a new line
}

//main function to run the user input validation as well as pass everything to the functions we made above
int main(int argc, char** argv)                                                 //how command line arguments are passed to to main in C
{
                                                                                //we want to take our input from a CSV file so we need to check for file name
  if (argc == 1)                                                                //user will specify the file name from terminal hence the argc
  {
    printf("Movie file not specified!\n");                                      //properly display the output if the file is not found
  }
  else
  {
    struct movie m[1000];                                                         //create a struct movie array arbitrary amount to store

    char *filename = argv[1];                                                   //take the commandline filename

    FILE* file = fopen(filename, "r");                                          //open the file with our program now that its specified and found

    if (file == NULL)                                                           //error validation for file opening
    {
      printf("There is some error opening file %s", filename);                  //error validation message
      return 0;                                                                 //return 0 to end the program
    }
    char line[4096];

    int count = 0;

    while(fgets(line, sizeof(line), file))                                      //basically as we continue to parse the file line by line until end of filename
    {
      if(count >= 1)                                                            //this is our counter since it starts at 0 its going to run whats below
      {
        char* tp = strdup(line);                                                //the pointer tp = the line
        char* tok = strtok(tp, "\n");                                           //the pointer tok
        int k = 1;
        while(tok != NULL)
        {
          if (k==1) strcpy(m[count-1].title, tok);
          if (k==2) m[count-1].year = atoi(tok);
          if (k==3) strcpy(m[count-1].lang, tok);
          if (k==4) m[count-1].rating = atof(tok);
          tok = strtok(NULL, ",\n");
          k++;
        }
        free(tp);
      }
      count++;
    }
                                                                                //show the user in terminal that the file has been processed correctly
    printf("Processed file %s and parsed data for %d movies\n", filename, count);

    int op;                                                                     //storage int for user input
    int year;                                                                   //storage year for user input
    char lang[1000];                                                            //storage lang for user input

    while(1)                                                                    //use a loop to continue asking user for inputs
    {                                                                           //our menu system
      printf("1. Show movies released in the specified year\n");
      printf("2. Show highest rated movie for each year\n");
      printf("3. Show the title and year of release of all movies in a specific language\n");
      printf("4. Exit from the program\n");
      printf("Enter a choice from 1 to 4:\n");

      scanf("%d", &op);                                                         //scan for user input and store into op

      switch(op)                                                                //use a switch to display the secondary menu (submenu after first)
      {
        case 1:
        {
          printf("Enter the year for which you want to see movies: ");          //asks user for what year they which to look up
          scanf("%d", &year);                                                   //scans for user input on year
          show_movies_byyear(m, count, year);                                   //passes it to the function we created above
          break;
        }
        case 2:
        {
          show_highest_rated(m, count);                                         //passes and shows the user the highest rated sorted movie list
          break;
        }
        case 3:
        {
          printf("Enter the language for which you want to see movies:");       //asks the user for what language input
          scanf("%s", lang);                                                    //scans for user input on the language
          show_movies_bylang(m, count, lang);                                   //passes it to the function we created above
          break;
        }
        case 4:
        {
          exit(0);
        }
      }
    }
  }
  return 0;
}
