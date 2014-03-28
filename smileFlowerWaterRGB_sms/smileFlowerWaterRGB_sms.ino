#include <rgb_matrix.h>
#include <SPI.h>
#include <SoftwareSerial.h>
 
#define N_X 1
#define N_Y 1

#define DATA_PIN  11
#define CLK_PIN   13

#define LATCH_PIN 8

rgb_matrix M = rgb_matrix(N_X, N_Y, DATA_PIN, CLK_PIN, LATCH_PIN);

SoftwareSerial gprsSerial(7, 8);
 
boolean smsWasSent=false;

enum { DRY_SOIL, HUMID_SOIL, IN_WATER };

byte state;
byte lastState;

unsigned long lastTime;
//Здесь выставляем периодичность опроса A0 в миллисекундах
int wetChekDelay=100;


void sendTextMessage() {
    // Устанавливает текстовый режим для SMS-сообщений
    gprsSerial.print("AT+CMGF=1\r");
    delay(100); // даём время на усваивание команды
    // Устанавливаем адресата: телефонный номер в международном формате
    gprsSerial.println("AT + CMGS = \"+79031034423\"");
    delay(100);
    // Пишем текст сообщения
    gprsSerial.println("ALARM!");
    delay(100);
    // Отправляем Ctrl+Z, обозначая, что сообщение готово
    gprsSerial.println((char)26);
}

void hook(void)
{
  if (lastState!=state)
  {
   M.clear();
   lastState=state;
  }

if ((millis()-lastTime)>wetChekDelay)
{
  lastTime=millis();
  int wet = analogRead(0);

  if (wet > 700)
  {
    state = IN_WATER;
    smsWasSent=false;
  }
  else if (wet < 300)
  {
    state = DRY_SOIL;
    
    if (!smsWasSent) {
      sendTextMessage();
      smsWasSent=true;
    }
    
  }
  else
  {
    state = HUMID_SOIL;
    smsWasSent=false;
  }
  
  Serial.print("Moisture Sensor Value:");
  Serial.println(wet);
}

  switch (state)
  {

    case IN_WATER :

      M.plot(1, 1, GREEN, 0);
      M.plot(1, 2, GREEN, 0);
      M.plot(2, 1, GREEN, 0);
      M.plot(2, 2, GREEN, 0);
      M.plot(1, 5, GREEN, 0);
      M.plot(1, 6, GREEN, 0);
      M.plot(2, 5, GREEN, 0);
      M.plot(2, 6, GREEN, 0);
      M.plot(4, 3, GREEN, 0);
      M.plot(4, 4, GREEN, 0);
      M.plot(6, 0, GREEN, 0);
      M.plot(6, 1, GREEN, 0);
      M.plot(6, 6, GREEN, 0);
      M.plot(6, 7, GREEN, 0);
      M.plot(7, 2, GREEN, 0);
      M.plot(7, 3, GREEN, 0);
      M.plot(7, 4, GREEN, 0);
      M.plot(7, 5, GREEN, 0);

      break;

    case HUMID_SOIL :

      M.plot(1, 1, GREEN, 0);
      M.plot(1, 2, GREEN, 0);
      M.plot(2, 1, GREEN, 0);
      M.plot(2, 2, GREEN, 0);
      M.plot(1, 5, GREEN, 0);
      M.plot(1, 6, GREEN, 0);
      M.plot(2, 5, GREEN, 0);
      M.plot(2, 6, GREEN, 0);
      M.plot(4, 3, GREEN, 0);
      M.plot(4, 4, GREEN, 0);
      M.plot(6, 1, GREEN, 0);
      M.plot(6, 2, GREEN, 0);
      M.plot(6, 3, GREEN, 0);
      M.plot(6, 4, GREEN, 0);
      M.plot(6, 5, GREEN, 0);
      M.plot(6, 6, GREEN, 0);
      M.plot(7, 2, GREEN, 0);
      M.plot(7, 3, GREEN, 0);
      M.plot(7, 4, GREEN, 0);
      M.plot(7, 5, GREEN, 0);

      M.plot(1, 1, RED, 0);
      M.plot(1, 2, RED, 0);
      M.plot(2, 1, RED, 0);
      M.plot(2, 2, RED, 0);
      M.plot(1, 5, RED, 0);
      M.plot(1, 6, RED, 0);
      M.plot(2, 5, RED, 0);
      M.plot(2, 6, RED, 0);
      M.plot(4, 3, RED, 0);
      M.plot(4, 4, RED, 0);
      M.plot(6, 1, RED, 0);
      M.plot(6, 2, RED, 0);
      M.plot(6, 3, RED, 0);
      M.plot(6, 4, RED, 0);
      M.plot(6, 5, RED, 0);
      M.plot(6, 6, RED, 0);
      M.plot(7, 2, RED, 0);
      M.plot(7, 3, RED, 0);
      M.plot(7, 4, RED, 0);
      M.plot(7, 5, RED, 0);

      break;

    case DRY_SOIL :

      M.plot(1, 1, RED, 0);
      M.plot(1, 2, RED, 0);
      M.plot(2, 1, RED, 0);
      M.plot(2, 2, RED, 0);
      M.plot(1, 5, RED, 0);
      M.plot(1, 6, RED, 0);
      M.plot(2, 5, RED, 0);
      M.plot(2, 6, RED, 0);
      M.plot(4, 3, RED, 0);
      M.plot(4, 4, RED, 0);
      M.plot(6, 2, RED, 0);
      M.plot(6, 3, RED, 0);
      M.plot(6, 4, RED, 0);
      M.plot(6, 5, RED, 0);
      M.plot(7, 0, RED, 0);
      M.plot(7, 1, RED, 0);
      M.plot(7, 6, RED, 0);
      M.plot(7, 7, RED, 0);

  }

}

void setup()
{
  gprsSerial.begin(19200);
  Serial.begin(57600);
  
  lastTime=millis();
}

void loop()
{
  M.set_cur(0, 0);
  M.display(hook);
}

