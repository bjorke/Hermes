#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  int i;
  time_t t;
  /* Intializes random number generator */
  srand((unsigned) time(&t));
  char test[128];
  sprintf(test,"%i",rand() % 100);
  printf("%s\n", test);

 return(0);
}
