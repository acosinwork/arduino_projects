
// inslude the SPI library:
#include <SPI.h>


// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

byte redBitmap[8] =
{
  /*  0b00111100,
    0b01000010,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01000010,
    0b00111100
    */
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
};

byte greenBitmap[8] =
{
/*  0b00000000,
  0b00000000,
  0b00000000,
  0b00100100,
  0b00000000,
  0b00100100,
  0b00011000,
  0b00000000,
};*/
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
};

void paintBitmap()

{
  byte currentLine = 0b00000001;
  //paint red bitmap
  for (int i = 0; i < 8; ++i)
  {
    paintLine(0, greenBitmap[i], currentLine );
    //paintLine(1, 1,  currentLine );
    currentLine = currentLine << 1;
  }
  
  

}

void paintLine(byte redPoint, byte greenPoint, byte line)
{
  byte curLine = 0b00000001;

  for (byte i = 22; i < 30; ++i)
  {
    digitalWrite(i, (line & curLine));
    curLine <<= 1;
  }

  curLine = 1;
  for (byte i = 30; i < 38; ++i)
  {
    digitalWrite(i, (redPoint & curLine));
    curLine <<= 1;
  }

  curLine = 1;
  for (byte i = 38; i < 46; ++i)
  {
    digitalWrite(i, (greenPoint & curLine));
    curLine <<= 1;
  }
  
 delay(500);
}

byte line = 1;
byte redLine = 0;
byte greenLine = 1;

void setup() {
  Serial.begin(9600);

  for (int i = 22; i < 46; ++i)
  {
    pinMode(i, OUTPUT);
//    digitalWrite(i,0);
  }

}

void loop() {

  paintBitmap();
  //delay(1000);


}

