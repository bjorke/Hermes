#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int main(int argc, char *argv[])    {
  key_t key;
  int shmid;
  char *data;
  int mode;

  /* make the key: */
  if ((key = 123123) == -1) {
      perror("ftok");
      exit(1);
  }
  /* connect to (and possibly create) the segment: */
  if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
      perror("shmget");
      exit(1);
  }
  /* Deallocate the shared memory segment.*/
  shmctl (shmid, IPC_RMID, 0);

}
