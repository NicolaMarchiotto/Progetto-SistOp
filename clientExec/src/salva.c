#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "my_library.h"
#include "errExit.h"

int main (int argc, char *argv[]) {

    char buf[50]="";

    if(argc<5){
      printf("\nWrong usage! id, key_service, filename_destionation, text_to_write\n");
      exit(0);
    }

    printf("Hi, I'm Salva program!\n");

    int file=open(argv[3], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if(file==-1)
      printf("\nErrore open file per servizio Stampa\n");

    for(int i=4;i<argc;i++){
      strcat(buf, argv[i]);
      strcat(buf, " ");
    }

    ssize_t w=write(file, buf, strlen(buf));
    if(w==-1)
      printf("\nErrore write per il servizio Stampa");
    else
      printf("\nService Stampa completed\n");

    if(close(file)==-1)
      errExit("\nclose file error on service stampa");

    return 0;
}
