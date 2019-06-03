#include <string.h>
#include <ctype.h>
#include <sys/sem.h>
#include "my_library.h"
#include "errExit.h"

void semOp (int semid, unsigned short sem_num, short sem_op) {

    struct sembuf sop;
    sop.sem_num=sem_num;
    sop.sem_op=sem_op;
    sop.sem_flg=0;

    if (semop(semid, &sop, 1) == -1)
        errExit("semop failed");
}


void strlwr(char * str){

  int i=0;

  while(str[i] !='\0'){
    if(str[i]>='A' && str[i]<='Z')
      str[i]=str[i]+32;

    i++;
  }
}
