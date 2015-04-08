#include <stdio.h>
#include <Checksum.h>
Checksum _Checksum;

#define sonar1TrigPin 13
#define sonar1EchoPin 12
#define sonar2TrigPin 13
#define sonar2EchoPin 12
#define sonar3TrigPin 13
#define sonar3EchoPin 12

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

void loop() {
  short int sonar1,sonar2,sonar3;
  sonar1 = sonar(sonar1EchoPin,sonar1TrigPin);
  sonar2 = sonar(sonar2EchoPin,sonar2TrigPin);
  sonar3 = sonar(sonar3EchoPin,sonar3TrigPin);

  //#####################
  //Only for testing
  sonar2 = sonar2 *1.564;
  sonar3 = sonar3 *0.654;
  //#####################

  char sumCheck;
  sumCheck = sonar1;
  sumCheck += sonar2;
  sumCheck += sonar3;
  short int checkSum = _Checksum.generate_verhoeff(&sumCheck);

  String sendString = "!";
  sendString += sonar1;
  sendString += ",";
  sendString += sonar2;
  sendString += ",";
  sendString += sonar3;
  sendString += ",";
  sendString += checkSum;
  Serial.println(sendString);

  delay(1);
}

short int sonar(short int echo,short int trigger){
  short int duration,distance;

  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;

  return distance;
}
