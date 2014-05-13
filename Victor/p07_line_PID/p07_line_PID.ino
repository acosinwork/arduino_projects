// Движение робота по линии под управлением
// PID-регулятора на основе данных аналогового
// датчика линии

// Пин для аналогового датчика линии
#define ANALOG_IN_PIN   A0

// Пины для управления Motor Shield
#define SPEED_LEFT_PIN    6
#define SPEED_RIGHT_PIN   5 
#define DIR_LEFT_PIN      7
#define DIR_RIGHT_PIN     4

// мощность двигателей [0..255]. Т.е. сигнал
// для Motor Shield (вперед)
#define POWER_FORWARD  85

// Коэффициент для притормаживания на поворотах
float kSpeed = 0.5;

// Коэффициенты пропорциональной, интегральной
// и дифференциальной составляющей
float kp = 0.07, ki = -0.005, kd = 0.07;

/*
 * Структура, описывающая мотор с колесом
 */
struct Wheel {
  int directionPin;
  int speedPin;
};

// Экземпляры колёс. У нас их 2: левое и правое
Wheel leftWheel, rightWheel;

// Переменные для PID-регулятора
int offset = 512, oldOffset = 512, integral = 0;

// показания датчика линии в среднем положении
int average = 700;
  

void setup()
{  
  // все пины для управления моторами — выходы
  pinMode(SPEED_LEFT_PIN, OUTPUT);
  pinMode(SPEED_RIGHT_PIN, OUTPUT);
  pinMode(DIR_LEFT_PIN, OUTPUT);
  pinMode(DIR_RIGHT_PIN, OUTPUT);
  
  // инициализация пинов моторов на колесах
  leftWheel.directionPin = DIR_LEFT_PIN;
  leftWheel.speedPin = SPEED_LEFT_PIN;
  rightWheel.directionPin = DIR_RIGHT_PIN;
  rightWheel.speedPin = SPEED_RIGHT_PIN;
  
  // Иницииализация последовательного порта
  Serial.begin(9600);
}

void loop()
{
  // Наш робот ездит вдоль линии
  // получить значение с аналогового датчика линии
  offset =  getLine();

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
  powerMotor(&rightWheel, POWER_FORWARD - uSpeed + u);
  powerMotor(&leftWheel, POWER_FORWARD - uSpeed - u);
    
//  Serial.print("offset: ");
//  Serial.print(offset);
//  Serial.print(" u: ");
//  Serial.println(u);
  
  // Расчет дифференциальной составляющей
  oldOffset = offset;
  delay(20);   
}

void stopMotors()
{
    // Остановить моторы
    powerMotor(&rightWheel, 0);
    powerMotor(&leftWheel, 0);
}

// Управление мощностью двигателя указанного колеса
void powerMotor(struct Wheel* wheel, int value)
{
  // выставляем направление двигателя
  digitalWrite(wheel->directionPin,  value > 0);
  // выставляем мощность двигателя
  analogWrite(wheel->speedPin, abs(value));
}

// Получить данные с датчика линии
int getLine()
{
  unsigned int sensorValue = 0;
  sensorValue = analogRead(ANALOG_IN_PIN);
  // переводим показания датчика
  return (1024 - sensorValue);
}
