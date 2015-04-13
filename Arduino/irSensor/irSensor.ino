#define IRSENSOR 3
void setup(){
 Serial.begin(9600);
 pinMode(IRSENSOR, INPUT); // SENSOR IS AN INPUT TYPE
}

void loop(){
 Serial.println((analogRead(IRSENSOR) / 615) * 1024);
 delay(500);
}
