#include <QuadDisplay.h>
#include <School.h>

void setup() {
  // put your setup code here, to run once:
analogWrite(3, 127);

Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

static unsigned long time = millis();
if (millis()-time>=200)
{
  analogWrite(3, map(analogRead(Z1), 0, 1023, 0, 255));
}
float value = voltageRead(A1);
Serial.println(value);

//displayFloat(W3_UP, value, 2);
//delay(1);

}


