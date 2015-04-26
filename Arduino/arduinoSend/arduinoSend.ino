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
  Serial.begin(115200);

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
    if(x>=1) return 1;
    return 0;
}
void loop() {
  //sonar setup
  short int sonar1,sonar2,sonar3,sonar4,sonar5;
  sonar1 = sonar(sonar1EchoPin,sonar1TrigPin);
  sonar2 = sonar(sonar2EchoPin,sonar2TrigPin);
  sonar3 = sonar(sonar3EchoPin,sonar3TrigPin);
  sonar4 = sonar(sonar4EchoPin,sonar4TrigPin);
  sonar5 = sonar(sonar5EchoPin,sonar5TrigPin);
  //Generate and send message
  generateMessage(sonar1,sonar2,sonar3,sonar4,sonar5);

  delay(10); //delay between messages
}

short int sonar(short int echo,short int trigger){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  short int duration = pulseIn(echo, HIGH);
  int delay = 26500;
  int sleepTime = delay - duration;
  delayMicroseconds(sleepTime);
  if(duration > delay){
    duration = delay;
  }

  short int retVal = (duration/2) / 29.1;
  if(retVal > 0){
    return  retVal;
  }
  return 0;
}
short int ir(int irPin){
  short int distance = 0;
  int val = analogRead(irPin);       // reads the value of the sharp sensor
  distance = 2076/(val - 11);

  if (distance<=0){
    distance = 100;
  }
  else if(distance >35){
    distance = 100;
  }
  return distance;
}

void generateMessage(short int sonar1,short int sonar2,short int sonar3,short int sonar4,short int sonar5){
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
    if(sendString.length() > 12){
      Serial.println(sendString);
    }
}
