#include <QuadDisplay.h>

void setup()
{
  
  pinMode(3,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  
 
}

void loop()
{
  digitalWrite(3,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  
 
    displayFloat(4, ((float)millis()/1000), true);
    delay(10);
}
