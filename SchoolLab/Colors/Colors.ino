
#include <School.h>

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print(map(analogRead(A2),0,1023,0,255));
Serial.print(' ');
Serial.print(map(analogRead(A3),0,1023,0,255));
Serial.print(' ');
Serial.print(map(analogRead(A4),0,1023,0,255));
Serial.println(' ');
delay(1);
}


