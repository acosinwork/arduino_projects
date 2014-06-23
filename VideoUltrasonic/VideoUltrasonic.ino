/*
 *UltrasonicDemo.pde - Ultrasonic sensor Ranging measure Demo
 *@Author:dragon
 *@DATA:2013-8-7
 *Company website:www.elecfreaks.com
 */
#include <Ultrasonic.h>

int trigpin = 12;//appoint trigger pin
int echopin = 13;//appoint echo pin

int vcc = 11;
//int gnd = 7;


Ultrasonic ultrasonic(trigpin, echopin);

//temperature

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS A0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Display

const byte PIN_CTL = A5;

int level = 0;

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
  for (byte y = 0; y < 4; ++y) {
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

  int pure_data = int_data;

  if ((pure_data > 999) && (pure_data < 10000))
  {

    capacity = int_data / 1000;

    if ((capacity >= 0) && (capacity < 10))
    {
      displayValue[0] = numbers[capacity];
      int_data = int_data - (capacity * 1000);
    }
  }
  if (pure_data > 99)
  {
    capacity = int_data / 100;

    if ((capacity >= 0) && (capacity < 10))
    {
      displayValue[1] = numbers[capacity];
      int_data = int_data - (capacity * 100);
    }
  }
  if (pure_data > 9)
  {
    capacity = int_data / 10;

    if ((capacity >= 0) && (capacity < 10))
    {
      displayValue[2] = numbers[capacity];
      int_data = int_data - (capacity * 10);
    }
  }
  capacity = int_data;

  if ((capacity >= 0) && (capacity < 10))
  {
    displayValue[3] = numbers[capacity];
  }

  // десятичная точка
  displayValue[3] ^= 1;

  for (capacity = 0; capacity < 4; ++capacity)
  {
    sendByte(displayValue[capacity]);
  }
  latch();
}

void setup()
{

  //temp
  sensors.begin();
  //display
  pinMode(PIN_CTL, OUTPUT);
  digitalWrite(PIN_CTL, HIGH);
  clearDisplay();

  pinMode(vcc, OUTPUT);

  digitalWrite(vcc, HIGH);


  Serial.begin(9600);//set Serial Baud rate
  Serial.println("Ultrasonic sensor starting!!!!!");


}

void loop()
{

  sensors.requestTemperatures();
  delay(50);

  float cmdistance;
  long microsec = ultrasonic.timing();

  Serial.print("microsec: ");
  Serial.print(microsec);

  cmdistance = ultrasonic.CalcDistance(microsec, Ultrasonic::CM); //this result unit is centimeter
  //cmdistance = sensors.getTempCByIndex(0);
  Serial.print(" cmdistance: ");
  Serial.println(cmdistance);

  displayInt(int(cmdistance * 10));

  delay(500);
}
