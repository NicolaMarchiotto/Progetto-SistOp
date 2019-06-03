#ifndef _REQUEST_HH
#define _REQUEST_HH
#include <string.h>
#include <ctype.h>

void strlwr(char * str);

struct Request{
  char id[20];
  char servizio[10];
  char fifo_name[20];
};

struct Response{
  char id[20];
  char servizio[10];
  int key;
};

struct mymsg{
  long type;
  char text[100];
};

struct mynode{
  char id[20];
  int key;
  int time;
};

#endif
