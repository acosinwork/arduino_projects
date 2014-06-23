
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
  /*
  0b00000000,
  0b00000000,
  0b00000000,
  0b00100100,
  0b00000000,
  0b00100100,
  0b00011000,
  0b00000000,
    */
  0b00011000,  
  0b00100100,
  0b01000010,
  0b10000001,  
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000

};

void paintBitmap()

{
  byte currentLine=0b10000000;
  //paint red bitmap
  for (int i = 0; i < 8; ++i)
  {
    paintLine(redBitmap[i], greenBitmap[i], currentLine );
    //paintLine(greenBitmap[i], greenBitmap[i], currentLine );
    
    //paintLine(255,255,  currentLine );
    currentLine >>= 1;
  }

}

void paintLine(byte redPoint, byte greenPoint, byte line)
{
  byte firstByte=0;
  byte secondByte=0;
  
  for (int i=0; i<4; ++i)
  {
    firstByte |= redPoint & 0b00000001;
    firstByte <<= 1;

    firstByte |= greenPoint & 0b00000001;
    firstByte <<= 1;
   
    redPoint >>= 1;
    greenPoint >>= 1;    
  }
    for (int i=0; i<4; ++i)
  {
    secondByte |= redPoint & 0b00000001;
    secondByte <<= 1;
    
    secondByte |= greenPoint & 0b00000001;
    secondByte <<= 1;
    
    redPoint >>= 1;
    greenPoint >>= 1;     
  }
 
  
  SPI.transfer(line);

  SPI.transfer(secondByte);
  SPI.transfer(firstByte);

  digitalWrite(slaveSelectPin, LOW);
  digitalWrite(slaveSelectPin, HIGH);

delay(2);


//

}


byte line = 1;
byte redLine = 0;
byte greenLine = 1;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  digitalWrite(slaveSelectPin, HIGH);
  // initialize SPI:
  SPI.begin();
  Serial.begin(9600);
}

void loop() {

  paintBitmap();
  //delay(1000);


}

