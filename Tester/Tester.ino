char *startDouble = "DOUBLE";
double test[10] = {123.35546,2.1,4,5,5.5,72,21,52.41,12,25};
char *startInt = "INT";
int test1[10] = {524,6987,54,59,456,5468,52,214,978};
void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.println(startDouble);
  for (int i = 0; i < 10; i = i + 1) {
  Serial.println(test[i]);
  };
  
  Serial.println(startInt);
  for (int i = 0; i < 10; i = i + 1) {
  Serial.println(test1[i]);
  };
  
  delay(10);
}
