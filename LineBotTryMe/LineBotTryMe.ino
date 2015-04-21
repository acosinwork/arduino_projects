#include <EEPROM.h>
#include <Wire.h>
#include <Strela.h>
#include <LineBot.h>


#define POWER_FORWARD  85


// Коэффициент для притормаживания на поворотах
float kSpeed = 0.5;

// Коэффициенты пропорциональной, интегральной
// и дифференциальной составляющей
float kp = 0.7, ki = -0.05, kd = 0.7;

int offset = 0, oldOffset = 0, integral = 0;

LineBot ss;

void setup() {
  // put your setup code here, to run once:
  ss.begin();

  ss.sensors.attach(P6);
  ss.sensors.attach(P7);


  while (!uDigitalRead(S2))
  {
    for (int i = 0; i < ss.sensors.count(); ++i)
      ss.sensors.setBlack(i);
  }
  
  tone(BUZZER, 200, 100);
  
  while (!uDigitalRead(S4))
  {
    for (int i = 0; i < ss.sensors.count(); ++i)
      ss.sensors.setWhite(i);
  }
  
  
  tone(BUZZER, 500, 100);
  
  delay(1000);
  


  Serial.begin(9600);


  //  ss.speed(70);
}

void loop() {

    // Наш робот ездит вдоль линии
  // получить значение с аналогового датчика линии
  offset =  getLine();
  
  integral = integral + offset;
  
/*
  // Расчет интегральной составляющей, "накапливающей"
  // ошибку. Ограничим ее промежутком от -200 до 200
  integral = constrain(integral + (offset - average), 
           -200, 200);
*/

  // Расчет управляющего воздействия на моторы с учетом
  // пропорциональной, интегральной и дифференциальной
  // составляющих отклонения показания, измеренного
  // датчиком, от эталонного. Ограничим значение
  // воздействия половиной мощности двигателей.
  int u = constrain( kp * offset +
      ki * (integral) +
      kd * (offset - oldOffset),
      - POWER_FORWARD, POWER_FORWARD);

  // Расчет уменьшения мощности для торможения на
  // поворотах. Чем больше значение управляющего
  // воздействия, тем больше торможение.
  int uSpeed = abs(kSpeed * u);

  // Управление мощностью двигателей
//  powerMotor(&rightWheel, POWER_FORWARD - uSpeed + u);
 // powerMotor(&leftWheel, POWER_FORWARD - uSpeed - u);
 
 ss.speed(POWER_FORWARD - uSpeed + u,POWER_FORWARD - uSpeed - u);
    
//  Serial.print("offset: ");
//  Serial.print(offset);
//  Serial.print(" u: ");
//  Serial.println(u);
  
  // Расчет дифференциальной составляющей
  oldOffset = offset;
  delay(20);   
  
}


int getLine()
{
  return (ss.sensors.readPct(0) - ss.sensors.readPct(1));
}
