
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define GPLCD_TWI_ADDR   0x21


// =======================================================================

//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x21, 4, 5, 6, 0, 1, 2, 3);

void setup()
{
    pinMode(12, OUTPUT);
    tone(12, 2000, 50);

    Wire.begin();
    lcd.begin(8, 2);
}

void loop()
{
    lcd.home();
    lcd.print("abcdefgh");
    lcd.setCursor(0, 1);
    lcd.print("ABCDEFGH");
}
