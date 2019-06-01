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
    printf("Hi, I'm ClientExec program!\n");
    return 0;
}
