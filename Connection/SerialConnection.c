#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <ctype.h>
#include "SerialConnection.h"
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/tty.usbmodem1411"/*UART NAME IN PROCESSOR*/
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
void openport(void);
void sendport(void);
void readport(void);
int fd=0, n;
static int cnt, size, s_cnt;
unsigned char *var;
struct termios oldtp, newtp;
char sendcmd1[10]="\0";
FILE *file;
bool runner = true;

void  readport(void){
  //file = fopen( "output.txt", "w+" );
  short int parsedValLoop = 0;
  char string[128];

  while (runner) {
    unsigned char buff;
    n = read(fd, &buff, 1);
    //	fcntl(fd,F_SETFL,0);
    if (n == -1) switch(errno) {
      case EAGAIN: /* sleep() */
      continue;
    }
    if (n ==0) {
      break;
    };
    //printf("%c",buff);
    if(buff == '!'){
      //printf("%s\n",string);
      short int sendInt[6];
      short int sendIntLoop = 0;
      for(int i = 0; i < sizeof(string);i++){
        if(string[i] == ','){
          //printf("string[i] %c\n",string[i]);
          sendIntLoop += 1;
        }
        else if(isdigit(string[i])){
          //printf("%i\n",string[i]- '0');
          sendInt[sendIntLoop] = string[i]- '0';;
        }
      }
      /*
      for(int i = 0; i < sizeof(sendInt)/sizeof(short int);i++){
        printf("%i\n",sendInt[i]);
      };
      */
      //sendDataToMem();

      //reset the array
      memset(string, 0, sizeof(string));

    }
    else{
      if(isdigit(buff) || buff == ','){
      //adds everything to the array
      strcat(string,(char*)&buff);
    }
  }


  }
  fclose (file);
}

void sendDataToMem(){
  int runner = 1;
  key_t shm_key = ftok("./somefile", 'b');
  const int shm_size = 1024;

  int shm_id;
  char* shmaddr, *ptr;
  int next[2];

  printf ("writer started.\n");
  while(runner == 1){

    /* Allocate a shared memory segment. */
    shm_id = shmget (shm_key, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);

    /* Attach the shared memory segment. */
    shmaddr = (char*) shmat (shm_id, 0, 0);

    printf ("shared memory attached at address %p\n", shmaddr);

    /* Start to write data. */
    ptr = shmaddr + sizeof (next);
    next[0] = sprintf (ptr, "mandy") + 1;
    ptr += next[0];
    next[1] = sprintf (ptr, "73453916") + 1;
    ptr += next[1];
    sprintf (ptr, "amarica");
    memcpy(shmaddr, &next, sizeof (next));
    printf ("writer ended.\n");

    /*calling the other process*/
    //system("./read.out");
    sleep(10);
  };
  /* Detach the shared memory segment. */
  shmdt (shmaddr);
  /* Deallocate the shared memory segment.*/
  shmctl (shm_id, IPC_RMID, 0);
}
void openport(void){

	 fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY |O_NDELAY );
	 printf("Connection open %d\n",fd);
   if (fd <0){
   	perror(MODEMDEVICE);

   }

   fcntl(fd,F_SETFL,0);
   tcgetattr(fd,&oldtp); /* save current serial port settings */
   bzero(&newtp, sizeof(newtp));

   newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

   newtp.c_iflag = IGNPAR | ICRNL;

   newtp.c_oflag = 0;
   newtp.c_lflag = ICANON;
   newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
   newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
   newtp.c_cc[VERASE]   = 0;     /* del */
   newtp.c_cc[VKILL]    = 0;     /* @ */
  // newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
   newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
   newtp.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
   //newtp.c_cc[VSWTC]    = 0;     /* '\0' */
   newtp.c_cc[VSTART]   = 0;     /* Ctrl-q */
   newtp.c_cc[VSTOP]    = 0;     /* Ctrl-s */
   newtp.c_cc[VSUSP]    = 0;     /* Ctrl-z */
   newtp.c_cc[VEOL]     = 0;     /* '\0' */
   newtp.c_cc[VREPRINT] = 0;     /* Ctrl-r */
   newtp.c_cc[VDISCARD] = 0;     /* Ctrl-u */
   newtp.c_cc[VWERASE]  = 0;     /* Ctrl-w */
   newtp.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
   newtp.c_cc[VEOL2]    = 0;     /* '\0' */


}

int main(int argc, char *argv[])    {

	openport();
	readport();
  return 0;
}
