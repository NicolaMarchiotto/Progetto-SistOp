#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/msg.h>

#include "my_library.h"
#include "errExit.h"

int main (int argc, char *argv[]) {
    printf("Hi, I'm Invia program!\n");

    do{
      if(argc<4)
        errExit("\nWrong usage! key_t key *char text\n");
    }
    while(argc<4);

    struct mymsg msg;
    char buf[100];
    key_t key=atoi(argv[3]);

    if(key<=0){
      printf("\nYou did not entered a numeric key\n");
      exit(0);
    }

    //int prova=msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);

    int msqid=msgget(key, S_IRUSR | S_IWUSR);

    for(int i=4;i<argc;i++){
      strcat(buf, argv[i]);
      strcat(buf, " ");
    }

    msg.type=1;
    strcpy(msg.text,buf);

    size_t mSize=sizeof(struct mymsg)-sizeof(long);

    if(msgsnd(msqid, &msg, mSize, 0)==-1)
      errExit("\nErrore msgsnd servizio Invia\n");
    else
      printf("\nService invia completed\n");


    /*struct mymsg resp;

    if(msgrcv(msqid,&resp,mSize,1,0)==-1)
      errExit("\nmsgrcv error");

    printf("\nRESP: %s\n", resp.text);*/


    if(msgctl(msqid,IPC_RMID,NULL)==-1)
      errExit("\nmsgctl failed service Invia\n");

    return 0;
}
