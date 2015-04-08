#include <stdio.h>
#include <Checksum.h>
Checksum myChecksum;

const char str[30] = "123132121121211123";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int retVal = myChecksum.generate_verhoeff(str);
  Serial.println(retVal);
  delay(500);
}
