
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
  0b00000000,
  0b00000000,
  0b00000000,
  0b00100100,
  0b00000000,
  0b00100100,
  0b00011000,
  0b00000000,
};

void paintBitmap()

{
  byte currentLine=128;
  //paint red bitmap
  for (int i = 0; i < 8; ++i)
  {
    //paintLine(redBitmap[i], greenBitmap[i], currentLine );
    paintLine(0,redBitmap[i],  currentLine );
    currentLine = currentLine >> 1;
  }

}

void paintLine(byte redPoint, byte greenPoint, byte line)
{
  byte firstByte=0;
  byte secondByte=0;
  byte a, b;
  
  a=0b00000001;
  b=0b00010000;
  
  for (byte i = 0; i < 4; ++i)
  {
      firstByte |= (redPoint & a)<<i;      
      secondByte |= (redPoint & b)<<i;

      firstByte |= (greenPoint & a)<<(i+1); 
      secondByte |= (greenPoint & b)<<(i+1); 
    
      a=a<<1;
      b=b<<1;
  }  
  
//  Serial.println(firstByte);
//  Serial.println(secondByte);
  
  
  SPI.transfer(line);
  SPI.transfer(firstByte);
  SPI.transfer(secondByte);



  digitalWrite(slaveSelectPin, HIGH);
  digitalWrite(slaveSelectPin, LOW);
//
  delay(100);
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

