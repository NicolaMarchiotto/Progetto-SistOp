#ifndef _REQUEST_HH
#define _REQUEST_HH
#include <string.h>
#include <ctype.h>

void strlwr(char * str);

/* errsemOpExit is a support function to manipulate a semaphore's value
 * of a semaphore set. semid is a semaphore set identifier, sem_num is the
 * index of a semaphore in the set, sem_op is the operation performed on sem_num
 */
void semOp (int semid, unsigned short sem_num, short sem_op);

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

struct senum{
    int val;
    struct semid_ds * buf;
    unsigned short * array;
};

#endif
