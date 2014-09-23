

#include <circleArea.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 1; i < 5; ++i ) {
    Serial.print("circle area with radius ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(circleArea(i));
  }
}

