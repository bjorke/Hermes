#include <Servo.h> 

#define MAX_SIGNAL 2075
#define MID_SIGNAL 1490
#define MIN_SIGNAL 1044

Servo steeringServo;
int pos = 0;    // variable to store the servo position 

Servo escServo;
int escpos = 0;    // variable to store the servo position 

int ch1; // Here's where we'll keep our channel values for receiver
int ch2;

int incomingByte = 0;   // for incoming serial data

void setup() {

  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  // for autonomous switch
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  steeringServo.attach(9);
  escServo.attach(3);

  Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {
  
  int autonomous = digitalRead(2); // switch on board
  
  if(autonomous) {
    
    digitalWrite(13, LOW); // switch off LED on board
    
    steeringServo.writeMicroseconds(random(1300, 1600));
    escServo.writeMicroseconds(random(1300, 1600));
    delay(500);
    
  } else {
    
      if (Serial.available() > 0) {
              // read the incoming byte:
              incomingByte = Serial.read();
 
              // say what you got:
              Serial.print("serial: ");
              Serial.println(incomingByte, DEC);
              
              steeringServo.writeMicroseconds(random(1500, 1600));
      }
    
      digitalWrite(13, HIGH); // switch on LED on board
      
      ch1 = pulseIn(6, HIGH, 25000); // each channel
      ch2 = pulseIn(7, HIGH, 25000);
      
      // this is shit, change it
      
      if(ch1 < 1300) { escServo.writeMicroseconds(MIN_SIGNAL); }
      else if(ch1 > 1800) { escServo.writeMicroseconds(MAX_SIGNAL); }
      else { escServo.writeMicroseconds(MID_SIGNAL); }
      
      //escServo.writeMicroseconds(ch1);
      steeringServo.writeMicroseconds(ch2);
    
    /*
      Serial.print("Channel 1:  "); // Print the value of 
      Serial.println(ch1);        // each channel
    
      Serial.print("Channel 2:  ");
      Serial.println(pos);
    
      Serial.println("_____________");
    */
      //delay(500); // I put this here just to make the terminal 
                  // window happier
  }
}
