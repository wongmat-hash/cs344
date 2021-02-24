#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000;

int sizeof_stdin;
//ALL CITED FROM EXAMPLE PROGRAM
char buffer_1[1000];                                                            //buffer 1 shared resource between input thread and line sep thread
int count_1 = 0;                                                                //number of  items in the buffer
int prod_idx_1 = 0;                                                             //index where the input thread will put next item
int con_idx_1 = 0;                                                              //index where line seperator will pick up next item
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;                            //init the mutex for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;                               //init the condition var for buffer 1

//ALL CITED FROM EXAMPLE PROGRAM
char buffer_2[1000];                                                            //buffer 2 shared resource between lines seperator and the ++ thread
int count_2 = 0;                                                                //number of items in our buffer
int prod_idx_2 = 0;                                                             //index where the line sep thread will put the next item
int con_idx_2 = 0;                                                              //index where the ++ thread will pick up the next item
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;                            //init the mutex for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;                               //init the condition var for buffer 2

//ALL CITED FROM EXAMPLE PROGRAM
char buffer_3[1000];                                                            //buffer 3 shared resource between ++  and the output thread
int count_3 = 0;                                                                //number of items in our buffer
int prod_idx_3 = 0;                                                             //index where the ++ thread will put the next item
int con_idx_3 = 0;                                                              //index where the output thread will pick up the next item
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;                            //init the mutex for buffer 3
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;                               //init the condition var for buffer 3

//put buff function for get input (stores the stdin input)                      ALL CITED FROM EXAMPLE PROGRAM
void put_buff_1(char item)                                                      //this function will be called to put items into buffer 1
{
  //printf("in get buff 1\n");
  pthread_mutex_lock(&mutex_1);                                                 //lock the mutex before putting items in the buffer
  buffer_1[prod_idx_1] = item;                                                  //put items in the buffer once locked
  prod_idx_1 = prod_idx_1 + 1;                                                  //increment the index where the next item will be put
  count_1++;                                                                    //increment our count
  pthread_cond_signal(&full_1);                                                 //signal to consumer that buffer is no longer empty
  pthread_mutex_unlock(&mutex_1);                                               //unlock mutex
}

//get input reads stdin by char and stores input into buffer array
//calls put buff 1 to store into array needs to +1 index when add 1
void *get_input(void *args)
{
  sizeof_stdin = sizeof_stdin+1;
  //char currentChar;
  //sizeof_stdin = lengthof(stdin);
  //fgets(&currentChar, sizeof(stdin), stdin);
  //printf("in get input\n");
  while(1)                                                                      //https://www.tutorialspoint.com/c_standard_library/c_function_feof.htm
  {                                                                             //while stdin is working
    char currentChar = fgetc(stdin);                                            //uses fgetc to read from stdin until eof
    if (feof(stdin))                                                            //if its end of file just return NULL
    {
      return NULL;
    }
    sizeof_stdin = sizeof_stdin+1;                                              //increment our global sizeof STDIN counter
    //printf("size of inside getinput: %d\n", sizeof_stdin);
    //printf("get input calling put buff 1\n");
    put_buff_1(currentChar);                                                    //put the char we are looking at into the buffer with function
  }
  return NULL;                                                                  //return NULL
}

//get buff function for line seperator (pulls from buffer 1)
char get_buff_1()                                                               //ALL CITED FROM EXAMPLE PROGRAM
{
  //printf("in get buff 1\n");
  pthread_mutex_lock(&mutex_1);                                                 //lock the mutext before checking if our buffer has any data
  while (count_1 == 0)                                                          //if the buffer is empty wait for the prod to signal that the buffer has data to pull
  {
    pthread_cond_wait(&full_1, &mutex_1);
  }
  char currentChar = buffer_1[con_idx_1];                                       //set the buffer index char to the char declared here
  con_idx_1 = con_idx_1 + 1;                                                    //increment our index which the next item will be picked up
  count_1--;
  pthread_mutex_unlock(&mutex_1);                                               //unlock our mutex
  return currentChar;                                                           //return the item
}

