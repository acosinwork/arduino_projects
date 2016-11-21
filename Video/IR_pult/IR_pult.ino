#define XAXIS_1      A1
#define XAXIS_2      A3
#define MANIPULATE 6

#include <TimerOne.h>

int transmitInterval = 25;
unsigned long time;
byte yVal;
byte xVal;

byte counter = 0;

byte sendVal;


void setup() {
  pinMode(MANIPULATE, OUTPUT);

  Timer1.initialize(14); // 72000 Hz, 14 microseconds, 36 kHz*2
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  // put your setup code here, to run once:
  time = millis();
  Serial.begin(1200);
}

void loop() {
  // put your main code here, to run repeatedly:

  if ((millis() - time) > transmitInterval)
  {

    if (counter & 1)
      Serial.write(yVal);
    else
      Serial.write(xVal);

    Serial.flush();
    time = millis();
    ++counter;
  }

  yVal = 0b10000000 | (analogRead(XAXIS_1) >> 3);
  xVal = 0b01111111 & (analogRead(XAXIS_2) >> 3);

}

void timerIsr()
{
  if ((PIND & _BV(MANIPULATE)) == _BV(MANIPULATE))
    PORTD &= ~_BV(MANIPULATE); //pin 5 LOW
  else
    PORTD |= _BV(MANIPULATE); //pin 5 HIGH
}

