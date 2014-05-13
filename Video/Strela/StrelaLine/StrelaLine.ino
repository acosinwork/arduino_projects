#include <Wire.h>
#include <Strela.h>
//#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define IR_PIN A5


#define GPLCD_TWI_ADDR   0x21

// Движение робота по линии под управлением
// PID-регулятора на основе данных аналогового
// датчика линии

// Пин для аналогового датчика линии
#define ANALOG_IN_PIN   A2



// мощность двигателей [0..255]. Т.е. сигнал
// для Motor Shield (вперед)
#define POWER_FORWARD  60

// Коэффициент для притормаживания на поворотах
float kSpeed = 1.5;

// Коэффициенты пропорциональной, интегральной
// и дифференциальной составляющей
float kp = 0.07, ki = -0.005, kd = 0.07;

/*
 * Структура, описывающая мотор с колесом

struct Wheel {
  int directionPin;
  int speedPin;
};

// Экземпляры колёс. У нас их 2: левое и правое
Wheel leftWheel, rightWheel;
*/


// Переменные для PID-регулятора
int offset = 512, oldOffset = 512, integral = 0;

// показания датчика линии в среднем положении
int average = 700;

LiquidCrystal_I2C lcd(0x21, 4, 5, 6, 0, 1, 2, 3);

void setup()
{
  Strela.begin();
  Strela.motorConnection(1,1);

//Strela.motors(100,200);

  lcd.begin(8, 2);
//  tone(12, 2000, 50);
//  noTone(12);
  // Иницииализация последовательного порта


  Serial1.begin(9600);
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
  lcd.clear();
  lcd.home();
  lcd.print("GO!!!!11");

  while (digitalRead(IR_PIN))
  {
    ;
  }

}


void loop()
{
  // Наш робот ездит вдоль линии
  // получить значение с аналогового датчика линии
  offset =  getLine();

  allPrint(offset);

  // Расчет интегральной составляющей, "накапливающей"
  // ошибку. Ограничим ее промежутком от -200 до 200
  integral = constrain(integral + (offset - average),
                       -200, 200);

  // Расчет управляющего воздействия на моторы с учетом
  // пропорциональной, интегральной и дифференциальной
  // составляющих отклонения показания, измеренного
  // датчиком, от эталонного. Ограничим значение
  // воздействия половиной мощности двигателей.
  int u = constrain( kp * (offset - average) +
                     ki * (integral) +
                     kd * (offset - oldOffset),
                     - POWER_FORWARD / 2, POWER_FORWARD / 2);

  // Расчет уменьшения мощности для торможения на
  // поворотах. Чем больше значение управляющего
  // воздействия, тем больше торможение.
  int uSpeed = abs(kSpeed * u);

  // Управление мощностью двигателей
  int leftWheel = POWER_FORWARD - uSpeed - u;
  int rightWheel = POWER_FORWARD - uSpeed + u;

  Strela.motors(leftWheel, rightWheel);
  
  motorPrint(leftWheel, rightWheel);



  /*
    powerMotor(&rightWheel, POWER_FORWARD - uSpeed + u);
    powerMotor(&leftWheel, POWER_FORWARD - uSpeed - u);
  */
  //  Serial.print("offset: ");
  //  Serial.print(offset);
  //  Serial.print(" u: ");
  //  Serial.println(u);

  // Расчет дифференциальной составляющей
  oldOffset = offset;
  delay(20);
    if (Strela.buttonRead(S2))
    {
      Strela.motors(0, 0);
      while (true)
      {
        ;
      }
    }
}
/*
void stopMotors()
{
    // Остановить моторы
    powerMotor(&rightWheel, 0);
    powerMotor(&leftWheel, 0);
}
*/
// Управление мощностью двигателя указанного колеса
/*
void powerMotor(struct Wheel* wheel, int value)
{
  // выставляем направление двигателя
  digitalWrite(wheel->directionPin,  value > 0);
  // выставляем мощность двигателя
  analogWrite(wheel->speedPin, abs(value));
}
*/

// Получить данные с датчика линии
int getLine()
{
  unsigned int sensorValue = 0;
  sensorValue = analogRead(ANALOG_IN_PIN);
  // переводим показания датчика
  return (1024 - sensorValue);
}

void allPrint(int data)
{

  Serial1.println(data);

}

void motorPrint(int left, int right)
{
  lcd.clear();
  lcd.home();
  lcd.print("L: ");
  lcd.print(left);
  lcd.setCursor(0, 1);
  lcd.print("R: ");
  lcd.print(right);

}
