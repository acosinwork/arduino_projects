#include <TimerOne.h>

#define SAMPLE_RATE 2000 // частота дискретизации
#define MICROSECONDS 1000000 // микросекунд в секунде
#define SAMPLE_PERIOD MICROSECONDS/SAMPLE_RATE //период измерения равен вот этому

#define FREQ_SENSOR_PIN A0 // или другое..

boolean volatile timeToReadValue = false; // эту переменную берём только из RAM

void setup() 
{
  Timer1.initialize(SAMPLE_PERIOD); //Взводим таймер на срабатывание через каждый период дискретизации
  Timer1.attachInterrupt( timerIsr ); //эта функция будет вызываться SAMPLE_RATE раз в секунду
  Serial.begin(115200); //шустренько шоб
}

void loop()
{
  // тут у нас код, который будут прерывать прерывания
  // 
  if (timeToReadValue) // Если пора считать значение
  {
    Serial.println(analogRead(FREQ_SENSOR_PIN)); //будем надеяться, что это быстрее чем timerIsr()
  }
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
  // измеряем напряженье.
  // Запомните, это очень, очень плохо измерять напряжение в Ардуино
  // в прерываниях. Так делать просто нельзя!
  // Потому что в прерываниях нельзя делат длительные операции 
  // да ещё и с вложенными прерываниями - а они тут... ой. 
  // Мы сделаем по другому
  // Будем просто оповещать о прерывании и надеяться на чудо
  timeToReadValue = true;   
}

