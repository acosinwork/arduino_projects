
#include <Wire.h>
#include <Strela.h>
#include <LiquidCrystal_I2C.h>

// =======================================================================

//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(LC_ADDR, LCEN, LCRW, LCRS, LC4, LC5, LC6, LC7);



void setup()
{

  Wire.begin();
  lcd.begin(8, 2);
  
  uPinMode(P10, INPUT);
  uPinMode(P9, INPUT);

}

void loop()
{
  
  

  lcd.clear();
  lcd.home();
  
  lcd.print(uDigitalRead(P10));
  lcd.print(uDigitalRead(P9));
  
  lcd.setCursor(0, 1);
  lcd.print(uDigitalRead(S1));
  lcd.print(uDigitalRead(S2));
  lcd.print(uDigitalRead(S3));
  lcd.print(uDigitalRead(S4));
  
  
  delay(30);
  
  if (uDigitalRead(S1))
  uDigitalWrite(L1, LOW);
  else
  uDigitalWrite(L1, HIGH);
  
  /*
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
*/

}

void printStringLCD()
{
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 1);

}
