
#include <SoftwareSerial.h>

SoftwareSerial gprsSerial(7, 8);

int btnPin = 12;
boolean prevBtn = LOW;

void setup()
{
  gprsSerial.begin(19200);
  delay(500);
  
  //Включаем GPRS Shield, эмулируя нажатие кнопки POWER
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);   // Подаем High на пин 9
  delay(3000);              // на 3 секунды
  digitalWrite(9, LOW);    // и отпускаем в Low. 
  
  
  delay(5000); // Ждём 5 секунд для старта шилда
  
  btnPin >>= 1;
}

void loop()
{

/*
 * Функция отправки SMS-сообщения
 */

  // Устанавливает текстовый режим для SMS-сообщений
  gprsSerial.print("AT+CMGF=1\r");
  delay(100); // даём время на усваивание команды
  // Устанавливаем адресата: телефонный номер в международном формате
  gprsSerial.println("AT + CMGS = \"+70123456789\"");
  delay(100);
  // Пишем текст сообщения
  gprsSerial.println("ALARM!");
  delay(100);
  // Отправляем Ctrl+Z, обозначая, что сообщение готово
  gprsSerial.println((char)26);

//Закончили работу
while (true)
{;;}

}
