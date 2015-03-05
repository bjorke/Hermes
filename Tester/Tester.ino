int test = 1;
int bytesSent = 0;
void setup() {
  Serial.begin(9600);
}
void loop() {
  bytesSent = Serial.write(test);
  Serial.println(bytesSent);

  delay(500);
}
