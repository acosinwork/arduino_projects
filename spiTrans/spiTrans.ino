#include <SPI.h>

#define FET_COUNT 3
// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
byte v = 0;
byte keys[FET_COUNT];

void setup() {
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
}

void loop() {
  // go through the six channels of the digital pot:
  SPI.transfer(v++);
  digitalWrite(slaveSelectPin, HIGH);
  digitalWrite(slaveSelectPin, LOW);
v&=0b00011111;
  delay(600);
}

void octoWrite(int address, byte value) {
  keys[address] = value;
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  //for (int i = 0; i < FET_COUNT; ++i) {
  SPI.transfer(value);
  // }
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);
}
