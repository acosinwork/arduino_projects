#include <SPI.h>

#define LEFT   A1
#define RIGHT  A0

#define BUT0   5
#define BUT1   8

#define LED0   6
#define LED1   7

#define STROBE  10

#define BUTTON_CHECK_TIME 100

#define LEFT_BIAS 511
#define RIGHT_BIAS 511

#define SHIFT_REG_COUNT 4
#define REG_OUTPUTS     8
#define SHIFT_OUTPUTS   REG_OUTPUTS*SHIFT_REG_COUNT

#define LEFT_CHANNEL_OUTPUTS   2*REG_OUTPUTS+2
#define RIGHT_CHANNEL_OUTPUTS  SHIFT_OUPUTS - LEFT_CHANNEL_OUTPUTS

#define FIRST_USED_REG_OUT    4
#define UNUSED_A0       7
#define UNUSED_A1       UNUSED_A0+REG_OUTPUTS
#define UNUSED_A2       UNUSED_A0+REG_OUTPUTS*2
#define UNUSED_A3       UNUSED_A0+REG_OUTPUTS*3

enum flashingType
{
  FULL_LINE,
  DOTTS
};

boolean led2 = false;


flashingType status = FULL_LINE;

unsigned long time;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED1, OUTPUT);
  pinMode(LED0, OUTPUT);

  pinMode(STROBE, OUTPUT);

  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  int dataLeft = analogRead(LEFT);
  int dataRight = analogRead(RIGHT);
  
  unsigned int leftChannelValue = getChannelValue(dataLeft, LEFT_BIAS);
  unsigned int rightChannelValue = getChannelValue(dataRight, RIGHT_BIAS);

  unsigned long preparedData = dataPrepare(leftChannelValue, rightChannelValue);

  flashingLeds(preparedData);
    

  if (millis() - time > BUTTON_CHECK_TIME) {
    
    digitalWrite(LED0, digitalRead(BUT0));
    digitalWrite(LED1, digitalRead(BUT1));
    
    time = millis();
  }

}

unsigned int getChannelValue(int channelValue, int bias)
{

  channelValue = abs(channelValue - bias);

  byte lastLed = map(channelValue, 0, 511, 0, 13);

  unsigned int data=0;

  switch ( status ) {
  case FULL_LINE:

    for (int i = 0; i < lastLed; ++i) {
      data |= 1;
      data <<= 1;
    }
    break;

  case DOTTS:
    data |= 1;
    for (int i = 0; i < lastLed; ++i) {
      data <<= 1;
    }
    break;      
  }

  return data;
}

unsigned long dataPrepare(unsigned int leftChannel, unsigned int rightChannel)
{

  unsigned long finalData = 0;
  byte i;
  
  for (i = 0; i < LEFT_CHANNEL_OUTPUTS; ++i) {
    if (!((i < FIRST_USED_REG_OUT)||(i = UNUSED_A0)||(i = UNUSED_A1))) {
      //WRONG
      finalData |= leftChannel & 1;
      leftChannel >>=1;
    }
    finalData<<=1;
  }

  for (i = LEFT_CHANNEL_OUTPUTS; i < SHIFT_OUTPUTS;  ++i) {
   if (!((i = UNUSED_A2)||(i = UNUSED_A3))) {
      finalData |= rightChannel & 1;
      rightChannel >>=1;
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


