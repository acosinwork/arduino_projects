#include "Wire.h"
#include "is31fl3728.h"
#include "font8x8/font8x8_basic.h"

IS32FL3728_TWI d;

uint8_t c = 32;

void setup()
{
 d.begin(0);
 d.fb().clear();
 d.updateDisplay();
 d.fb().setFont((const uint8_t *)font8x8_basic, 128, 8);
}

void loop()
{
 d.fb().setCharacter(c);
 c = (c+1)%(128-32)+32;
 d.updateDisplay();
 delay(500);
}