void put_buff_2(char item)                                                      //ALL CITED FROM EXAMPLE PROGRAM
{
  //printf("in get buff 2\n");
  //printf("item in 2: %c\n", item);
  pthread_mutex_lock(&mutex_2);                                                 //lock the mutex before putting item in the buffer
  buffer_2[prod_idx_2] = item;                                                  //put items into the buffer
  prod_idx_2 = prod_idx_2 + 1;                                                  //increment the index where the next item will be put
  count_2++;
  pthread_cond_signal(&full_2);                                                 //signal to the consumer that the buffer is no longer empty
  pthread_mutex_unlock(&mutex_2);                                               //unlock the mutex
}
//line seperator will pull from buff1 array with get buff 1
//needs to adjust buffer 1 to +1 index so it knows to check next element
//put buff 2 will be called and it will be stored into 2nd buffer array
void *lineSeperator(void *args)                                                 //line seperator thread function
{
  char charN, charS, charT, charO, charP, charNew;                              //declare our char that will store the currently looked at
  int storage = sizeof_stdin;
  //test to make sure we set correctly
  printf("this is storage: %d\n", storage);

  //now use a while loop to go through entire chars in buffer
  while (storage != 0)
  {
    charN = get_buff_1();                                                       //pull from our buffer to check for the \n then the S
    if (charN == '\n')                                                          //check if its a newline possible for STOP
    { //check if its the S
      charS = get_buff_1();
      if (charS == 'S')
      { //check if its the T
        charT = get_buff_1();
        if (charT == 'T')
        {
          charO = get_buff_1();
          if (charO == 'O')
          { //check if its the P
            charP = get_buff_1();
            if (charP == 'P')
            { //check to see if its the \n
              charNew = get_buff_1();
              if (charNew == '\n')
              {
                printf("WE FOUND AN ACTUAL STOP\n");
                storage = 0;  //break the loop
              }
              else if (charNew != '\n')
              { //its not the new line so push in everything and subtract from storage
                put_buff_2(charN);
                put_buff_2(charS);
                put_buff_2(charT);
                put_buff_2(charO);
                put_buff_2(charP);
                put_buff_2(charNew);
                storage = storage - 6;
              }
            }
            else if (charP != 'P')  //its not the P
            { //push all remenants back in and subtract from storage each time
              put_buff_2(charN);
              put_buff_2(charS);
              put_buff_2(charT);
              put_buff_2(charO);
              put_buff_2(charP);
              storage = storage - 5;
            }
          }
          else if (charO != 'O') //its not the O
          {
            put_buff_2(charN);
            put_buff_2(charS);
            put_buff_2(charT);
            put_buff_2(charO);
            storage = storage - 4;
          }
        }
        else if //its not the t
        {
          put_buff_2(charN);
          put_buff_2(charS);
          put_buff_2(charT);
          storage = storage - 3;
        }
      }
      else if //its not a S
      {
        put_buff_2(charN);
        put_buff_2(charS);
        storage = storage - 2;
      }
    }
    else if //its not the \n


    //check for stop
    //pull and check batch of 5 chars
    charS = get_buff_1();                                                       //pull from the buffer the char we are looking at
    if (charN == '\n' && charS == 'S')
    { //it matched with s so check for T
      //printf("found an S looking for T now...\n");
      storage = storage - 1;
      charT = get_buff_1();   //pull from buffer again
      if (charT == 'T')
      { //it matched with T so check for O
        //printf("found an T looking for O now...\n");
        storage = storage - 1;
        charO = get_buff_1(); //pull from buffer again
        if (charO == 'O')
        { //it matched with O so check for P
          //printf("found an O looking for P now...\n");
          storage = storage - 1;
          charP = get_buff_1(); //pull from buffer again
          if (charP == 'P')
          { //it matched with P so now check for \n
            //printf("found an P looking for newline now...\n");
            storage = storage - 1;
            charNew = get_buff_1(); //pull from buffer again
            if (charNew == '\n')
            {
              //we found a match so resize sizeof_stdin;
              printf("found a VALID STOP ending loop...\n");
              //sizeof_stdin = storage;
              //printf("testing sizeof_stdin: %d\n", sizeof_stdin);
              storage = 0;
            }
            else if (charNew != '\n') //it didn't match with \n
            { //push P, O, T, S
              //printf("didn't find the newline restarting...\n");
              put_buff_2(charP);
              put_buff_2(charO);
              put_buff_2(charT);
              put_buff_2(charS);
              put_buff_2(charN);
            }
          }
          else if (charP != 'P') //it didnt match with P
          {
            //printf("didn't find the P restarting...\n");
            put_buff_2(charO);
            put_buff_2(charT);
            put_buff_2(charS);
            put_buff_2(charN);
          }
        }
        else if (charO != 'O')  //it didn't match with O
        {
          //printf("didn't find the O restarting...\n");
          put_buff_2(charT);
          put_buff_2(charS);
          put_buff_2(charN);
        }
      }
      else if (charT != 'T')  //it didnt match with T
      {
        //printf("didn't find the T restarting...\n");
        put_buff_2(charS);
        put_buff_2(charN);
      }
    }
    else if (charS != 'S')  //it didnt match with S
    {
      //check for newline and delete if there is new line
      //printf("didn't find the S restarting...\n");
      if (charS == '\n')                                                    //if the char is a new line we delete it
      {
        //printf("yep its a newline to delete.\n");
        //sizeof_stdin = sizeof_stdin+1;
        charS = ' ';
        //printf("calling put buff 2\n");
        put_buff_2(charS);                                                  //put the char into the next buffer using put buff 2
        storage = storage -1;
      }
      else if (charS != '\n')                                               //else if its not a newline char
      {
        //printf("didn't find a newline to delete..\n");
        put_buff_2(charS);                                                  //just pass it to the next buffer right away
        storage = storage - 1;
      }
    }
  }
  printf("testing outside of while loop sizeof_stdin: %d\n", sizeof_stdin);
  return NULL;
}

