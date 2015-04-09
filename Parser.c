#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
  short int parsedVal[4];
  char * string = "!123,456,789,0";

  short int parsedValLoop = 0;
  short int loopPlaceHolder = 0;
  if(string[0] == '!'){
    char placeHolder[12];
    for(short int i = 1; i < strlen(string); i++){
      if(string[i] == ','){
        parsedVal[parsedValLoop] = atoi(placeHolder);
        parsedValLoop += 1;
        loopPlaceHolder = 0;
      }
      else{
        placeHolder[loopPlaceHolder] = string[i];
        loopPlaceHolder++;
      };
    };
  };

  for(short int i = 0; i < sizeof(parsedVal)/sizeof(parsedVal[0]) ; i++){
    printf("%i\n",parsedVal[i]);
  };

  return 0;
}
