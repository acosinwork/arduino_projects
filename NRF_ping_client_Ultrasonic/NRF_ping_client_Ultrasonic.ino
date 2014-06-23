/**
 * A Mirf example to test the latency between two Ardunio.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>


//Display

const byte PIN_CTL = A5;

int level=0;

const byte numbers[] = {
  0b00000011,  //0
  0b10011111,  //1
  0b00100101,  //2
  0b00001101,  //3
  0b10011001,  //4
  0b01001001,  //5
  0b01000001,  //6
  0b00011111,  //7
  0b00000001,  //8
  0b00001001   //9
};

void sendByte(byte data, const byte n = 8)
{
  for (byte i = n; i > 0; i--)
  {
    if (data & 1)
    {
      noInterrupts();
      digitalWrite(PIN_CTL, LOW);
      digitalWrite(PIN_CTL, HIGH);
      interrupts();
      delayMicroseconds(30);
    }
    else
    {
      digitalWrite(PIN_CTL, LOW);
      delayMicroseconds(15);
      digitalWrite(PIN_CTL, HIGH);
      delayMicroseconds(60);
    }
    data >>= 1;
  }
}

void latch()
{
  digitalWrite(PIN_CTL, LOW);
  delayMicroseconds(100);
  digitalWrite(PIN_CTL, HIGH);
  delayMicroseconds(300);
}

void clearDisplay()
{
  for (byte y = 0; y<4; ++y){
    sendByte(0b11111111);
  }
  latch();
}

void displayInt(int int_data)
{
  byte displayValue[] = {
    0b11111111,
    0b11111111, 
    0b11111111, 
    0b11111111  
  };
  byte capacity;

  int pure_data=int_data;

  if ((pure_data>999)&&(pure_data<10000)) 
  {

    capacity=int_data/1000;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[0]=numbers[capacity];
      int_data=int_data - (capacity*1000);
    }
  }
  if (pure_data>99) 
  {
    capacity=int_data/100;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[1]=numbers[capacity];
      int_data=int_data - (capacity*100);
    }
  }
  if (pure_data>9) 
  {
    capacity=int_data/10;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[2]=numbers[capacity];
      int_data=int_data - (capacity*10);
    }
  }
  capacity=int_data;  

  if ((capacity >= 0)&&(capacity <10))
  {
    displayValue[3]=numbers[capacity];
  }

// десятичная точка
  displayValue[3] ^=1;

  for (capacity=0; capacity<4; ++capacity)
  {
    sendByte(displayValue[capacity]);
  }
  latch();
}


void setup() {
  
    //display
  pinMode(PIN_CTL, OUTPUT);
  digitalWrite(PIN_CTL, HIGH);
  clearDisplay();
  
  
  Serial.begin(9600);
  /*
   * Setup pins / SPI.
   */

  /* To change CE / CSN Pins:
   *
   * Mirf.csnPin = 9;
   * Mirf.cePin = 7;
   */
  /*
  Mirf.cePin = 7;
  Mirf.csnPin = 8;
  */
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  /*
   * Configure reciving address.
   */

  Mirf.setRADDR((byte *)"clie1");

  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */

  Mirf.payload = sizeof(float);

  /*
   * Write channel and payload config then power up reciver.
   */

  /*
   * To change channel:
   *
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */

  Mirf.config();

  Serial.println("Beginning ... ");
}

void loop() {
  float distance = 0;

  Mirf.setTADDR((byte *)"serv1");

  Mirf.send((byte *)&distance);

  while (Mirf.isSending()) {
  }
  Serial.println("Finished sending");
  delay(10);
/*  while (!Mirf.dataReady()) {
    //Serial.println("Waiting");

    return;
  }
}*/

Mirf.getData((byte *) &distance);

Serial.print("Distance: ");
Serial.println(distance);

displayInt(int(distance));

delay(500);
}



