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

int main(){
  long int a=0;

  for(int i=0;i<10;i++){
    a=(long int)time(NULL);
    printf("\ntempo %i : %ld",i,a);
    a=(a*10)+1;
    printf("\nTempO %i : %ld",i,a);

    sleep(1);
  }

  return 0;
}
