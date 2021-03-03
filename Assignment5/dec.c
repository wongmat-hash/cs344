#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

static const char library[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";                  //our library of in bounds allowed chars

//function to display error messages
void error(const char *msg)
{
  perror(msg);                                                                  //displays error message
  exit(1);
}
