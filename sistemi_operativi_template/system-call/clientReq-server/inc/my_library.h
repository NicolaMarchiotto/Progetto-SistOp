#ifndef _REQUEST_HH
#define _REQUEST_HH
#include <string.h>
#include <ctype.h>

void strlwr(char * str);

void semOp (int semid, unsigned short sem_num, short sem_op);

void printSemaphoresValue (int semid);

long int getkey(char s[20]);

struct Request{
  char id[20];
  char servizio[10];
  char fifo_name[20];
};

struct Response{
  char id[20];
  char servizio[10];
  long int key;
};

struct mymsg{
  long type;
  char text[100];
};

struct mynode{
  char id[20];
  long int key;
  long int time;
};

union semun{
    int val;
    struct semid_ds * buf;
    unsigned short * array;
};

#endif
