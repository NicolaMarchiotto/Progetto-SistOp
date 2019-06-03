#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <time.h>

#include "my_library.h"
#include "errExit.h"

char FifoServer[20]="FIFOSERVER";
char FifoClient[20];
pid_t pid;
int *ptr_count;
struct mynode *ptr_vet;


void sigHandler(int sing) {
//DELETING FIFOSERVER AND KILLING SERVER

	if(sing==SIGTERM){
      printf("\n<Server> Deleting %s", FifoServer);
      if(unlink(FifoServer)==-1)
        printf("\n<Server> Deleting %s error\n", FifoServer);
      else
        printf("\n<Server> %s deleted\n", FifoServer);

      kill(pid,SIGTERM);

      printf("\n\n\nFine file Server\n\n\n");
      exit(0);
  }

  if(sing==SIGALRM){

    //#####RIVEDI###
    for(int i=0;i<*ptr_count;i++){
      if(ptr_vet[i].time>=300)
        //ptr_vet[i]=NULL;
    alarm(30);
  	}
	}
}

//GETKEY FUNCTION

int getkey(char s[20]){

  int t=(int)time(NULL);

  int key=0;

  if(strcmp(s,"stampa")==0)
    key=(t*10)+1;
  else if(strcmp(s,"salva")==0)
    key=(t*10)+2;
  else
    key=(t*10)+3;

  sleep(1);

  return key;
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
  sigdelset(&mySet, SIGALRM);
  // blocking all signals but SIGINT -> faccio diventare la maschera del processo myset
  sigprocmask(SIG_SETMASK, &mySet, NULL);

  // set the function sigHandler as handler for the signal SIGINT -> SOVRASCRIVO FUNZIONE HANDLER SIGINT
  if (signal(SIGTERM,sigHandler) == SIG_ERR)
      errExit("change signal handler failed");

//WELLCOME

  printf("Hi, I'm Server program!\n");

//CREATING SHARED MEMORY

  int shmid=shmget(5,1000*sizeof(struct mynode), IPC_CREAT | O_RDWR );
  ptr_vet=(struct mynode *)shmat(shmid,NULL,0);


  //creating count for helping managing
  int shmidInt=shmget(6,sizeof(int), IPC_CREAT | O_RDWR );
  ptr_count=(int *)shmat(shmidInt,NULL,0);
  *ptr_count=0;

//CREATING KEY MANAGER

  pid = fork();

  if (pid == -1)
      printf("KeyManager not created!");
  else
	{
		if (pid==0)
		{
     if(signal(SIGALRM, sigHandler)==SIG_ERR)
       printf("\nsigHandler for ALARM failed");

     for(int i=0;i<*ptr_count;i++){
       if(ptr_vet[i].time>=300)
         printf("");//ptr_vet[i]=NULL;
     	}

		 alarm(30);
		}
	}
//CREATING FIFOSERVER

  printf("\n<Server> Creating %s", FifoServer);
  int fd=mkfifo(FifoServer, O_CREAT | S_IRUSR | S_IWUSR);
  if(fd==-1)
    printf("\n<Server> MkFifo error\n");
  else
    printf("\n<Server> %s created\n",FifoServer);

//OPENIG FIFOSERVER

  printf("\n<Server> Opening %s", FifoServer);
  int fs=open(FifoServer, O_RDONLY);

	if(fs==-1)
    printf("\n<Server> Open fifo error\n");
  else
    printf("\n<Server> %s opened\n",FifoServer);

//OPENIG FIFOSERVER FAKECLIENT

    printf("\n<Client fake> Opening %s", FifoServer);
    int fake_fs=open(FifoServer, O_WRONLY);

    if(fake_fs==-1)
      printf("\n<Client fake> Open fifo error\n");
    else
        printf("\n<Client fake> %s opened\n",FifoServer);

//READING FIFOSERVER

  while(1)
  {
    int br=read(fs,&req,sizeof(struct Request));

    //Checking the number of bytes from the FIFO
    if (sizeof(req)!=sizeof(struct Request))
      printf("\n<Server> la %s sembra rotta", FifoServer);
    if (br!=sizeof(struct Request))
      printf("\n<Server> sembra che non ho ricevuto una struct Request\n");

    //printf("\nRequest:\n%s\n%s\n%s\n",req.id,req.servizio,req.fifo_name);

  //OPENING FIFOCLIENT

    strcpy(FifoClient,req.fifo_name);

    printf("\n<Server> Opening %s", FifoClient);
    int fc=open(FifoClient, O_WRONLY);

    if(fc==-1)
      printf("\n<Server> Open fifo error\n");
    else
      printf("\n<Server> %s opened\n",FifoClient);

//GENERATING KEY
  resp.key=getkey(req.servizio);



//WRITING IN FIFOCLIENT

    strcpy(resp.id,req.id);
    strcpy(resp.servizio,req.servizio);
    write(fc,&resp,sizeof(struct Response));
  }


  return 0;
}
