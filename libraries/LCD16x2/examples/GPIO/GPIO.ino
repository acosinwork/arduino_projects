#include <LCD16x2.h>
#include <Wire.h>

LCD16x2 lcd;

void setup(){

  Wire.begin();
  
  // Set GPIO1 as OUTPUT
  lcd.pinMode(lcd.GPIO1, OUTPUT);
  
  // Set GPIO6 as INPUT
  lcd.pinMode(lcd.GPIO6, INPUT);
  
  // Turn on pullup
  lcd.digitalWrite(lcd.GPIO6, HIGH);
}
void loop(){

  // Check if GPIO6 is HIGH or LOW
  if(lcd.digitalRead(lcd.GPIO6) == HIGH)
    lcd.digitalWrite(lcd.GPIO1, HIGH);
  else
    lcd.digitalWrite(lcd.GPIO1, LOW);
    
  delay(100);
}
