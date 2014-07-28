#include <SPI.h>

#define LEFT   A1
#define RIGHT  A0

#define BUT0   5
#define BUT1   8

#define LED0   6
#define LED1   7

#define STROBE  10



enum flashingType
{
  FULL_LINE,
  DOTTS
}

bool led2 = false;

int channelBias[] = {
  511,
  511
}

flashingType status = FULL_LINE;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED1, OUTPUT);
  pinMode(LED0, OUTPUT);

  pinMode(STROBE, OUTPUT);

  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

}


unsigned int getChannelValue(int channelValue, byte channelNumber)
{
  channelValue = abs(channelValue - channelBias[channelNumber]);

  byte lastLed = map(channelValue, 0, 511, 0, 12);

  unsigned int data=0;

  switch ( status ) {
    case FULL_LINE:

      for (int i = 0; i <= lastLed; ++i) {
        data |= 1;
        data <<= 1;
      }

      break;
    case DOTTS:

      data |= 1;
      for (int i = 0; i <= lastLed; ++i) {
        data <<= 1;
      }
      break;      
  }

  return data;
}

unsigned long dataPrepare(int leftChannel, int rightChannel)
{

  unsigned long finalData = 0;

  for (byte i = 0; i < sizeOf(finalData)-1; ++i) {
    if (!((i = 7)||(i = 15)||(i = 23)||(i = 31))) {
      //TODO
    }

    finalData<<=1;

  }

  if (led2)
    finalData |= 1;

  return finalData;
}

void flashingLeds(unsigned long sendValue)
{
  SPI.transfer(sendValue);
  
  digitalWrite(STROBE, LOW);
  digitalWrite(STROBE, HIGH);  
}

