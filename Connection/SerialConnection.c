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
#include <sys/stat.h>
#include <unistd.h>

void usage(void);
int serialport_init(char* serialport, int baud);
int serialport_read_until(int fd, char* buf, char until);
int shareMem();

char* serialport = "/dev/tty.usbmodem1411";

int main(int argc, char *argv[])
{
    int fd = 0;
    int baudrate = B9600;  // default
    char buf[256];
    int rc,n;
    int option_index = 0, opt;

    fd = serialport_init(serialport,9600);

    serialport_read_until(fd, buf, '\n');
    printf("read: %s\n",buf);

    shareMem();

    exit(EXIT_SUCCESS);
} // end main
int shareMem(){
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

    return 0;
}
int serialport_read_until(int fd, char* buf, char until)
{
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

int serialport_init(char* serialport, int baud)
{

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
