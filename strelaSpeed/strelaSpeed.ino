#include "TimerOne.h"


byte pwmFreq = 0;
byte motorSpeed1 = 0;
byte motorSpeed2 = 0;

void setup()
{
  Timer1.initialize(4);
  Timer1.attachInterrupt( timerIsr );

  for (int pin = 4; pin < 8; ++pin) {
    pinMode(pin, OUTPUT);
  }

  pinMode(4, OUTPUT);
  DDRE |= _BV(2);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{

for (int i=0; i<255; ++i)
{
  motorSpeed1=i;
  motorSpeed2=255-i;
  delay(100);
}

}

void timerIsr() //Прерывание таймера. Здесь мы делаем PWM на 1-м таймере
{
  if (motorSpeed1 < pwmFreq)
    digitalWrite(9, LOW);
  else
    digitalWrite(9, HIGH);
    
  if (motorSpeed2 < pwmFreq)
    digitalWrite(10, LOW);
  else
    digitalWrite(10, HIGH);

  ++pwmFreq;
}
