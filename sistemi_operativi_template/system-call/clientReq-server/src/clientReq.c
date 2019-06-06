#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "my_library.h"
#include "errExit.h"

int main (int argc, char *argv[]){

  char nome_utente[20];
  char servizio[10];
  char buff1[50];


  struct Request req;
  struct Response resp;

//CREATING PATH FOR FIFOCLIENT

  char FifoClient[20]="FIFOCLIENT";
  char sup[10];
  sprintf(sup,"%d",getpid());
  strcat(FifoClient, sup) ;

//CREATING PATH FOR FIFOSERVER

  char FifoServer[20]="FIFOSERVER";

//PROGRAM START

  printf("Hi, I'm ClientReq program!\n");

  printf("\nDigit your user name:");
  scanf(" %[^\n]s", nome_utente);
  printf("\nDigit the service to select:");
  printf("\n- Stampa\n- Salva\n- Invio\n\nService: ");

//INSERIMENTO SERVIZIO

  scanf(" %[^\n]s", servizio);
  strlwr(servizio);

//CREATING FIFOCLIENT

  //printf("\n<Client %i> Creating %s",getpid(), FifoClient);
  int fd=mkfifo(FifoClient, O_CREAT | S_IRUSR | S_IWUSR);
  if(fd==-1){
    strcpy(buff1,"");
    sprintf(buff1,"\n<Client %i> MkFifo error\n",getpid());
    errExit(buff1);
  }

//OPENIG FIFOSERVER

  //printf("\n<Client %i> Opening %s",getpid(), FifoServer);
  int fs=open(FifoServer, O_WRONLY);
  if(fs==-1){
    strcpy(buff1,"");
    sprintf(buff1,"\n<Client %i> Open fifo error\n",getpid());
    errExit(buff1);
  }

//WRITING IN FIFOSERVER

  strcpy(req.id,nome_utente);
  strcpy(req.servizio,servizio);
  strcpy(req.fifo_name,FifoClient);

  write(fs,&req,sizeof(struct Request));

//OPENING FIFOCLIENT

  //printf("\n<Client %i> Opening %s",getpid(), FifoClient);
  int fc=open(FifoClient, O_RDONLY);
  if(fc==-1){
    strcpy(buff1,"");
    sprintf(buff1,"\n<Client %i> Open fifo error\n",getpid());
    errExit(buff1);
  }

//READING FIFOCLIENT

  int br=read(fc,&resp,sizeof(struct Response));

  //Checking the number of bytes from the FIFO
  if (sizeof(resp)!=sizeof(struct Response))
    printf("\n<Client %i> %s looks broken", getpid(), FifoClient);
  if (br!=sizeof(struct Response))
    printf("\n<Client %i> looks like you haven't recived a struct Response\n", getpid());

  if(resp.key==0)
    printf("\nIt looks like your entry was not one of the option\n");
  else
    printf("\nRESPONSE\nid: %s\nservice: %s\nkey: %ld\n",resp.id,resp.servizio,resp.key);

//DELETING FIFOCLIENT

  //printf("\n<Client %i> Deleting %s",getpid(), FifoClient);
  if(unlink(FifoClient)==-1){
    strcpy(buff1,"");
    sprintf(buff1,"\n<Client %i> Deleting %s error\n",getpid(), FifoClient);
    errExit(buff1);
  }

  //printf("\n\n\nFine file ClientReq %i\n\n\n", getpid());

  return 0;
}
