#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "my_library.h"
#include "errExit.h"

int shmidInt;
int *ptr_count;
int shmid;
struct mynode *ptr_vet;
int semid;

const int semkey=1;
const int shmkey=2;
const int shmkeyint=3;

#define SIZE 300
#define TIME 300

int main (int argc, char *argv[]) {
    printf("Hi, I'm ClientExec program!\n");


    if (argc <= 2) {
        printf("\n<ClientExec> Usage: %s username, key, service arguments\n", argv[0]);
        return 0;
    }

    char username[21]="";
    strcpy(username,argv[1]);
    long int key=atol(argv[2]);


//GETTING SHARED MEMORY

    shmid=shmget(shmkey,SIZE*sizeof(struct mynode), S_IRUSR | S_IWUSR );
  	if(shmid==-1){
  		errExit("\n<ClientExec> shmget error for shmid, run server firts");
	  }

  	ptr_vet=(struct mynode *)shmat(shmid,NULL,0);
    if(ptr_vet==(void *)-1)
  		errExit("\n<ClientExec> shmat tr_vet\n");

    shmidInt=shmget(shmkeyint,sizeof(int), S_IRUSR | S_IWUSR );
  	if(shmidInt==-1){
  		errExit("\n<ClientExec> shmget error for shmidInt, run server firts");
  	}

    ptr_count=(int *)shmat(shmidInt,NULL,0);
    if(ptr_count==(void *)-1)
  		errExit("\n<ClientExec> shmat tr_vet\n");

//GETTING SEMAPHORES SET

    semid=semget(semkey, 1, IPC_CREAT | S_IWUSR | S_IRUSR);
    if (semid == -1)
        errExit("<ClientExec> semget failed");

//CHECKING IF THERE'S A CORRISPONDING ENTRY

    int find=0;

    semOp(semid,0 ,-1);


    for(int i=0;i<*ptr_count;i++){
      if((strcmp(ptr_vet[i].id,username)==0) && (ptr_vet[i].key==key)){
          find=1;
          strcpy(ptr_vet[i].id,ptr_vet[*ptr_count-1].id);
          ptr_vet[i].key=ptr_vet[*ptr_count-1].key;
          ptr_vet[i].time=ptr_vet[*ptr_count-1].time;
          (*ptr_count)--;
          break;
      }
    }

    semOp(semid,0,1);

    int service=0;

    if(find==0)
      printf("\n<ClientExec> Matching not found\n\n");
    else{
      printf("\n<ClientExec> Matching found\n\n");

      service=key%10;

      switch (service) {
        case 1:
          if(execv("stampa", argv)==-1)
            errExit("\n<ClientExec> execl failed\n");
          break;

        case 2:
          if(execv("salva", argv)==-1)
            errExit("\n<ClientExec> execl failed\n");
          break;

        case 3:
          if(execv("invia", argv)==-1)
            errExit("\n<ClientExec>execl failed\n");
          break;

        default:
          printf("\n<ClientExec> Some error occured\n");
          break;

      }

    }

    return 0;
}
