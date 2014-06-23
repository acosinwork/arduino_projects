#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 5
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

const byte PIN_CTL = 9;

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
  
  displayValue[3]&=B11111110;


  for (capacity=0; capacity<4; ++capacity)
  {
    sendByte(displayValue[capacity]);
  }
  latch();
}


void setup(void)

{
  pinMode(PIN_CTL, OUTPUT);
  digitalWrite(PIN_CTL, HIGH);
  clearDisplay();

 
  // start serial port
Serial.begin(9600);
Serial.println("Dallas Temperature IC Control Library Demo");
// Start up the library
sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
}

void loop(void)
{
// call sensors.requestTemperatures() to issue a global temperature
// request to all devices on the bus
Serial.print("Requesting temperatures...");
sensors.requestTemperatures(); // Send the command to get temperatures
Serial.println("DONE");
 
Serial.print("Temperature for Device 1 is: ");
Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire

displayInt(int(10*sensors.getTempCByIndex(0)));

}
