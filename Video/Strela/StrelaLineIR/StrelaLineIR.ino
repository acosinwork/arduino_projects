#include <Wire.h>
#include <Strela.h>
#include <Servo.h>


/*
Программа для использования
IR Kit For Arduino Model: DFR0107
Нажимаемые кнопки на ПДУ передаются
через Serial monitor на
скорости 9600 бод.
*/

// Кол-во бит передаваемых ПДУ
#define IR_BIT_LENGTH 32
// 32 бита делятся на две группы
// по 15 бит, игнорируюя два неизменных
// бита посередине
#define FirstLastBit 15
// временной интервал (мкс) кодирования двоичной "1"
#define BIT_1 1500
// временной интервал (мкс) кодирования двоичного "0"
#define BIT_0 450
// временной интервал (мкс) стартового бита
#define BIT_START 4000

// Порт подключения ИК-приемника
#define IR_PIN A5

// если 1, то выводить длину импульса потока
// данных в мкс
int debug = 0;
// если 1, то вывести декодированные
// верифицированные числа
int outputVerify = 0;
// если 1, то вывести число декодированной кнопки
int outputKey = 0;
// заголовок для верификация ПДУ от DFR0107
// 11111100000000 (уникальный для разных ПДУ)
int remoteVerify = 16128;
unsigned long time;

bool action = false;

byte servoPos = 20;
bool servoDirection = true;

Servo myservo;  // create servo object to control a servo


void setup() {
  myservo.attach(11);

  pinMode(IR_PIN, INPUT);
  time = millis();
    Serial.begin(9600);
  Strela.begin();
}

void loop() {

  if ((millis() - time) > 130) // задержка для пропуска повтора нажатия на кнопку
  {
    // получить код нажатой кнопки ПДУ
    int key = getIrKey();
    // Распознать кнопку по коду
    doResponse(key);
    time = millis();
    servoSweep();
  }

  if (action)
  {
    doTheAction();
  }



}

/*
Ожидает нажатия кнопки на ПДУ, возвращает
число, соответствующее нажатой кнопке
*/
int getIrKey()
{
  int pulse[IR_BIT_LENGTH];
  int bits[IR_BIT_LENGTH];

  // пока "свой" ПДУ не передал данные
  do {
    // ожидание стартового бита
    do {}
    while (pulseIn(IR_PIN, HIGH) < BIT_START);

    // считать интервалы сигналов в массив
    readPulse(pulse);
    // декодировать интервалы в биты
    pulseToBits(pulse, bits);
    // задержка для окончания передачи сигналов
    delay(60);
  }
  while (!verifyRC(bits));

  // конвертировать двоичные биты в целое число
  return bitsToInt(bits);
}


/*
При приеме сигналов от ПДУ использует команду
pulseIn для измерения временных интервалов (мкс)
и записи их в массив
*/
void readPulse(int pulse[])
{
  for (int i = 0; i < IR_BIT_LENGTH; i++)
  {
    pulse[i] = pulseIn(IR_PIN, HIGH);
  }
}

/*
Из массива с длиннами импульсов конвертирует значения
в массив со значениями бит.
Короткие ИК-импульсы декодируются как двоичный "0",
длинные - как двоичная "1"
*/
void pulseToBits(int pulse[], int bits[])
{
  // если режим debug, то выводить подробности
  if (debug) {
    Serial.println("-----");
  }
  for (int i = 0; i < IR_BIT_LENGTH; i++)
  {
    if (debug) {
      Serial.println(pulse[i]);
    }
    //это 1?
    if (pulse[i] > BIT_1)
    {
      bits[i] = 1;
    }
    //это 0?
    else if (pulse[i] > BIT_0)
    {
      bits[i] = 0;
    }
    else //некорректные данные
    {
      Serial.println("Error");
    }
  }
}

/*
Проверка заголовка ПДУ (первые 14 бит)
*/
boolean verifyRC(int bits[])
{
  int result = 0;
  int seed = 1;

  //Конвертируем двоичные биты в целое число
  for (int i = 0 ; i < (FirstLastBit) ; i++)
  {
    if (bits[i] == 1)
    {
      result += seed;
    }
    seed *= 2;
  }
  // если режим outputVerify, то выводить данные
  if (outputVerify)
  {
    Serial.print("Remote ");
    Serial.print(result);
    Serial.println(" verification code");
  }
  // Если заголовок для верификации ПДУ совпал, то значит
  // сигнал от "своего" ПДУ
  return (remoteVerify == result);
}


/*
Конвертирует массив с двоичными данными в целое число
*/

int bitsToInt(int bits[])
{
  int result = 0;
  int seed = 1;

  for (int i = (IR_BIT_LENGTH - FirstLastBit) ;
       i < IR_BIT_LENGTH ; i++)
  {
    if (bits[i] == 1)
    {
      result += seed;
    }
    seed *= 2;
  }
  return result;
}


/*
Определяет какая кнопка на ПДУ была нажата
*/

void doResponse(int key)
{
  // если режим вывода кода
  if (outputKey)
  {
    Serial.print("Key ");
    Serial.println(key);
  }

  switch (key)
  {
    case 32640:
      Serial.println("POWER");
      action = !action;
      break;

    case 32385:
      Serial.println("FUNC/STOP");
      break;

    case 32130:
      Serial.println("|<<");
      break;

    case 32002:
      Serial.println(">||");
      break;

    case 31875:
      Serial.println(">>|");
      break;

    case 32512:
      Serial.println("VOL+");
      break;

    case 31492:
      Serial.println("VOL-");
      break;

    case 31620:
      Serial.println("v");
      break;

    case 31365:
      Serial.println("^");
      break;

    case 30982:
      Serial.println("EQ");
      break;

    case 30855:
      Serial.println("ST/REPT");
      break;

    case 31110:
      Serial.println("0");
      break;

    case 30600:
      Serial.println("1");
      break;

    case 30472:
      Serial.println("2");
      break;

    case 30345:
      Serial.println("3");
      break;

    case 30090:
      Serial.println("4");
      break;

    case 29962:
      Serial.println("5");
      break;

    case 29835:
      Serial.println("6");
      break;

    case 29580:
      Serial.println("7");
      break;

    case 29452:
      Serial.println("8");
      break;

    case 29325:
      Serial.println("9");
      break;

    default:
      {
        Serial.print("Key ");
        Serial.print(key);
        Serial.println(" not programmed");
      }
      break;
  }
}


void doTheAction()
{

}

void servoSweep()
{

  if (!servoDirection)
  {
    if (servoPos < 160)
      servoPos += 10;
  }
  else
  {
    if (servoPos > 20)
      servoPos -= 10;
  }
  if ((servoPos = 20) || (servoPos = 160))
    servoDirection = !servoDirection;

  myservo.write(servoPos);
  
  Serial.println(servoPos);
  Serial.println(servoDirection);
  

}






