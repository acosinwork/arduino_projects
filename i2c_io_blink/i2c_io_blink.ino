
#include <Wire.h>
enum IOcommand {
      WHO_AM_I
    , PIN_MODE_INPUT
    , PIN_MODE_OUTPUT
    , PIN_MODE_PULLUPsdf
    , DIGITAL_READ
    , DIGITAL_WRITE_HIGH
    , DIGITAL_WRITE_LOW
    , PWM_FREQ
    , ANALOG_WRITE
    , ANALOG_READ
    , CHANGE_I2C_ADDR
    , RESET_ME
};




byte addr = 42;

byte cmd[3];

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  // while (!Serial);             // Leonardo: wait for serial monitor
  cmd[0] = PWM_FREQ;
  cmd[1] = 0;
  cmd[2] = 255;
send();

  cmd[0] = ANALOG_WRITE;
  cmd[1] = 3;
  cmd[2] = 1;
  send();
}

void send() {
  Wire.beginTransmission(addr);
  Wire.write(cmd, 3);
  Wire.endTransmission();
}

byte x=0;

void loop()
{

cmd[2] = ++x;
  send();
  delay(100);
Serial.println(x);
  /*for (byte i = 0; i < 9; ++i) {

    cmd[1] = i;
    Wire.beginTransmission(addr);
    Wire.write(cmd, 3);
    Wire.endTransmission();
    // delay(10);
    uint16_t c, d;
    Wire.requestFrom(addr, 2);
    while (Wire.available())   // slave may send less than requested
    {
      c = Wire.read();    // receive a byte as character
      //  Serial.println(c);
      c <<= 8;
      c |= Wire.read();
      Serial.print(c);         // print the character
      Serial.print('\t');
    }
  }
  Serial.println();
  //  delay(100);
  */
}

