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
      if(argc<2)
        printf("\nWrong usage! key_t key *char text");
    }
    while(argc<2)

    struct mymsg msg;
    key_t key=atoi(argv[1]);
    char buf[100];

    int prova=msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);

    int msqid=msgget(key, S_IRUSR | S_IWUSR);

    for(int i=2;i<argc;i++){
      strcat(buf, argv[i]);
      strcat(buf, " ");
    }

    msg.type=1;
    strcpy(msg.text,buf);

    size_t mSize=sizeof(struct mymsg)-sizeof(long);

    if(msgsnd(msqid, &msg, mSize, 0)==-1)
      printf("\nErrore msgsnd servizio Invia");

    /*
    struct mymsg resp;

    if(msgrcv(msqid,&resp,mSize,1,0)==-1)
      printf("\nmsgrcv error");

    printf("\nRESP: %s", resp.text);

    */

    if(msgctl(msqid,IPC_RMID,NULL)==-1)
      errExit("msgctl failed service Invia");



    return 0;
}
