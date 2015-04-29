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

    while (Serial.available()) {
        int ch = Serial.read();
        if (ch == -1) {
            // Handle error
        }
        else if (ch == delimiter) {
            break;
        }
        else {
            intData += (char) ch;
        }
    }

    int intLength = intData.length() + 1;
    intData.toCharArray(intBuffer, intLength);


    // Reinitialize intData for use next time around the loop
    intData = "";

    // Convert ASCII-encoded integer to an int
    int i = atoi(intBuffer);
    
    if(i != 0) { Serial.println(i); }
    
    

  } else {
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
}




