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
#define BAUDRATE B115200
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
char* serialport = "/dev/tty.usbmodem1411";

int openport(char* serialport, int baud);
void sendport(void);
void readport(void);
int serialport_read_until(int fd, char* buf, char until);

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
    buff = malloc(30);
    n = serialport_read_until(fd, &buff, 1);
    //	fcntl(fd,F_SETFL,0);
    if (n == -1) switch(errno) {
      case EAGAIN: /* sleep() */
      continue;
    }
    if (n ==0) {
      break;
    };
    printf("%c",buff);
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
          sendInt[sendIntLoop] = string[i]- '0';
        }
      }
      sendDataToMem(sendInt);

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
const int SHM_SIZE = 1024;
char * data;
int shmid;

void allocateMemorySpace(){
  key_t key;
  /* make the key: */
  if ((key = 123123) == -1) {
      perror("ftok");
  }
  /* connect to (and possibly create) the segment: */
  if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
      perror("shmget");
  }
}

void deallocateMemorySpace(){
  /* Deallocate the shared memory segment.*/
  shmctl (shmid, IPC_RMID, 0);
}

void sendDataToMem(short int sendInt[6]){

  char* data;
  int next[6];


  /* attach to the segment to get a pointer to it: */
  data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) {
      perror("shmat");
  }
  char* inData;
  /* Start to write data. */

  next[0] = sprintf(inData,"%i",sendInt[0])+1;
  inData += next[0];
  next[1] = sprintf(inData,"%i",sendInt[1])+1;
  inData += next[1];
  next[2] = sprintf(inData,"%i",sendInt[2])+1;
  inData += next[2];
  next[3] = sprintf(inData,"%i",sendInt[3])+1;
  inData += next[3];
  next[4] = sprintf(inData,"%i",sendInt[4])+1;
  inData += next[4];
  next[5] = sprintf(inData,"%i",sendInt[5])+1;
  inData += next[5];

  /* read or modify the segment, based on the command line: */
  memcpy(data, &next, sizeof (next));


  printf ("next %lu\n",sizeof (next));

  /* detach from the segment: */
  if (shmdt(data) == -1) {
      perror("shmdt");
  }

}
int openport(char* serialport, int baud){

    printf("serialport.. %s\n",serialport);
    printf("baud.. %i\n",baud);

    struct termios toptions;
    int fd;

    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;

    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}
int serialport_read_until(int fd, char* buf, char until){
    char b[1];
    int i=0;
    do {
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0]; i++;
    } while( b[0] != until );

    buf[i] = 0;  // null terminate the string
    return 0;
}
int main(int argc, char *argv[])    {
  allocateMemorySpace();
  fd = openport(serialport,115200);

	readport();
  return 0;
}
