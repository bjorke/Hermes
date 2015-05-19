// This test version is meant for arduinos with only one hardware serial port
// and therefor uses SoftwareSerial for connecting to the 9DOF 
// (which unfortunately gives garbage data when we use a higher baudrate than 19200)
// On the Arduino Mega a hardware serial port will communicate with the 9DOF instead

#include <SoftwareSerial.h>
//choose digital pin numbers here
SoftwareSerial softSerial(2, 3);
String buff;
 
void setup() {
  Serial.begin(115200);
  //we can't go higher than 19200 with software serial
  softSerial.begin(19200);
}
 
void loop() {
  if(softSerial.available()){
    buff = "";
    while(softSerial.available()){
      char in = (char)softSerial.read();
      buff += in;
    }
    parseData(buff);
  }
 
  if(Serial.available()){
    while(Serial.available()){
      softSerial.write(Serial.read());
    }
  }
}
 
void parseData(String s){
   
  union byte2float{
  byte bval[4];
  float fval;
}
b2f;
   
  float yaw;
 
  byte b[13];
  s.getBytes(b,13);
    
  b2f.bval[0] = b[0];
  b2f.bval[1] = b[1];
  b2f.bval[2] = b[2];
  b2f.bval[3] = b[3];
  yaw = b2f.fval;
  
  Serial.print("Yaw: ");
  Serial.println(yaw);
}
  

