#include <stdio.h>
#include <Checksum.h>
#include <stdlib.h>
Checksum _Checksum;

#define sonar1TrigPin 13
#define sonar1EchoPin 12
#define sonar2TrigPin 13
#define sonar2EchoPin 12
#define sonar3TrigPin 13
#define sonar3EchoPin 12
#define ir1Port 0
#define ir2Port 1

void setup() {
  Serial.begin(9600);

  // set up sonar

  pinMode(sonar1TrigPin, OUTPUT);
  pinMode(sonar1EchoPin, INPUT);
  pinMode(sonar2TrigPin, OUTPUT);
  pinMode(sonar2EchoPin, INPUT);
  pinMode(sonar3TrigPin, OUTPUT);
  pinMode(sonar3EchoPin, INPUT);
}
int intLen(unsigned x) {
    if(x>=1000000000) return 10;
    if(x>=100000000) return 9;
    if(x>=10000000) return 8;
    if(x>=1000000) return 7;
    if(x>=100000) return 6;
    if(x>=10000) return 5;
    if(x>=1000) return 4;
    if(x>=100) return 3;
    if(x>=10) return 2;
    return 1;
}
void loop() {
  //sonar setup
  short int sonar1,sonar2,sonar3;
  sonar1 = sonar(sonar1EchoPin,sonar1TrigPin);
  sonar2 = sonar(sonar2EchoPin,sonar2TrigPin);
  sonar3 = sonar(sonar3EchoPin,sonar3TrigPin);
  //ir setup
  short int ir1, ir2;
  ir1 = sonar(sonar1EchoPin,sonar1TrigPin);
  ir2 = sonar(sonar1EchoPin,sonar1TrigPin);
  //Generate and send message
  Serial.println(generateMessage(sonar1,sonar2,sonar3,ir1,ir2));

  delay(1);
}
short int ir(short int irPort){
  short int ir;
  ir = analogRead (irPort);
  return ((short int)ir / 615) * 1024;
}
short int sonar(short int echo,short int trigger){
  short int duration;
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  short int val = (duration/2) / 29.1;
  short int valLen = intLen(val);
  //in order to achoure the length is 3
  char * returnVal=(char*)malloc(4);

  if(valLen >3 || valLen <0){
    sprintf(returnVal,"%d",val);
  }
  else{
  switch (valLen){
    case 3:
    sprintf(returnVal,"%d",val);
    break;
    case 2:
    sprintf(returnVal,"0%d",val);
    break;
    case 1:
    sprintf(returnVal,"00%d",val);
    break;
    case 0:
    sprintf(returnVal,"000%d",val);
    break;
  }
}
  return (int)strtol(returnVal, NULL, 10);
}

String generateMessage(short int sonar1,short int sonar2,short int sonar3,short int ir1,short int ir2){
  //checksum
  char * sumCheck;
  sumCheck = (char*)malloc(intLen(sonar1)+intLen(sonar2)+intLen(sonar3)+intLen(ir1)+intLen(ir2)+1);
  sprintf(sumCheck,"%d%d%d%d%d",sonar1,sonar2,sonar3,ir1,ir2);

  short int checkSum = _Checksum.generate_verhoeff(sumCheck);

  String sendString = "!";
  sendString += sonar1;
  sendString += ",";
  sendString += sonar2;
  sendString += ",";
  sendString += sonar3;
  sendString += ",";
  sendString += ir1;
  sendString += ",";
  sendString += ir2;
  sendString += ",";
  sendString += checkSum;
  free ((char*)checkSum);
  return sendString;
}
