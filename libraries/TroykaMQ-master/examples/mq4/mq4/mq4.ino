// библиотека для работы с датчиками MQ (Troyka-модуль)
#include <TroykaMQ.h>

//имя для пина, к которому подключен датчик
#define PIN_MQ4  A0
// создаём объект для работы с датчиком и передаём ему номер пина
MQ4 mq4(PIN_MQ4);

void setup()
{
  // открываем последовательный порт
  Serial.begin(9600);
  // перед калибровкой датчика прогрейте его 60 секунд
  // выполняем калибровку датчика на чистом воздухе
  mq4.calibrate();
  // выводим сопротивление датчика в чистом воздухе (Ro) в serial-порт
  Serial.print("Ro = ");
  Serial.println(mq4.getRo());
}

void loop()
{
  // выводим отношения текущего сопротивление датчика
  // к сопротивление датчика в чистом воздухе (Rs/Ro)
  Serial.print("Ratio: ");
  Serial.print(mq4.readRatio());
  // выводим значения метана в ppm
  Serial.print(" Methane: ");
  Serial.print(mq4.readMethane());
  Serial.println(" ppm ");
  delay(100);
}
