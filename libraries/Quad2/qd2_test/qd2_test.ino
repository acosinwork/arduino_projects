// Подключаем библиотеку
#include <QuadDisplay2.h>

QuadDisplay qd;

void setup()
{
  qd.begin(13,11,10);
}

void loop()
{
  // можно выводить целые числа
  qd.displayInt(123);
  delay(1000);

  // и отрицательные тоже
  qd.displayInt(-123);
  delay(1000);

  // можно показывать ведущие нули (0012)
  qd.displayInt(12, true);
  delay(1000);

  // можно показывать вещественные числа
  // с заданной точностью, например 2 знака после запятой
  qd.displayFloat(-1.23, 2);
  delay(1000);
  
  // можно показывать температуру в °C
  qd.displayTemperatureC(-5);

  // можно показывать нехитрый текст (on/off, например) или
  // произвольную графику
  qd.displayDigits(QD_O, QD_f, QD_f, QD_NONE); // off
  delay(1000);
  qd.displayDigits(QD_O, QD_n, QD_NONE, QD_NONE); // on
  delay(1000);
  // и, конечно, всё очищать
  qd.displayClear();
  delay(1000);
}
