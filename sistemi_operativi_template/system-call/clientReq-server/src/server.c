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

char FifoServer[20]="FIFOSERVER";
char FifoClient[20];
pid_t pid;
int shmidInt;
int *ptr_count;
int shmid;
struct mynode *ptr_vet;
struct mynode *ptr_sup;
int semid;

const int semkey=1;
const int shmkey=2;
const int shmkeyint=3;



void sigHandler(int sing) {

//DEALLOCATING RESOURCES, KILLING SERVER AND KEYMANAGER

	if(sing==SIGTERM){
		if(pid>0){
      //printf("\n<Server> Deleting %s", FifoServer);
      if(unlink(FifoServer)==-1)
        printf("\n<Server> Deleting %s error\n", FifoServer);

			kill(pid,SIGTERM);

			if(shmdt(ptr_vet)==-1)
				errExit("\nshmdt error for ptr_vet");
			if(shmctl(shmid,IPC_RMID,NULL)==-1)
				errExit("\nshmctl error for shmid");

			if(shmdt(ptr_count)==-1)
				errExit("\nshmdt error for ptr_count");
			if(shmctl(shmidInt,IPC_RMID,NULL)==-1)
				errExit("\nshmctl error for shmidInt");

			if(semctl(semid, 0,IPC_RMID))
				errExit("semctl error for deleting semset");

			//printf("\n\n\nFine file Server\n\n\n");
			wait(NULL);
      exit(0);
		}
		else
			exit(0);
  }
}

int main (int argc, char *argv[]) {

  struct Request req;
  struct Response resp;

//BLOCCO TUTTI I SEGNALI APPARTE SIGTERM

  // set of signals (N.B. it is not initialized!)
  sigset_t mySet;
  // initialize mySet to contain all signals -> SEGNALI BLOCCATI=TUTTI
  sigfillset(&mySet);
  // remove SIGTERM from mySet -> ABLITO SIGINT AD ESSERE RICEVUTO
  sigdelset(&mySet, SIGTERM);
  // blocking all signals but SIGINT -> faccio diventare la maschera del processo myset
  sigprocmask(SIG_SETMASK, &mySet, NULL);

  // set the function sigHandler as handler for the signal SIGINT -> SOVRASCRIVO FUNZIONE HANDLER SIGINT
  if (signal(SIGTERM,sigHandler) == SIG_ERR)
      errExit("change signal handler failed");

//CREO SET DI SEMAFORI
	semid=semget(semkey, 1, IPC_CREAT | S_IWUSR | S_IRUSR);
	if (semid == -1)
			errExit("semget failed");

	// Initialize the semaphore set with semctl
	unsigned short semInitVal[] = {1};
	union semun arg;
	arg.array = semInitVal;

	if(semctl(semid, 0, SETALL, arg))
			errExit("semctl SETALL failed");


//CREATING SHARED MEMORY

  shmid=shmget(shmkey,300*sizeof(struct mynode), IPC_CREAT | S_IRUSR | S_IWUSR );
	if(shmid==-1)
			errExit("\nshmget error for shmid");

	ptr_vet=(struct mynode *)shmat(shmid,NULL,0);
	ptr_sup=ptr_vet;
  //creating count for helping managing


  shmidInt=shmget(shmkeyint,sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR );
	if(shmidInt==-1)
		errExit("\nshmget error for shmidInt");
  ptr_count=(int *)shmat(shmidInt,NULL,0);

	*ptr_count=0;

//WELCOME

	printf("Hi, I'm Server program!\n");


//CREATING KEY MANAGER

  pid = fork();

  if (pid == -1)
      printf("KeyManager not created!");

	if (pid==0)
	{
		int i=0;

		while(1)
		{
			printf("\n\n\nPrinting db\n---------------------\n");

			for(i=0;i<*ptr_count;i++){
				if( ((time_t)time(NULL))-(time_t)(ptr_vet[i].time) >= 300){
					//swap

					//printf("\n\n\n\n\nDENTRO IF, TROVATA ENTRI CHE SCADE\n\n\n\n");

					semOp(semid,0 ,-1);
					strcpy(ptr_vet[i].id,ptr_vet[*ptr_count-1].id);
					ptr_vet[i].key=ptr_vet[*ptr_count-1].key;
					ptr_vet[i].time=ptr_vet[*ptr_count-1].time;

					(*ptr_count)--;

					i=i-1;

					//printf("\ndopo\n i: %i\nptr_count: %i\n", i,*ptr_count);

					semOp(semid,0,1);
				}
				else{
					if((*ptr_count)!=0)
						printf("\nEntry number %i:\nId: %s\nKey: %ld\nTime_stamp: %ld\n",i+1,ptr_vet[i].id,ptr_vet[i].key,ptr_vet[i].time);
				}
			}

		if((*ptr_count)==0)
				printf("\nLooks like the database is empty\n");

			//printf("\nValore di ptr_count: %i\n", *ptr_count);

		 sleep(30);
		}
	}


//CREATING FIFOSERVER

  //printf("\n<Server> Creating %s", FifoServer);
  int fd=mkfifo(FifoServer, O_CREAT | S_IRUSR | S_IWUSR);
  if(fd==-1)
    errExit("\n<Server> MkFifo error\n");

//OPENIG FIFOSERVER

  //printf("\n<Server> Opening %s", FifoServer);
  int fs=open(FifoServer, O_RDONLY);

	if(fs==-1)
    errExit("\n<Server> Open fifo error\n");

//OPENIG FIFOSERVER FAKECLIENT

    //printf("\n<Client fake> Opening %s", FifoServer);
    int fake_fs=open(FifoServer, O_WRONLY);

    if(fake_fs==-1)
      errExit("\n<Client fake> Open fifo error\n");

//READING FIFOSERVER

  while(1)
  {
    int br=read(fs,&req,sizeof(struct Request));

    //Checking the number of bytes from the FIFO
    if (sizeof(req)!=sizeof(struct Request))
      printf("\n<Server> %s looks broken", FifoServer);
    if (br!=sizeof(struct Request))
      printf("\n<Server> looks like you haven't recived a struct Requst\n");

    //printf("\nRequest:\n%s\n%s\n%s\n",req.id,req.servizio,req.fifo_name);

  //OPENING FIFOCLIENT

    strcpy(FifoClient,req.fifo_name);

    //printf("\n<Server> Opening %s", FifoClient);
    int fc=open(FifoClient, O_WRONLY);

    if(fc==-1)
      errExit("\n<Server> Open fifo error\n");

//GENERATING KEY
  resp.key=getkey(req.servizio);
	strcpy(resp.id,req.id);
	strcpy(resp.servizio,req.servizio);


	semOp(semid, 0 , -1);

	if(resp.key!=0){
		long int a=(long int)time(NULL);
		struct mynode m;
		m.key=resp.key;
		m.time=a;
		strcpy(m.id,resp.id);

		ptr_vet[*ptr_count]=m;
		*ptr_count=*ptr_count+1;
	}


	semOp(semid, 0 , 1);

	//WRITING IN FIFOCLIENT
	printf("\nWriting back to client...\n");
  write(fc,&resp,sizeof(struct Response));

	}

  return 0;
}