//get buff function for ++ sign to pull from line sep buffer
char get_buff_2()                                                               //ALL CITED FROM EXAMPLE PROGRAM
{
  //printf("in get buff2\n");
  pthread_mutex_lock(&mutex_2);                                                 //lock the mutex before checking if the buffer has data
  while (count_2 == 0)
  {
    pthread_cond_wait(&full_2, &mutex_2);                                       //buffer is empty so we wait for prod to signal to the consumer that the buffer has data
  }
  char currentChar = buffer_2[con_idx_2];                                       //we store the buffer data into char var
  con_idx_2 = con_idx_2 + 1;                                                    //increment our index in our buffer
  count_2--;
  pthread_mutex_unlock(&mutex_2);                                               //unlock the mutex
  return currentChar;                                                           //return the char we want
}

//put buff 3 for the plus plus sign function to store into 3rd buffer for the output function
void put_buff_3(char item)
{
  //printf("in put buff 3\n");
  //printf("buffer 3 item: %c\n", item);
  pthread_mutex_lock(&mutex_3);                                                 //lock the mutex before putting item in the buffer
  buffer_3[prod_idx_3] = item;                                                  //put items into the buffer
  prod_idx_3 = prod_idx_3 + 1;                                                  //increment the index where the next item will be put
  count_3++;
  pthread_cond_signal(&full_3);                                                 //signal to the consumer that the buffer is no longer empty
  pthread_mutex_unlock(&mutex_3);                                               //unlock the mutex
}

//plus sign thread will pull from buff 2 array with get buff 2
//needs to process and adjust buffer 1 to +1 index so it knows to check next element
//put buff 3 will be called and it will store into 3rd buffer array
void *plusplusSign(void *args)
{
  int counterable = sizeof_stdin;
  //printf("size of stdin in plusplus %d\n", sizeof_stdin);
  char currentChar1, currentChar2, currentChar3;
  //printf("inside plus plus sign\n");
  for (int i = 0; i < sizeof_stdin; i++)
  {
    //printf("\ncounterable: %d\n", counterable);
    if (counterable != 1)
    {
      currentChar1 = get_buff_2();                                                //get the first value to check
      currentChar2 = get_buff_2();                                                //pull again and get the second value
    }
    else if (counterable == 1)
    {
      return NULL;
    }
    //printf("in the loop %d\n", i);
    //printf("currentChar1: %c\n", currentChar1);                               //test to ensure that the char is carrying over
    //printf("currentChar2: %c\n", currentChar2);
    if (currentChar2 == '\n')                                                   //if the 2nd value is new line we are at a\n or end of file
    {
      fflush(stdout);
      //printf("%c", currentChar1);
      put_buff_3(currentChar1);
      counterable--;
      return NULL;
    }
    else if (currentChar1 == '\n')                                              //otherwise if the currentChar1 ==
    {
      return NULL;
    }
    else if ((currentChar1 == '+') && (currentChar2 == '+'))
    {
      char sigma = '^';
      fflush(stdout);
      //printf("%c", sigma);
      put_buff_3(sigma);
      counterable--;
      counterable--;
    }
    //else if ((currentChar1 == ' ') && (currentChar2 == '+'))                  //check if we have a ++ next to each other
    else if (currentChar2 == '+')                                               //checks for the following: a+a, _+_, a+_, _+a
    {
      //check if the next char is also a +
      currentChar3 = get_buff_2();
      if (currentChar3 == '+')                                                  //we have a double plus
      {
        char sigma = '^';
        fflush(stdout);
        //printf("%c", currentChar1);
        fflush(stdout);
        //printf("%c", sigma);
        put_buff_3(currentChar1);
        counterable--;
        put_buff_3(sigma);                                                      //if the third one is + we evaluate it to ^
        counterable--;
        counterable--;
      }
      else
      {
        //otherwise just put the 3 values into
        fflush(stdout);
        printf("%c", currentChar1);                                             //otherwise we push all 3 in
        fflush(stdout);
        printf("%c", currentChar2);
        fflush(stdout);
        printf("%c", currentChar3);
        put_buff_3(currentChar1);
        counterable--;
        put_buff_3(currentChar2);
        counterable--;
        put_buff_3(currentChar3);
        counterable--;
      }
    }
    else                                                                        //otherwise its not a ++ so we just put in all the chars
    {
      //printf("putting stuff into buffer 3\n");                                //tests to make sure we have the right things to put into buffer 3
      fflush(stdout);
      //printf("%c", currentChar1);
      fflush(stdout);
      //printf("%c", currentChar2);
      put_buff_3(currentChar1);
      counterable--;
      put_buff_3(currentChar2);
      counterable--;
    }
  }
  //printf("exited loop\n");                                                    //test to make sure our loop ended
  return NULL;
}

