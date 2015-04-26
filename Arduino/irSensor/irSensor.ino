int ir1 = 0;

void setup()
{
  Serial.begin(115200);               // starts the serial monitor
}

void loop(){

  Serial.println(ir(ir1));            // prints the value of the sensor to the serial monitor
  delay(100);                    // wait for this much time before printing next value

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
