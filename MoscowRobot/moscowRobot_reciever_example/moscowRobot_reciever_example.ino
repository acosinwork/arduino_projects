/*
 * код для проверки работы передатчика
 * 
 * Подключите приемник к 10-й ноге ардуины, ардуйну к компу. Откройте SerialMonitor. Принимайте пакеты, отправленные с RS232 железки
 */
 
#include <SoftwareSerial.h>

#define IR_SPEED 1200    // ИК приёмник быстро не умеет работать.

SoftwareSerial mySerial(10, 11); // RX, TX - к 10-й ноге подключаем ИК-приёмник

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  mySerial.begin(IR_SPEED);
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
}

