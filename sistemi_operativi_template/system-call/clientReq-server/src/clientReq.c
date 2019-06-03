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

  printf("\nInserire il nome utente:");
  scanf(" %[^\n]s", nome_utente);
  printf("\nInserire il nome del servizio:");
  printf("\n- Stampa\n- Salva\n- Invio\n\nServizio:");

//INSERIMENTO SERVIZIO

  scanf(" %[^\n]s", servizio);
  strlwr(servizio);

//CREATING FIFOCLIENT

  printf("\n<Client %i> Creating %s",getpid(), FifoClient);
  int fd=mkfifo(FifoClient, O_CREAT | S_IRUSR | S_IWUSR);
  if(fd==-1)
    printf("\n<Client %i> MkFifo error\n",getpid());
  else
    printf("\n<Client %i> %s created\n",getpid(),FifoClient);

//OPENIG FIFOSERVER

  printf("\n<Client %i> Opening %s",getpid(), FifoServer);
  int fs=open(FifoServer, O_WRONLY);
  if(fs==-1)
    printf("\n<Client %i> Open fifo error\n", getpid());
  else
    printf("\n<Client %i> %s opened\n",getpid(),FifoServer);


//WRITING IN FIFOSERVER

  strcpy(req.id,nome_utente);
  strcpy(req.servizio,servizio);
  strcpy(req.fifo_name,FifoClient);

  write(fs,&req,sizeof(struct Request));

//OPENING FIFOCLIENT

  printf("\n<Client %i> Opening %s",getpid(), FifoClient);
  int fc=open(FifoClient, O_RDONLY);
  if(fc==-1)
    printf("\n<Client %i> Open fifo error\n",getpid());
  else
    printf("\n<Client %i> %s opened\n",getpid(),FifoClient);

//READING FIFOCLIENT

  int br=read(fc,&resp,sizeof(struct Response));

  //Checking the number of bytes from the FIFO
  if (sizeof(resp)!=sizeof(struct Response))
    printf("\n<Client %i> la %s sembra rotta", getpid(), FifoClient);
  if (br!=sizeof(struct Response))
    printf("\n<Client %i> sembra che non ho ricevuto una struct Response\n", getpid());

  printf("\nRESPONSE\nkey: %i\n",resp.key);

//DELETING FIFOCLIENT

  printf("\n<Client %i> Deleting %s",getpid(), FifoClient);
  if(unlink(FifoClient)==-1)
    printf("\n<Client %i> Deleting %s error\n",getpid(), FifoClient);
  else
    printf("\n<Client %i> %s deleted\n",getpid(), FifoClient);

  printf("\n\n\nFine file ClientReq %i\n\n\n", getpid());

  return 0;
}
