#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
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

void  readport(void){
	unsigned char buff;
	file = fopen( "output.txt", "w+" );
  int looper = 0;
  while (looper < 1000) {
    n = read(fd, &buff, 1);
    //	fcntl(fd,F_SETFL,0);
    if (n == -1) switch(errno) {
    case EAGAIN: /* sleep() */
      continue;

    }
    if (n ==0) break;
    fputc(buff, file);
    printf("%d \"%c\"\n", n,buff);
    looper++;
  }
  fclose (file);
}
void openport(void){

	 fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY |O_NDELAY );
	 printf("oviya :%d\n",fd);
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
