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

int main(int argc, char *argv[]){
  allocateMemory();
  int loop = 0;
  while(1){
    writeToMem(loop);
    sleep(1);
    loop++;
    if(loop > 1){
      loop = 0;
      }
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
  char * sendData = (char*)malloc(30);

  if(loop == 0){
    sprintf(sendData,"000000000000000");
  }
  else{
    sprintf(sendData,"001000000000000");
  }
  /* attach to the segment to get a pointer to it: */
  data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) {
      perror("shmat");
  }
  char* inData;
  /* Intializes random number generator */
  int next[6];
  inData = data + sizeof (next);
  next[0] = sprintf(inData,"%c%c%c",sendData[0],sendData[1],sendData[2])+1;
  inData += next[0];
  next[1] = sprintf(inData,"%c%c%c",sendData[3],sendData[4],sendData[5])+1;
  inData += next[1];
  next[2] = sprintf(inData,"%c%c%c",sendData[6],sendData[7],sendData[8])+1;
  inData += next[2];
  next[3] = sprintf(inData,"%c%c%c",sendData[9],sendData[10],sendData[11])+1;
  inData += next[3];
  next[4] = sprintf(inData,"%c%c%c",sendData[12],sendData[13],sendData[14])+1;
  inData += next[4];
  /*
  next[5] = sprintf(inData,"%c%c%c",sendData[15],sendData[16],sendData[17])+1;
  inData += next[5];
  */
  /* read or modify the segment, based on the command line: */
  memcpy(data, &next, sizeof (next));


  /* detach from the segment: */
  if (shmdt(data) == -1) {
      perror("shmdt");
      exit(1);
  }
};
