#include <EEPROM.h>
#include <Wire.h>
#include <Strela.h>
#include <LineBot.h>
#include <LiquidCrystal_I2C.h>
#include <PID_v1.h>

unsigned long timetimetime = 0;

int power = 150;//90;

const double multiplier = 3.0 / 1024;
double Setpoint, Input, Output;

double Kp = 1.14, Ki = 0.4, Kd = 0.08;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


LineBot bot;

LiquidCrystal_I2C lcd(LC_ADDR, LCEN, LCRW, LCRS, LC4, LC5, LC6, LC7);

void setup() {
  // put your setup code here, to run once:

  bot.begin();
  bot.sensors.attach(P6);
  bot.sensors.attach(P7);

  //Инициализация экрана. У нашего экрана 8 символов 2 строки
  lcd.begin(8, 2);
  //Переместим курсор в начало первой стройки (символ 0, строка 0)
  lcd.home();

  printAll();

  while (!uDigitalRead(S2))
  {
    for (int i = 0; i < bot.sensors.count(); ++i)
      bot.sensors.setBlack(i);
  }

  tone(BUZZER, 400, 100);

  while (!uDigitalRead(S4))
  {
    for (int i = 0; i < bot.sensors.count(); ++i)
      bot.sensors.setWhite(i);
  }
  tone(BUZZER, 500, 100);

  Input = getLine();
  Setpoint = 0;

  delay(1000);
  //turn the PID on
  myPID.SetSampleTime(30);
  myPID.SetOutputLimits(-power, power);

  myPID.SetMode(AUTOMATIC);

}

void loop() {
  // put your main code here, to run repeatedly:

  Input = getLine();
  if (myPID.Compute())
  {
//    lcd.home();
//    lcd.print(millis() - timetimetime);
//    timetimetime = millis();

    int uSpeed = Output;
    int brake = (abs(uSpeed)>>1);

    bot.speed(power - uSpeed - brake, power + uSpeed - brake);
  }
  checkClick();

}

int getLine()
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

      while (uDigitalRead(S1))
        delay(10);


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
          Kp = setK();
        } else if (uDigitalRead(S3))
        {
          pressed = true;
          Ki = setK();
        } else if (uDigitalRead(S4))
        {
          pressed = true;
          Kd = setK();
        }
        else if (uDigitalRead(S1))
        {
          pressed = true;
        }
      }

      myPID.SetTunings(Kp, Ki, Kd);

      // reinitialize PID
      //      myPID.SetMode(MANUAL);
      //      myPID.SetMode(AUTOMATIC);


      printAll();
      delay(3000);
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
  lcd.print(myPID.GetKp());
  lcd.print(myPID.GetKi());
  lcd.setCursor(0, 1);
  lcd.print(myPID.GetKd());
}

double setK()
{
  return (double)analogRead(P8) * multiplier;
}




