#include <QuadDisplay.h>

void setup()
{
}

void loop()
{
  for (int i = 0; i < NUM_DIGITAL_PINS; ++i) {
    displayInt(i, i, true);
    delay(10);
  }
}
