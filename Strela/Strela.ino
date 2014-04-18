#include <Wire.h>

#include <Strela.h>

//Strela strela;

void setup() {
  // put your setup code here, to run once:3
  Strela.begin();
  Strela.motorConnection(true,false);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=-100; i<100; ++i)
{
  Strela.motors(-i,i);
  delay(100);
}
}
