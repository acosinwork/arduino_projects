// библиотека для работы с датчиками MQ (Troyka-модуль)
#include <TroykaMQ.h>

//имя для пина, к которому подключен датчик
#define PIN_MQ8  A0
// создаём объект для работы с датчиком и передаём ему номер пина
MQ8 mq8(PIN_MQ8);

void setup()
{
  // открываем последовательный порт
  Serial.begin(9600);
  // перед калибровкой датчика прогрейте его 60 секунд
  // выполняем калибровку датчика на чистом воздухе
  mq8.calibrate();
  // выводим сопротивление датчика в чистом воздухе (Ro) в serial-порт
  Serial.print("Ro = ");
  Serial.println(mq8.getRo());
}

void loop()
{
  // выводим отношения текущего сопротивление датчика
  // к сопротивление датчика в чистом воздухе (Rs/Ro)
  Serial.print("Ratio: ");
  Serial.print(mq8.readRatio());
  // выводим значения газов в ppm
  Serial.print(" LPG: ");
  Serial.print(mq8.readHydrogen());
  Serial.println(" ppm ");
  delay(100);
}
