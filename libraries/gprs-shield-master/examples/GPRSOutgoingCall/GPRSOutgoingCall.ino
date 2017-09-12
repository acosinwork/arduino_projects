// библиотека для работы с GPRS устройством
#include <GPRS_Shield_Arduino.h>
 
// номер на который будем звонить
#define PHONE_NUMBER  "+79263995140"

// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(Serial1);
// можно указать дополнительные параметры — пины PK и ST
// по умолчанию: PK = 2, ST = 3
// GPRS gprs(Serial1, 2, 3);

void setup()
{
  // включаем GPRS шилд
  gprs.powerOn();
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  while (!Serial) {
  }
  // открываем Serial-соединение с GPRS Shield
  Serial1.begin(9600);
  // проверяем есть ли связь с GPRS устройством
  while (!gprs.init()) {
    // если связи нет, ждём 1 секунду
    // и выводим сообщение об ошибке
    // процесс повторяется в цикле
    // пока не появится ответ от GPRS устройства
    delay(1000);
    Serial.print("GPRS Init error\r\n");
  }
  // вывод об удачной инициализации GPRS Shield
  Serial.println("GPRS init success");
  // сообщаем о наборе номера
  Serial.print("Start to call ");
  Serial.print(PHONE_NUMBER);
  // звоним по указанному номеру
  gprs.callUp(PHONE_NUMBER);
}
 
void loop()
{
}
