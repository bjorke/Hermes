#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
int main ()
{
  key_t shm_key = ftok("./somefile", 'b');
  const int shm_size = 1024;
  int shm_id;
  char* shmaddr, *ptr;
  char* shared_memory[4];
  int *p;

  /* Allocate a shared memory segment. */
  shm_id = shmget (shm_key, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);

  /* Attach the shared memory segment. */
  shmaddr = (char*) shmat (shm_id, 0, 0);

  printf ("shared memory attached at address %p\n", shmaddr);

  /* Start to read data. */
  p = (int *)shmaddr;
  ptr = shmaddr + sizeof (int) * 2;
  shared_memory[0] = ptr;
  ptr += *p++;
  shared_memory[1] = ptr;
  ptr += *p;
  shared_memory[2] = ptr;
  ptr += *p;
  /*
  shared_memory[3] = ptr;
  ptr += *p;
  shared_memory[4] = ptr;
  */
  printf ("0=%s\n", shared_memory[0]);
  printf ("1=%s\n", shared_memory[1]);
  printf ("2=%s\n", shared_memory[2]);
  printf ("3=%s\n", shared_memory[3]);
  //printf ("4=%s\n", shared_memory[4]);

  /* Detach the shared memory segment. */
  shmdt (shmaddr);
  return 0;
}
