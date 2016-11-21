// библиотека для работы с датчиками MQ (Troyka-модуль)
#include <TroykaMQ.h>

// имя для пина, к которому подключен датчик
#define PIN_MQ7         A0
// имя для пина, к которому подключен нагреватель датчика
#define PIN_MQ7_HEATER  11

// создаём объект для работы с датчиком
// и передаём ему номер пина выходного сигнала и нагревателя
MQ7 mq7(PIN_MQ7, PIN_MQ7_HEATER);

void setup()
{
  // открываем последовательный порт
  Serial.begin(9600);
  // запускаем термоцикл
  // в течении 60 секунд на нагревательный элемент подаётся 5 вольт
  // в течении 90 секунд — 1,5 вольта
  mq7.cycleHeat();
}

void loop()
{
  // если прошёл интервал нагрева датчика
  // и калибровка не была совершена
  if (!mq7.isCalibrated() && mq7.atHeatCycleEnd()) {
    // выполняем калибровку датчика на чистом воздухе
    mq7.calibrate();
    // выводим сопротивление датчика в чистом воздухе (Ro) в serial-порт
    Serial.print("Ro = ");
    Serial.println(mq7.getRo());
    // запускаем термоцикл
    mq7.cycleHeat();
  }
  // если прошёл интевал нагрева датчика
  // и калибровка была совершена
  if (mq7.isCalibrated() && mq7.atHeatCycleEnd()) {
    // выводим отношения текущего сопротивление датчика
    // к сопротивлению датчика в чистом воздухе (Rs/Ro)
    Serial.print("Ratio: ");
    Serial.print(mq7.readRatio());
    // выводим значения газов в ppm
    Serial.print(" CarbonMonoxide: ");
    Serial.print(mq7.readCarbonMonoxide());
    Serial.println(" ppm ");
    delay(100);
    // запускаем термоцикл
    mq7.cycleHeat();
  }
}

