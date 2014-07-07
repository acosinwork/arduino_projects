#include <Servo.h>


#define BUTTON_PIN  3
#define LED_PIN     13
#define SERVO  9

boolean buttonWasUp = true;  // была ли кнопка отпущена?
boolean ledEnabled = false;  // включен ли свет?\
Servo myServo;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  myServo.attach(SERVO);
}

void loop()
{
  // определить момент «клика» несколько сложнее, чем факт того,
  // что кнопка сейчас просто нажата. Для определения клика мы
  // сначала понимаем, отпущена ли кнопка прямо сейчас...
  boolean buttonIsUp = digitalRead(BUTTON_PIN);

  // ...если «кнопка была отпущена и (&&) не отпущена сейчас»...
  if (buttonWasUp && !buttonIsUp) {
    // ...может это «клик», а может и ложный сигнал (дребезг),
    // возникающий в момент замыкания/размыкания пластин кнопки,
    // поэтому даём кнопке полностью «успокоиться»...
    delay(10);
    // ...и считываем сигнал снова
    buttonIsUp = digitalRead(BUTTON_PIN);
    if (!buttonIsUp) {  // если она всё ещё нажата...
      // ...это клик! Переворачиваем сигнал светодиода
      ledEnabled = !ledEnabled;
      digitalWrite(LED_PIN, ledEnabled);
    }
  }

  // запоминаем последнее состояние кнопки для новой итерации
  buttonWasUp = buttonIsUp;

  if (ledEnabled)  //если светодиод горит - то поворачиваем на 45 град
  {
    myServo.write(45);
  }
  else
  {
    myServo.write(90); //а если нет - то поворачиваем на 90 град
  }
}
