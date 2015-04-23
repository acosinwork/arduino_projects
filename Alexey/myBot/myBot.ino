#include <EEPROM.h>
#include <Wire.h>
#include <Strela.h>
#include <LineBot.h>
#include <LiquidCrystal_I2C.h>

#define INT_TIME 20

const double multiplier = 5.0 / 1024;

int power = 50;

double k = 0.015;
double cub = 0.00000017;

unsigned long time;

LineBot bot;


LiquidCrystal_I2C lcd(LC_ADDR, LCEN, LCRW, LCRS, LC4, LC5, LC6, LC7);


void setup() {
  // put your setup code here, to run once:
  bot.begin();

  bot.sensors.attach(P6);
  bot.sensors.attach(P7);

  lcd.begin(8, 2);
  //Переместим курсор в начало первой стройки (символ 0, строка 0)
  lcd.home();

  while (!uDigitalRead(S2))
  {
    for (int i = 0; i < bot.sensors.count(); ++i)
      bot.sensors.setBlack(i);
  }

  tone(BUZZER, 200, 100);

  while (!uDigitalRead(S4))
  {
    for (int i = 0; i < bot.sensors.count(); ++i)
      bot.sensors.setWhite(i);
  }
  tone(BUZZER, 500, 100);

  delay(1000);

  //  Serial.begin(9600);

}

void loop() {

  if (millis() - time >= INT_TIME) {
    time = millis();

    int error =  getErr(); // должен быть 0, если нет - регулируем

    int u = constrain( k * error + cub * pow(error, 3),
                       - power, power);

    bot.speed(power + u , power - u);
  }

  checkClick();
}

int getErr()
{
  return (bot.sensors.readPct(0) - bot.sensors.readPct(1));
}

bool checkClick()
{


  static bool lastClick = false;
  static unsigned long time = 0;
  if (millis() - time >= 50)
  {
    bool result = uDigitalRead(S1);
    if (!lastClick && result) {
      tone(BUZZER, 1000, 50);
      bot.speed(0);
      bool pressed = false;
      while (!pressed)
      {
        if ((millis() % 50) > 40)
        {
          lcd.clear();
          lcd.home();
          lcd.print(setK());
        }

        if (uDigitalRead(S2))
        {
          pressed = true;
          k = setK() * 0.1;
        } else if (uDigitalRead(S3))
        {
          pressed = true;
          power = setK() * 50;
        } else if (uDigitalRead(S4))
        {
          pressed = true;
          cub = setK() * 0.000001;
        }
      }
      printAll();
    }
    time = millis();
    lastClick = result;
  }
  return lastClick;
}


void printAll()
{
  lcd.clear();
  lcd.home();
  lcd.print(k);
  lcd.setCursor(0, 1);
  lcd.print(cub);
}

double setK()
{
  return (double)analogRead(P8) * multiplier;
}


