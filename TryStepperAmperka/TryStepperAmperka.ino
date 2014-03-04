#include <StepperAmperka.h>

//Количество шагов на 1 оборот. Характеристика используемого шагового двигателя
int steps_per_revolution=200;

// Инициализация Motor Shield в двухфазном режиме. Возможные значения: 
// WAVE_DRIVE, FULL_STEP, HALF_STEP
StepperAmperka motor = StepperAmperka(steps_per_revolution, 4, 5, 6, 7, FULL_STEP);

void setup()
{
  // Устанавливаем скорость вращения 30 оборотов в минуту.
  motor.setSpeed(30);
}

void loop()
{
// Один оборот в одну сторону
  motor.step(200);

// Затем подождём
  delay(1000);

// Один оборот в другую сторону
  motor.step(-200);

// И ещё подождём
  delay(1000);
}
