#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



int main (int argc, char *argv[]) {
    printf("Hi, I'm Salva program!\n");

    char buf[50]="";

    int file=open(argv[1], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if(file==-1)
      printf("\nErrore open file per servizio Stampa");

    for(int i=2;i<argc;i++){
      strcat(buf, argv[i]);
      strcat(buf, " ");
    }

    ssize_t w=write(file, buf, strlen(buf));
    if(w==-1)
      printf("\nErrore write per il servizio Stampa");

    close(file);

    return 0;
}
