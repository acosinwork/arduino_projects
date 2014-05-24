
#include <Wire.h>
#include <Strela.h>
#include <LiquidCrystal_I2C.h>

// =======================================================================

//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(LC_ADDR, LCEN, LCRW, LCRS, LC4, LC5, LC6, LC7);

Strela S;

void setup()
{

  Wire.begin();
  S.begin();
  lcd.begin(8, 2);

}

void loop()
{
  
  for (int i=L4; i<=L1; ++i)
  {
      lcd.clear();
  lcd.home();
  lcd.print(i);
    uDigitalWrite(i, HIGH);
    delay(1000);
    uDigitalWrite(i, LOW);
    delay(1000);
  }


}

void printStringLCD()
{
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 1);

}
