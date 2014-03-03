//Определяем пины для управления Motor Shield
#define E1    5
#define E2    6
#define H1    4
#define H2    7

// Рассмотрим три режима управления шаговым мотором:
#define WAWE_DRIVE  0 // Однофазный режим
#define FULL_STEP   1 // Двухфазный режим
#define HALF_STEP   2 // Полушаговый режим

//Задержка между переключением обмоток. Определяет скорость вращения
int delayTime = 5;

unsigned long startTime;

// Эта функция непосредственно выставляет значение на пинах
void doStep(boolean E1State, boolean H1State, boolean E2State, boolean H2State)
{
  digitalWrite(E1, E1State);
  digitalWrite(H1, H1State);
  digitalWrite(E2, E2State);
  digitalWrite(H2, H2State);
  delay(delayTime);
}
// А здесь описнаы комбинации управляющих импульсов в зависимости от режима
void rotate(byte rotateMode)
{
  switch (rotateMode)
  {
    case WAWE_DRIVE :
      doStep(1, 1, 0, 0);
      doStep(0, 0, 1, 1);
      doStep(1, 0, 0, 0);
      doStep(0, 0, 1, 0);
      break;

    case FULL_STEP :
      doStep(1, 1, 1, 1);
      doStep(1, 0, 1, 1);
      doStep(1, 0, 1, 0);
      doStep(1, 1, 1, 0);
      break;

    case HALF_STEP :
      doStep(1, 1, 0, 0);
      doStep(1, 1, 1, 1);
      doStep(0, 0, 1, 1);
      doStep(1, 0, 1, 1);

      doStep(1, 0, 0, 0);
      doStep(1, 0, 1, 0);
      doStep(0, 0, 1, 0);
      doStep(1, 1, 1, 0);
      break;
  }
}

void setup() {

  // Настраиваем ножки на выход
  for (int i = 4; i < 8; ++i)
  {
    pinMode(i, OUTPUT);
  }

  startTime = millis();

}

void loop() {

  // Узнаём время прошедшее с начала работы в миллисекундах
  unsigned long loopTime = millis() - startTime;

  // Переведём его в секунды
  unsigned int timeInSecond = loopTime / 1000;

  // Шагаем! Каждую секунду режим управления будет меняться.
  rotate(timeInSecond % 3);

}
