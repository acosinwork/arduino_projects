#include <Wire.h>
#include <Strela.h>
#include <LiquidCrystal_I2C.h>
//#include <Servo.h>

#define ANALOG_IN_PIN   A2
#define IR_PIN A5

#define SERVO 11
#define DISTANCE A3


#define K1 0.05//0.015
#define KUB 0.000013//0.00000017

unsigned long time;

//Servo myservo;

bool drive = true;

LiquidCrystal_I2C lcd(0x21, 4, 5, 6, 0, 1, 2, 3);

int basespeed = 60;
int average = 0;

float k1 = 1.0;//0.05;//0.015
float kub = 0.0;//0.000001;//0.00000017

void setup() {
  // put your setup code here, to run once:

  myservo.attach(SERVO);
  myservo.write(90);

  Strela.begin();
  Serial1.begin(9600);
  lcd.begin(8, 2);
  Strela.motorConnection(1, 1);

  int black, white;

  while (!Strela.buttonRead(S1))//digitalRead(IR_PIN))
  {
    black = analogRead(ANALOG_IN_PIN);
    lcd.clear();
    lcd.home();
    lcd.print("Black");
    lcd.setCursor(0, 1);
    lcd.print(black);
    delay(100);

  }
  delay(1000);

  while (!Strela.buttonRead(S1))//digitalRead(IR_PIN))
  {
    white = analogRead(ANALOG_IN_PIN);
    lcd.clear();
    lcd.home();
    lcd.print("White");
    lcd.setCursor(0, 1);
    lcd.print(white);
    delay(100);
  }

  average = (black + white) / 2;

  time = millis();

  while ((digitalRead(IR_PIN)) && !Strela.buttonRead(S3))
  {
    if ((millis() - time) > 100)
    {
      lcd.clear();
      lcd.home();
      lcd.print("AVG=");
      lcd.print(average);
      lcd.setCursor(0, 1);
      lcd.print(analogRead(ANALOG_IN_PIN));
      time = millis();
    }
  }




}

void loop() {

  String test;

  while (Serial1.available() > 0)
  {
    char incomingChar = Serial1.read();

    if (incomingChar != '\n' )
    {
      test += incomingChar;
    }
  }

  if (test.length() > 0)
  {
    lcd.clear();
    lcd.home();
    
    char testChar = test[0];
    lcd.print(testChar);
    lcd.setCursor(0, 1);

    lcd.print(testChar);
    int inData = (test.substring(1, test.length() - 1)).toInt();
    switch (testChar)
    {
      case 'b':
        basespeed = inData;
        lcd.print(basespeed);
        break;
      case 'k':
        k1 = inData;// / 10000;
        lcd.print(k1);
        break;
      case 'c':
        kub = inData;// / 1000000;
        lcd.print(kub);
        break;
      case 's':
        drive = !drive;
        break;
    }
    test = "";
  }


  // put your main code here, to run repeatedly:
  int e = analogRead(ANALOG_IN_PIN) - average; //+(1-digitalRead(11))*POVOROT;//-diff;
  float u = K1 * e  + (KUB) * pow(e, 3) ;
  int leftSpeed = basespeed + int(u);
  int rightSpeed = basespeed - int(u);



  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  if ((millis() - time) > 100)
  {
/*    Serial1.print("e");
    Serial1.print(e);
    Serial1.print(" u");
    Serial1.print(u);
    Serial1.print(" k");
    Serial1.print(k1);
    Serial1.print(" kub");
    Serial1.print(kub);
    Serial1.print(" bspd");
    Serial1.print(basespeed);

    Serial1.print("L");
    Serial1.print(leftSpeed);
    Serial1.print(" R");
    Serial1.println(rightSpeed);*/
    servoSeek();
    time = millis();
  }

  if (drive)
  {
    Strela.motors(leftSpeed, rightSpeed);
  }
  else
  {
    Strela.motors(0, 0);
  }
}


void servoSeek()
{

  static int pos = 90;
  static bool direct = true;


  if ((direct) && (pos < 150))
  {
    pos += 10;
  }
  else if (pos > 30)
  {
    direct = false;
    pos -= 10;
  }
  else
  {
    direct = true;
  }

  if (analogRead(DISTANCE) > 500)
    drive = false;

  myservo.write(pos);
  
  if(Strela.buttonRead(S4))
  {
    kub-=0.000001;
  }
}



