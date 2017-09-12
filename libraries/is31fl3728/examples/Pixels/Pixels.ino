#include "Wire.h"
#include "is31fl3728.h"

IS32FL3728_TWI d;

void setup()
{
 d.begin(0);
 d.fb().clear();
 d.updateDisplay();
}

void loop()
{
 d.fb().setPixel(random(8),random(8));
 d.fb().clearPixel(random(8),random(8));
 d.updateDisplay();
 delay(500);
}
