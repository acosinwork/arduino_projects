#include <Wire.h>

#include <Strela.h>

//Strela strela;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:3
  Strela.begin();
//  Strela.motorConnection(true,false);

pinMode(6,OUTPUT);



}

void loop() {
  // put your main code here, to run repeatedly:
//analogWrite(6,20);
delay(2000);

  for (int i=-255; i<256; ++i)
{
  Strela.motors(i,0);
  delay(10);
}

  for (int i=-255; i<256; ++i)
{
  Strela.motors(0,i);
  delay(10);
}

}
