#include <stdio.h>
#include <Checksum.h>
Checksum _Checksum;

#define trigPin 13
#define echoPin 12


void setup() {
  Serial.begin(9600);

  // set up sonar

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);



}

void loop() {
  short int duration,distance,distance1,distance2;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  distance1 = distance *1.564;
  distance2 = distance *0.654;

  char sumCheck;
  sumCheck = distance;
  short int checkSum = _Checksum.generate_verhoeff(&sumCheck);

  String sendString = "!";
  sendString += distance;
  sendString += ",";
  sendString += distance1;
  sendString += ",";
  sendString += distance2;
  sendString += ",";
  sendString += checkSum;
  Serial.println(sendString);

  delay(1);
}
