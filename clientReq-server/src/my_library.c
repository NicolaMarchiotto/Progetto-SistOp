#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/types.h>

#include "my_library.h"
#include "errExit.h"


/* errsemOpExit is a support function to manipulate a semaphore's value
 * of a semaphore set. semid is a semaphore set identifier, sem_num is the
 * index of a semaphore in the set, sem_op is the operation performed on sem_num
 */

void semOp (int semid, unsigned short sem_num, short sem_op) {

    struct sembuf sop;
    sop.sem_num=sem_num;
    sop.sem_op=sem_op;
    sop.sem_flg=0;

    if (semop(semid, &sop, 1) == -1)
        errExit("semop failed");
}

//function which prints the semaphores value, for debugging

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

//function for generating key

long int getkey(char s[20]){

	printf("\nGenerating key...\n");

  long int t=(long int)time(NULL);

  long int key=0;

  if(strcmp(s,"stampa")==0)
    key=(t*10)+1;
  else if(strcmp(s,"salva")==0)
    key=(t*10)+2;
  else if(strcmp(s,"invia")==0)
    key=(t*10)+3;
	else key=0;

  sleep(1);

  return key;
}

//string_to_lower_case function

void strlwr(char * str){

  int i=0;

  while(str[i] !='\0'){
    if(str[i]>='A' && str[i]<='Z')
      str[i]=str[i]+32;

    i++;
  }
}
