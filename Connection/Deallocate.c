#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
int main(int argc, char *argv[])    {
  key_t shm_key = ftok("./somefile", 'b');
  const int shm_size = 1024;
  int shm_id;
  shm_id = shmget (shm_key, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);

  /* Deallocate the shared memory segment.*/
  shmctl (shm_id, IPC_RMID, 0);

}
