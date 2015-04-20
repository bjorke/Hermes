#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int main(int argc, char *argv[])
{
    key_t key;
    int shmid;
    char * data;
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
    while(1){
        /* attach to the segment to get a pointer to it: */
        data = shmat(shmid, (void *)0, 0);
        if (data == (char *)(-1)) {
            perror("shmat");
            exit(1);
        }
        char* inData;
        time_t t;
        /* Intializes random number generator */
        srand((unsigned) time(&t));
        int next[6];
        inData = data + sizeof (next);
        //next[0] = sprintf(inData,"%i\n",rand() % 100 + 1);
        next[0] = sprintf(inData,"000")+1;
        inData += next[0];
        //next[1] = sprintf(inData,"%i\n",rand() % 100 + 1);
        next[1] = sprintf(inData,"123")+1;
        inData += next[1];
        next[2] = sprintf(inData,"234")+1;
        inData += next[2];
        next[3] = sprintf(inData,"345")+1;
        inData += next[3];
        next[4] = sprintf(inData,"456")+1;
        inData += next[4];
        next[5] = sprintf(inData,"567")+1;
        inData += next[5];
        /* read or modify the segment, based on the command line: */
        memcpy(data, &next, sizeof (next));


        /* detach from the segment: */
        if (shmdt(data) == -1) {
            perror("shmdt");
            exit(1);
        }
        sleep(1);
    }

    return 0;
}
