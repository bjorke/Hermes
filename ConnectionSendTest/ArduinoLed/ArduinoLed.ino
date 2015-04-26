/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int incomingByte = 0;   // for incoming serial data
// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  Serial.begin(115200);     // opens serial port, sets data rate to 9600 bps
}

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if(incomingByte == '1'){
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    else if(incomingByte == '0'){
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    }
  }
}

