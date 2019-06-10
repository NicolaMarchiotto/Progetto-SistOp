#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "my_library.h"
#include "errExit.h"

int main (int argc, char *argv[]) {

  if(argc<4){
    errExit("Wrong usage of Stampa program! text_to_write");
  }

    printf("Hi, I'm Stampa program!\n");

    printf("\n");

    for(int i=3;i<argc;i++)
      printf("%s ", argv[i]);

    printf("\n");

    return 0;
}
