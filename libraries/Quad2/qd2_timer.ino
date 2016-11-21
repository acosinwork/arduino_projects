#include <QuadDisplay2.h>
//Отображаем количество секунд, прошедшее со старта скетча

QuadDisplay qd;

void setup()
{
  qd.begin(13,11,10);
}

void loop()
{
  float currentTime = (float)millis() / 10000;
  
  qd.displayFloat(currentTime, 1);
  
  delay(50);
}
