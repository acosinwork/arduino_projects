#include <Wire.h>
#include <Strela.h>
#include <LiquidCrystal_I2C.h>
//#include <Servo.h>

#define ANALOG_IN_PIN   P7
#define IR_PIN A5

#define SERVO 11
#define DISTANCE A3


#define K1 0.05//0.015
#define KUB 0.000013//0.00000017

unsigned long time;

//Servo myservo;

//bool drive = true;

LiquidCrystal_I2C lcd(0x21, 4, 5, 6, 0, 1, 2, 3);

int basespeed = 40;
int average = 0;

float k1 = 1.0;//0.05;//0.015
float kub = 0.0;//0.000001;//0.00000017

void setup() {
uPinMode(P9, INPUT);
lcd.begin(8, 2);


  int black, white;

  while (!uDigitalRead(S1))//digitalRead(IR_PIN))
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

  while (!uDigitalRead(S1))//digitalRead(IR_PIN))
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

  while ((digitalRead(IR_PIN)) && !uDigitalRead(S3))
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
 int e = analogRead(ANALOG_IN_PIN) - average; //+(1-digitalRead(11))*POVOROT;//-diff;
  float u = K1 * e  + (KUB) * pow(e, 3) ;
  int leftSpeed = basespeed + int(u);
  int rightSpeed = basespeed - int(u);



  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
    if (drive)
  {
    drive(leftSpeed, rightSpeed);
  }
  else
  {
    drive(0, 0);
  }
}
