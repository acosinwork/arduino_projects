/* "Системы модули и компоненты" ("СМК"). 2017. Москва.
Библиотека MBee-Arduino.
Распространяется свободно. Надеемся, что программные продукты, созданные
с помощью данной библиотеки будут полезными, однако никакие гарантии, явные или
подразумеваемые не предоставляются. */

#include "MBee.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "HardwareSerial.h"


/*********************************************************************
 * Public методы класса MBee.
 *********************************************************************/
void MBee::begin(Stream &serial)
{
  _serial = &serial;
} 
 
void MBee::setSerial(Stream &serial)
{
  _serial = &serial;
}

bool MBee::available()
{
  return _serial->available();
}

uint8_t MBee::read()
{
  return _serial->read();
}

void MBee::write(uint8_t val)
{
  _serial->write(val);
}




