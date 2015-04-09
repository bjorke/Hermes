#include <stdio.h>
#include "Checksum.c"

int main(int argc, char *argv[]){

  char str[30] = "12313212121211";
  int retVal = generate_verhoeff(str);
  printf("%d\n",retVal);
  char str2[30] = "123132121212115";
  printf("%d\n",validate_verhoeff(str2));
  return 0;
};
