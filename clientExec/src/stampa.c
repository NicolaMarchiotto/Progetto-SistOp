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
    printf("\nWrong usage! id, key_service, text_to_write\n");
    exit(0);
  }

    printf("Hi, I'm Stampa program!\n\n");

    for(int i=3;i<argc;i++)
      printf("%s ", argv[i]);

    printf("\n");

    return 0;
}
