#include <Servo.h>

#define MAX_SIGNAL 2075
#define MID_SIGNAL 1490
#define MIN_SIGNAL 1044

//
char intBuffer[12];
String intData = "";
int delimiter = (int) '\n';
//

// define lights pins
int reverseLights = 12;
int turnLeft = 11;
int turnRight = 4;
int frontLights = 5;
int brakeLights = 10;
int rearLights = A0;
int police1 = A1;
int police2 = A2;

Servo steeringServo;
int pos = 0;    // variable to store the servo position

Servo escServo;
int escpos = 0;    // variable to store the servo position

int ch1; // Here's where we'll keep our channel values for receiver
int ch2;

int incomingByte = 0;   // for incoming serial data

void setup() {

  pinMode(8, INPUT); // receiver channel 1
  pinMode(6, INPUT); // receiver channel 2

  // for autonomous switch
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  // controls
  steeringServo.attach(9);
  escServo.attach(3);

  // lights
  pinMode(reverseLights, OUTPUT);
  pinMode(turnLeft, OUTPUT);
  pinMode(turnRight, OUTPUT);
  pinMode(frontLights, OUTPUT);
  pinMode(brakeLights, OUTPUT);

  Serial.begin(115200); // Pour a bowl of Serial

}


// normal function of the lights
void lights(int autonomous) {

}



void loop() {

  int autonomous = digitalRead(2);

  if(autonomous) {
    digitalWrite(13, LOW);
    digitalWrite(brakeLights, LOW);

    // RANDOM BEHAVIOUR - DEMO
    /*
    steeringServo.writeMicroseconds(random(1300, 1600));
    escServo.writeMicroseconds(random(1300, 1600));
    delay(500);
    */

    if (Serial.available() > 0) {
        int ch = Serial.read();
        if(ch == '!'){
          intData = "!";
        }
        else if(intData.length() > 4){
          int * cleanInt = cleanData(intData);
          int steering = getSteering(*(cleanInt));
          if(checkSpeed(*(cleanInt + 1)) && steering > 0){
            drive(steering,*(cleanInt + 1));
          }
          else{
            //if there's anything wrong we set the wheels straigth and set the motor to 1500 I.E off
            drive(1385,1500);
          }
          intData = "";
        }
        else {
            intData += (char) ch;
        }
    }

  }
  else {
    controller();
  }
}
bool checkSpeed(int speed){
  int minVal = 1000;
  int maxVal = 2000;
  if(speed > minVal && speed < maxVal){
    return true;
  }

  return false;
}
int getSteering(int steering){
  /*
  50 is our zero base, 1385 is center
  larger is rigth, 1876 is full rigth
  smaller is left, 857 is full left
  */
  int returnVal = 0;
  switch(steering){
    case 30 :
      returnVal = 857;
      break;
    case 40 :
      returnVal = 1140;
      break;
    case 50 :
      returnVal = 1385;
      break;
    case 60 :
      returnVal = 1630;
      break;
    case 70 :
      returnVal = 1876;
      break;

  }
  return returnVal;

}
int * cleanData(String inData){
  if(intData.charAt(0) == '!'){
    //Serial.println(inData);
    static int returnVal[2];
    //Steering
    char * ec1 = (char*)malloc(2);
    //Speed
    char * ec2 = (char*)malloc(2);

    sprintf(ec1,"%c%c",intData.charAt(1),intData.charAt(2));
    sprintf(ec2,"%c%c",intData.charAt(3),intData.charAt(4));

    returnVal[0] =  atoi(ec1);
    returnVal[1] =  (atoi(ec2)*10)+1000;
    return returnVal;
  }
  return NULL;
}
void drive(int steering,int speed){

  if(steering > 850 && steering < 1880 && speed > 999 && speed < 2001) {
    /*
    Serial.println("steering");
    Serial.println(steering);
    Serial.println("speed");
    Serial.println(speed);
    */
    steeringServo.writeMicroseconds(steering);
    escServo.writeMicroseconds(speed);
  }
  else{
    steeringServo.writeMicroseconds(1385);
    escServo.writeMicroseconds(1500);
  }
}
void controller(){
    digitalWrite(13, HIGH);
    digitalWrite(brakeLights, HIGH);


    ch1 = pulseIn(8, HIGH, 25000); // each channel
    ch2 = pulseIn(6, HIGH, 25000);

    // this is shit, change it

    if(ch1 < 1150) { escServo.writeMicroseconds(MIN_SIGNAL); }
    else if(ch1 > 2050) { escServo.writeMicroseconds(MAX_SIGNAL); }
    else if(ch1 < 1680 && ch1 > 1530) { escServo.writeMicroseconds(MID_SIGNAL); }
    else { escServo.writeMicroseconds(ch1); }


    // for lights
    if(ch1 < 1500) {
      digitalWrite(reverseLights, HIGH);
    } else if(ch1 > 1800) {
      digitalWrite(frontLights, HIGH);
    } else {
      digitalWrite(reverseLights, LOW);
      digitalWrite(frontLights, LOW);
    }

    if(ch2 < 1300) {
      digitalWrite(turnLeft, HIGH);
    } else if(ch2 > 1500) {
      digitalWrite(turnRight, HIGH);
      //digitalWrite(reverseLights, HIGH);
    } else {
      digitalWrite(turnLeft, LOW);
      digitalWrite(turnRight, LOW);
      //digitalWrite(reverseLights, LOW);
    }

          /*
    if(ch1 < 1600 && ch1 > 1500) {
       escServo.writeMicroseconds(MID_SIGNAL);
    } else if(ch1 > 1800) {
        escServo.writeMicroseconds(MIN_SIGNAL);
    } else if(ch1 < 1200) {
        escServo.writeMicroseconds(MAX_SIGNAL);
    }else {
        escServo.writeMicroseconds(ch1);
    }*/


    //escServo.writeMicroseconds(ch1);
    steeringServo.writeMicroseconds(ch2);

    // write to serial
    Serial.print("Channel 1:  ");
    Serial.println(ch1);

    Serial.print("Channel 2:  ");
    Serial.println(ch2);

    Serial.println("_____________");
    //delay(500); // I put this here just to make the terminal
                // window happier


}
