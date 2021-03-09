#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[])
{
  int length = atoi(argv[1]);
  char key[length+1];
  srand(time(0));

  for (int i = 0; i< length; i++)
  {
  int letter = rand() % 27;
  key[i] = code[letter];

  key[length] = '\0';
  printf("%s\n", key);

  return 0;
  }
}
