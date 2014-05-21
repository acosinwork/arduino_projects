#include <Wire.h>
#include <Strela.h>

void setup() {
  // put your setup code here, to run once:
  Strela.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; ++i)
  {
    Strela.ledWrite(i, HIGH);
    buttons();
    delay(50);
    Strela.ledWrite(i, LOW);
    buttons();
    delay(50);
  }


  for (int i = 0; i < 4; ++i)
  {
    Strela.ledWrite(3 - i, HIGH);
    buttons();
    delay(50);
    Strela.ledWrite(3 - i, LOW);
    buttons();
    delay(50);
  }

}

void buttons()
{
  for (int i = 0; i < 4; ++i)
  {
    if (Strela.buttonRead(i))

      tone(BUZZER, (440 * (i + 1)), 100);

  }
}
