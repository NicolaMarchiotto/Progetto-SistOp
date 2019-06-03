#include <string.h>
#include <stdio.h>
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

void printSemaphoresValue (int semid) {
    unsigned short semVal[1];
    union semun arg;
    arg.array = semVal;

    // get the current state of the set
    if (semctl(semid, 0 /*ignored*/, GETALL, arg) == -1)
        errExit("semctl GETALL failed");

    // print the semaphore's value
    printf("semaphore set state: %d\n",semVal[0]);
}



void strlwr(char * str){

  int i=0;

  while(str[i] !='\0'){
    if(str[i]>='A' && str[i]<='Z')
      str[i]=str[i]+32;

    i++;
  }
}
