#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>
#include <sys/shm.h>
#include <ctype.h>
#include "Memory.h"
#define SHM_SIZE 1024  /* make it a 1K shared memory segment */
key_t key;
int shmid;
char * data;
int mode;
int loop = 10;

int main(int argc, char *argv[]){
  allocateMemory();
  while(1){
    writeToMem(loop);
    loop++;
    if(loop > 98){
      loop = 10;
    }
    sleep(1);
  }
}
void allocateMemory(){
  /* make the key: */
  if ((key = 456456) == -1) {
      perror("ftok");
  }

  /* connect to (and possibly create) the segment: */
  if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
      perror("shmget");
  }
}

void writeToMem(int loop){
  char * sendData = (char*)malloc(8);

  sprintf(sendData,"%i%i%i",loop,loop,loop);
  printf("sendData %s\n",sendData);
  /* attach to the segment to get a pointer to it: */
  data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) {
      perror("shmat");
  }
  char* inData;
  int next[3];
  inData = data + sizeof (next);
  next[0] = sprintf(inData,"%c%c",sendData[0],sendData[1])+1;
  inData += next[0];
  next[1] = sprintf(inData,"%c%c",sendData[2],sendData[3])+1;
  inData += next[1];
  next[2] = sprintf(inData,"%c%c",sendData[4],sendData[5])+1;

  /* read or modify the segment, based on the command line: */
  memcpy(data, &next, sizeof (next));

  /* detach from the segment: */
  if (shmdt(data) == -1) {
      perror("shmdt");
      exit(1);
  }
};