//get buff 3 function for the output function to write with
char get_buff_3()                                                               //ALL CITED FROM EXAMPLE PROGRAM
{
  //printf("in get buff2\n");
  pthread_mutex_lock(&mutex_3);                                                 //lock the mutex before checking if the buffer has data
  while (count_3 == 0)
  {
    pthread_cond_wait(&full_3, &mutex_3);                                       //buffer is empty so we wait for prod to signal to the consumer that the buffer has data
  }
  char currentChar = buffer_3[con_idx_3];                                       //we store the buffer data into char var
  //printf("currentChar in buff 3: %c\n", currentChar);
  con_idx_3 = con_idx_3 + 1;                                                    //increment our index in our buffer
  count_3--;
  pthread_mutex_unlock(&mutex_3);                                               //unlock the mutex
  return currentChar;                                                           //return the char we want
}

//output thread will be called and pull from buff 3
//needs to print and adjust buffer to +1 so it knows to check for next element
void *write_output(void *args)
{
  int counterable = sizeof_stdin;
  counterable = counterable/80;                                                 //this should give you how many lines to write
  printf("in write output\n");
  char currentChar;
  //printf("this is buffer_2[1]: %c\n", buffer_2[1]);
  //printf("size of size: %d\n", sizeof_stdin);
  for (int i = 0; i < sizeof_stdin-1; i++)
  {
    currentChar = get_buff_2();
    fflush(stdout);
    printf("%c", currentChar);
  }
  //printf("finished loop\n");
  printf("\n");
  return NULL;
}

int main(void)
{
  //create all 4 threads
  pthread_t input_t, linesep_t, plus_sign_t, output_t;

  pthread_create(&input_t, NULL, get_input, NULL);                              //create our thread for get Input
  pthread_join(input_t, NULL);                                                  //join the threads
  //printf("in MAIN this is sizeof: %d\n", sizeof_stdin);

  //for (int i = 0; i < sizeof_stdin; i++)
  //{
  pthread_create(&linesep_t, NULL, lineSeperator, NULL);
  //}
  pthread_join(linesep_t, NULL);
  //printf("in MAIN 2.0 this is sizeof: %d\n", sizeof_stdin);
  //for (int i = 0; i < sizeof_stdin; i++)
  //{
  //pthread_create(&plus_sign_t, NULL, plusplusSign, NULL);
  //printf("back in main from plus plus\n");
  //pthread_join(plus_sign_t, NULL);
  //printf("in MAIN 3.0 this is sizeof: %d\n", sizeof_stdin);


  pthread_create(&output_t, NULL, write_output, NULL);                          //create our thread for write output
  //}
  //pthread_join(input_t, NULL);
  pthread_join(output_t, NULL);
  return EXIT_SUCCESS;
}
