#include <EEPROM.h>
#include <Wire.h>
#include <Strela.h>
#include <LineBot.h>

LineBot ss;

void setup() {
  // put your setup code here, to run once:
  ss.begin();

  ss.sensors.attach(P6);
  ss.sensors.attach(P7);


  while (uDigitalRead(S1))
  {
    for (int i = 0; i < ss.sensors.count(); ++i)
      ss.sensors.setBlack(i);
  }
  
  while (uDigitalRead(S1))
  {
    for (int i = 0; i < ss.sensors.count(); ++i)
      ss.sensors.setWhite(i);
  }
  
  


  //  Serial.begin(9600);


  //  ss.speed(70);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < ss.sensors.count(); ++i)
  {
    Serial.print(ss.sensors.readPct(i));
    Serial.print(" ");
  }

  Serial.println();

  delay(500);

}
