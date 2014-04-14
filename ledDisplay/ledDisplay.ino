
// inslude the SPI library:
#include <SPI.h>


// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

byte pinsGreen[8] = {
  30, 32, 34, 36,
  2, 4, 6, 8
};

byte pinsRed[8] = {
  31, 33, 35, 37,
  3, 5, 7, 9

};

byte redBitmap[8] =
{
    0b00111100,
    0b01000010,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01000010,
    0b00111100
    /*
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001*/
};

byte greenBitmap[8] =
{
  0b00000000,
  0b00000000,
  0b00000000,
  0b00100100,
  0b00000000,
  0b00100100,
  0b00011000,
  0b00000000

/*  0b01111111,
  0b10111111,
  0b11011111,
  0b11101111,
  0b11110111,
  0b11111011,
  0b11111101,
  0b11111110*/
};

int pot;

void paintBitmap()

{
  byte currentLine = 0b00000001;
  //paint red bitmap
  for (int i = 0; i < 8; ++i)
  {
    paintLine(~redBitmap[i], ~greenBitmap[i], currentLine );
    //paintLine(0b00001000, 0b10001000,  currentLine );
    currentLine = currentLine << 1;
  }
  
  

}

int scanDelay = 100;

void paintLine(byte redPoint, byte greenPoint, byte line)
{
  byte curLine = 0b00000001;


  for (byte i = 0; i < 8; ++i)
  {
    digitalWrite(pinsRed[i], (redPoint & curLine));
    digitalWrite(pinsGreen[i], (greenPoint & curLine));

    curLine <<= 1;
  }
  curLine = 1;
  
    for (byte i = 22; i < 30; ++i)
  {
    digitalWrite(i, (line & curLine));
    curLine <<= 1;
  }

// delayMicroseconds(1000);
//scanDelay -= 1;
//scanDelay = max(scanDelay, 1);
//delay(scanDelay);
  pot=((analogRead(A0))>>4)+2;
delay(pot);
}

byte line = 1;
byte redLine = 0;
byte greenLine = 1;

void setup() {
  Serial.begin(9600);

  for (int i = 22; i < 38; ++i)
  {
    pinMode(i, OUTPUT);

  }
  
    for (int i = 2; i < 10; ++i)
  {
    pinMode(i, OUTPUT);

  }
  
  pinMode(A0, INPUT);

}

void loop() {

  paintBitmap();
  //delay(1000);
  



}

