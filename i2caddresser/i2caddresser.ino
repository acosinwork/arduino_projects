#include <Wire.h>
byte addr = 42;
byte address = 10;
byte address1 = 42;
byte error;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(address);
  error = Wire.endTransmission();

  if (error == 0) {
    Serial.println(address);
    Wire.requestFrom(address, 4);    // request 6 bytes from slave device #2
    while (!Wire.available());
    byte msg[5];
    uint32_t c = 0;
    addr++;
//    Serial.println(addr);
    for (int i = 0; i < 4; ++i) // slave may send less than requested
    {
      c <<= 8;
      msg[i] = Wire.read();
      c |= msg[i];   // receive a byte as character
    }
    Serial.println(c);
    delay(100);
    msg[4] = addr;
    Wire.beginTransmission(address);
    Wire.write(msg, 5);
    Wire.endTransmission();
    delay(100);

  } else {
    Wire.beginTransmission(address1++);
    error = Wire.endTransmission();
    if (error == 0)
      Serial.println(address1);
  };
}
