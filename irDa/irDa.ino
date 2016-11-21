#define YAXIS_1      A0
#define XAXIS_1      A1
#define YAXIS_2      A2
#define XAXIS_2      A3
#define MANIPULATE   6

#define BYTE_COUNT   4

#include <TimerOne.h>

int transmitInterval = 20;

uint16_t sendByteCounter = 0;

unsigned long time;

byte signals[] = {
  YAXIS_1, XAXIS_1,
  YAXIS_2, XAXIS_2
};


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
    byte modulo = sendByteCounter % BYTE_COUNT;
    byte sendVal = lowByte(analogRead(signals[modulo]) >> 4);

    sendVal |= (modulo << 6);

    Serial.write(sendVal);//, BIN);//sendVal);

    Serial.flush();
    time = millis();
    ++sendByteCounter;
  }
}

void timerIsr()
{
  if ((PIND & _BV(MANIPULATE)) == _BV(MANIPULATE))
    PORTD &= ~_BV(MANIPULATE); //pin 5 LOW
  else
    PORTD |= _BV(MANIPULATE); //pin 5 HIGH
}

