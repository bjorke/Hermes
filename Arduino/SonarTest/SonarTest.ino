#include <stdio.h>
#include <stdlib.h>

#define sonar1TrigPin 13
#define sonar1EchoPin 12

void setup() {
  Serial.begin(115200);

  pinMode(sonar1TrigPin, OUTPUT);
  pinMode(sonar1EchoPin, INPUT);
}

void loop() {
  sonar(sonar1EchoPin,sonar1TrigPin);

  delay(500);

}

short int sonar(short int echo,short int trigger){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  unsigned long duration = pulseIn(echo, HIGH);
  String dur1 = "duration = ";
  dur1 += duration;
  Serial.println(dur1);

  int delay = 26500;
  int sleepTime = delay - duration;
  delayMicroseconds(sleepTime);
  if(duration > delay){
    duration = delay;
  }

  short int retVal = (duration/2) / 29.1;
  String del = "Delay = ";
  del += sleepTime;
  Serial.println(del);
  String leng1 = "length = ";
  leng1 += retVal;
  Serial.println(leng1);
  if(retVal > 0){
    return  retVal;
  }
  else{
    return 0;
  }
}

