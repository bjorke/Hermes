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
#define sonar4TrigPin 13
#define sonar4EchoPin 12
#define sonar5TrigPin 13
#define sonar5EchoPin 12

void setup() {
  Serial.begin(9600);

  // set up sonar

  pinMode(sonar1TrigPin, OUTPUT);
  pinMode(sonar1EchoPin, INPUT);
  pinMode(sonar2TrigPin, OUTPUT);
  pinMode(sonar2EchoPin, INPUT);
  pinMode(sonar3TrigPin, OUTPUT);
  pinMode(sonar3EchoPin, INPUT);
  pinMode(sonar4TrigPin, OUTPUT);
  pinMode(sonar4EchoPin, INPUT);
  pinMode(sonar5TrigPin, OUTPUT);
  pinMode(sonar5EchoPin, INPUT);
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
  short int sonar1,sonar2,sonar3,sonar4,sonar5;
  sonar1 = sonar(sonar1EchoPin,sonar1TrigPin);
  sonar2 = sonar1 * 0.97;
  sonar3 = sonar1 * 0.88;
  sonar4 = sonar1 * 1.12;
  sonar5 = sonar1 * 1.10;
  /*
  sonar2 = sonar(sonar2EchoPin,sonar2TrigPin);
  sonar3 = sonar(sonar3EchoPin,sonar3TrigPin);
  sonar4 = sonar(sonar4EchoPin,sonar4TrigPin);
  sonar5 = sonar(sonar5EchoPin,sonar5TrigPin);
  */
  //Generate and send message
  Serial.println(generateMessage(sonar1,sonar2,sonar3,sonar4,sonar5));

  delay(1);
}

short int sonar(short int echo,short int trigger){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  short int duration = pulseIn(echo, HIGH);
  return  (duration/2) / 29.1;
}

String generateMessage(short int sonar1,short int sonar2,short int sonar3,short int sonar4,short int sonar5){
  //checksum
  char * sumCheck;
  sumCheck = (char*)malloc(intLen(sonar1)+intLen(sonar2)+intLen(sonar3)+intLen(sonar4)+intLen(sonar5)+1);
  sprintf(sumCheck,"%d%d%d%d%d",sonar1,sonar2,sonar3,sonar4,sonar5);

  short int checkSum = _Checksum.generate_verhoeff(sumCheck);

  String sendString = "!";
  sendString += sonar1;
  sendString += ",";
  sendString += sonar2;
  sendString += ",";
  sendString += sonar3;
  sendString += ",";
  sendString += sonar4;
  sendString += ",";
  sendString += sonar5;
  sendString += ",";
  sendString += checkSum;
  return sendString;
}
