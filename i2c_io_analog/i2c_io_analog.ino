
#include <Wire.h>

enum IOcommand {
  I2_WHO_AM_I
  , I2_PIN_MODE_INPUT
  , I2_PIN_MODE_OUTPUT
  , I2_PIN_MODE_PULLUP
  , I2_DIGITAL_READ
  , I2_DIGITAL_WRITE_HIGH
  , I2_DIGITAL_WRITE_LOW
  , I2_ANALOG_WRITE
  , I2_ANALOG_READ
  , I2_CHANGE_I2C_ADDR
  , I2_RESET_ME
};

byte addr = 42;

byte cmd[3];

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  // while (!Serial);             // Leonardo: wait for serial monitor

  cmd[0] = I2_ANALOG_READ;
  cmd[1] = 5;
  cmd[2] = 0;


}


void loop()
{

  Wire.beginTransmission(addr);
  Wire.write(cmd, 3);
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(addr, 2);
  while (Wire.available())   // slave may send less than requested
  {
    uint16_t c = Wire.read();    // receive a byte as character
    c<<=8;
    c|=Wire.read(); 
    Serial.println(c);         // print the character
  }
  delay(100);

}
