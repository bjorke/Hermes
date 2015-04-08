#include <stdio.h>
#include <Checksum.h>
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

void loop() {
  //sonar setup
  short int sonar1,sonar2,sonar3;
  sonar1 = sonar(sonar1EchoPin,sonar1TrigPin);
  sonar2 = sonar(sonar2EchoPin,sonar2TrigPin);
  sonar3 = sonar(sonar3EchoPin,sonar3TrigPin);
  //ir setup
  short int ir1, ir2;
  ir1 = ir(ir1Port);
  ir2 = ir(ir2Port);
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
  return (duration/2) / 29.1;
}
String generateMessage(short int sonar1,short int sonar2,short int sonar3,short int ir1,short int ir2){
  //checksum
  char sumCheck;
  sumCheck = sonar1;
  sumCheck += sonar2;
  sumCheck += sonar3;
  sumCheck += ir1;
  sumCheck += ir2;
  short int checkSum = _Checksum.generate_verhoeff(&sumCheck);

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
  return sendString;
}
