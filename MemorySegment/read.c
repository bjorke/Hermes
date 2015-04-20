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
  char *data, *ptr;
  int mode;
  char* shared_memory[6];
  int *p;

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
  /* attach to the segment to get a pointer to it: */
  data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) {
      perror("shmat");
      exit(1);
  }
  p = (int *)data;
  printf("sizeof data = %lu\n",sizeof (data));
  printf("data %s",data);
  ptr = data + 24;
  shared_memory[0] = ptr;
  ptr += *p++;
  shared_memory[1] = ptr;
  ptr += *p;
  shared_memory[2] = ptr;
  ptr += *p;
  shared_memory[3] = ptr;
  ptr += *p;
  shared_memory[4] = ptr;
  ptr += *p;
  shared_memory[5] = ptr;
  ptr += *p;

  printf ("0=%d\n", atoi(shared_memory[0]));
  printf ("1=%d\n", atoi(shared_memory[1]));
  printf ("2=%d\n", atoi(shared_memory[2]));
  printf ("3=%d\n", atoi(shared_memory[3]));
  printf ("4=%d\n", atoi(shared_memory[4]));
  printf ("5=%d\n", atoi(shared_memory[5]));

  /* detach from the segment: */
  if (shmdt(data) == -1) {
      perror("shmdt");
      exit(1);
  }
  return 0;
}
