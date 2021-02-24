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
  char charN, charS, charT, charO, charP, charNew;                              //storage var to check for /nSTOP/n
  int storage = sizeof_stdin;                                                   //how we break out of our loop
  storage = storage;
  //printf("testing of init storage: %d\n", storage);

  do
  {
    //printf("storage val: %d\n", storage);
    charN = get_buff_1();                                                       //check if its a /n followed by S
    charS = get_buff_1();                                                       //but since we need a case with just /n check for S also
    //printf("charN val: %c\n", charN);
    //printf("charS val: %c\n", charS);
    if (charN == '\n' && charS == 'S')
    {
      storage = storage - 1;
      storage = storage - 1;
      //we found the potential start of STOP
      charT = get_buff_1();                                                     //now check for the T
      //printf("charT val: %c\n", charT);
      if (charT == 'T')
      {
        storage = storage - 1;
        //we found the T now look for O
        charO = get_buff_1();
        //printf("charO val: %c\n", charO);
        if (charO == 'O')
        {
          storage = storage - 1;
          //we found the O now look for the P
          charP = get_buff_1();
          //printf("charP val: %c\n", charP);
          if (charP == 'P')
          {
            storage = storage - 1;
            charNew = get_buff_1();
            //printf("charNew val: %c\n", charNew);
            if (charNew == '\n')
            {
              // we found the newline and end of our loop
              storage = 0;
            }
            else if (charNew != '\n')
            {
              //failed to find STOP restarting
              put_buff_2(charN);
              put_buff_2(charS);
              put_buff_2(charT);
              put_buff_2(charO);
              put_buff_2(charP);
              put_buff_2(charNew);
              //printf("pushing the following in order: %c, %c, %c, %c, %c, %c\n", charN, charS, charT, charO, charP, charNew);
              //storage = storage -6;
            }
          }
          else if (charP != 'P')
          {
            //its not a P so push in charP, O, T, S '\n'
            put_buff_2(charN);
            put_buff_2(charS);
            put_buff_2(charT);
            put_buff_2(charO);
            put_buff_2(charP);
            //printf("pushing the following in order: %c, %c, %c, %c, %c\n", charN, charS, charT, charO, charP);
            //storage = storage -5;
          }
        }
        else if (charO != 'O')
        {
          //its not an O so push in charO, T, S, \n
          put_buff_2(charN);
          put_buff_2(charS);
          put_buff_2(charT);
          put_buff_2(charO);
          //printf("pushing the following in order: %c, %c, %c, %c\n", charN, charS, charT, charO);
          //storage = storage -4;
        }
      }
      else if (charT != 'T')
      {
        //its not a T so push in the charT, S, '\n'
        put_buff_2(charN);
        put_buff_2(charS);
        put_buff_2(charT);
        //printf("pushing the following in order: %c, %c, %c\n", charN, charS, charT);
        //storage = storage -3;
      }
    } //ends the char == '\n and chars == s if'
    else if (charN == '\n' && charS != 'S')
    {
      //its a /n we need to delete and keep S char
      charN = ' ';                                                              //set it to new space
      put_buff_2(charN);                                                        //push in the new space
      put_buff_2(charS);                                                        //push in the non S char
      //printf("pushing the following in order: %c, %c\n", charN, charS);
      storage = storage -2;
    }
    else if (charN != '\n' && charS == '\n')
    {
      //printf("found potential STOP new\n");
      storage = storage - 1;
      storage = storage - 1;
      //check if the \n could be start of a new STOP
      charT = get_buff_1();
      //printf("charT val: %c\n", charT);
      if (charT == 'S')
      {
        storage = storage - 1;
        charO = get_buff_1();
        //printf("charO val: %c\n", charO);
        if (charO == 'T')
        {
          storage = storage - 1;
          //we found the O now look for the P
          charP = get_buff_1();
          //printf("charP val: %c\n", charP);
          if (charP == 'O')
          {
            storage = storage - 1;
            charNew = get_buff_1();
            //printf("charNew val: %c\n", charNew);
            if (charNew == 'P')
            {
              storage = storage - 1;
              char charNewNew = get_buff_1();
              //printf("charNewNew: %c\n", charNewNew);
              if (charNewNew == '\n')
              {
                // we found the newline and end of our loop
                storage = 0;
              }
              else if (charNewNew!= '\n')
              {
                put_buff_2(charN);
                put_buff_2(charS);
                put_buff_2(charT);
                put_buff_2(charO);
                put_buff_2(charP);
                put_buff_2(charNew);
                put_buff_2(charNewNew);
                //printf("pushing the following in order: %c, %c, %c, %c, %c, %c, %c\n", charN, charS, charT, charO, charP, charNew, charNewNew);
              }
            }
            else if (charNew != 'P')
            {
              //failed to find STOP restarting
              put_buff_2(charN);
              put_buff_2(charS);
              put_buff_2(charT);
              put_buff_2(charO);
              put_buff_2(charP);
              put_buff_2(charNew);
              //printf("pushing the following in order: %c, %c, %c, %c, %c, %c\n", charN, charS, charT, charO, charP, charNew);
              //storage = storage -6;
            }
          }
          else if (charP != 'O')
          {
            //its not a P so push in charP, O, T, S '\n'
            put_buff_2(charN);
            put_buff_2(charS);
            put_buff_2(charT);
            put_buff_2(charO);
            put_buff_2(charP);
            //printf("pushing the following in order: %c, %c, %c, %c, %c\n", charN, charS, charT, charO, charP);
            //storage = storage -5;
          }
        }
        else if (charO != 'T')
        {
          //its not an O so push in charO, T, S, \n
          put_buff_2(charN);
          put_buff_2(charS);
          put_buff_2(charT);
          put_buff_2(charO);
          //printf("pushing the following in order: %c, %c, %c, %c\n", charN, charS, charT, charO);
          //storage = storage -4;
        }
      }//ends if
      else
      {
        //its not a STOP so just rewrite to space instead of \n
        //otherwise its not a space and just a char so push both in and repeate
        charS = ' ';
        put_buff_2(charN);                                                        //push in the new space
        put_buff_2(charS);                                                        //push in the non S char
        //printf("pushing the following in order: %c, %c\n", charN, charS);
        storage = storage -2;
      }
    }//ends else if
    else
    {
      //they are both not related just push them both
      put_buff_2(charN);                                                        //push in the new space
      put_buff_2(charS);                                                        //push in the non S char
      //printf("pushing the following in order: %c, %c\n", charN, charS);
      storage = storage -2;
    }
  }while(storage != 0);                                                          //loops as long as theres more char to find until STOP
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
  //int counterable = sizeof_stdin;
  //counterable = counterable/80;                                                 //this should give you how many lines to write
  printf("in write output\n");
  char currentChar;
  //printf("this is buffer_2[1]: %c\n", buffer_2[1]);
  //printf("size of size: %d\n", sizeof_stdin);
  printf("here is count_3: %d\n", count_3);
  int x = count_2;
  for (int i = 0; i < x; i++)
  {
    currentChar = get_buff_3();
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
  pthread_create(&plus_sign_t, NULL, plusplusSign, NULL);
  //printf("back in main from plus plus\n");
  pthread_join(plus_sign_t, NULL);
  //printf("in MAIN 3.0 this is sizeof: %d\n", sizeof_stdin);


  pthread_create(&output_t, NULL, write_output, NULL);                          //create our thread for write output
  //}
  //pthread_join(input_t, NULL);
  pthread_join(output_t, NULL);
  return EXIT_SUCCESS;
}
