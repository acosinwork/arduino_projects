#include <TimerOne.h>
#include <SPI.h>

void SPItoMIDIInit()
{
  Timer1.initialize(50000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  SPI.begin();
}

void timerIsr()
{
  
  byte buttons = SPI.transfer(sendWal); // там и кнопки и индикация
  
  //UI_ReadWRite(todo);
  
}

/*
byte UI_ReadWRite(byte sendWal)
{
  return SPI.transfer(sendWal);
}

*/
