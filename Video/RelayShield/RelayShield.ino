#define DOTS              1
#define DASHES            3
#define SPACE             5
#define MORSE_LENGTH_MASK 0b00000111

int dotLengthMillis = 100;
bool powerLight = false;
//bool serialMode=false;

byte morseCharCode[] =
{
  // _ - 1; . - 0
  //_____111; _ - code, 1 - symbolCount
  0b01000010, // A ._
  0b10000100, // B _...
  0b10100100, // C _._.
  0b10000011, // D _..
  0b00000001, // E .
  0b00100100, // F .._.
  0b11000011, // G _ _.
  0b00000100, // H ....
  0b00000010, // I ..
  0b01110100, // J ._ _ _
  0b10100011, // K _._
  0b01000100, // L ._..
  0b11000010, // M _ _
  0b10000010, // N _.
  0b11100011, // O _ _ _
  0b01100100, // P ._ _.
  0b11010100, // Q _ _ . _
  0b01000011, // R ._.
  0b00000011, // S ...
  0b10000001, // T _
  0b00100011, // U .._
  0b00010100, // V ..._
  0b01100011, // W ._ _
  0b10010100, // X _.._
  0b10110100, // Y _._ _
  0b11000100 // Z _ _..
};

byte morseNumCode[] =
{
  0b11111101, // 0 _ _ _ _ _
  0b01111101, // 1 ._ _ _ _
  0b00111101, // 2 .._ _ _
  0b00011101, // 3 ..._ _
  0b00001101, // 4 ...._
  0b00000101, // 5 .....
  0b10000101, // 6 _....
  0b11000101, // 7 _ _...
  0b11100101, // 8 _ _ _..
  0b11110101  // 9 _ _ _ _.
};

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
#define IR_PIN 10

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

void setup() {
  // put your setup code here, to run once:

  for (int i = 4; i < 10; ++i)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  digitalWrite(9, HIGH);

  pinMode(IR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0)
  {
    // read the oldest byte in the serial buffer:
    byte serialByte=Serial.read();
    parseCodeToMorse(serialByte);
  }
  else
  {
    // получить код нажатой кнопки ПДУ
    int key = getIrKey();

    // Распознать кнопку по коду
    doResponse(key);
    // задержка для пропуска повтора нажатия на кнопку
    delay(130);
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
      allPinWrite(powerLight);
      powerLight = !powerLight;
      break;

    case 32385:
      Serial.println("FUNC/STOP");
      break;

    case 32130:
      Serial.println("|<<");
      break;

    case 32002:
      Serial.println(">||");

      allPinWrite(LOW);

      delay(500);

      parseCodeToMorse('h');
      parseCodeToMorse('e');
      parseCodeToMorse('l');
      parseCodeToMorse('l');
      parseCodeToMorse('o');
      parseCodeToMorse(' ');
      parseCodeToMorse('w');
      parseCodeToMorse('o');
      parseCodeToMorse('r');
      parseCodeToMorse('l');
      parseCodeToMorse('d');
      
      parseCodeToMorse(' ');

      allPinWrite(HIGH);


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
      digitalWrite(7, !digitalRead(7));
      break;

    case 30472:
      Serial.println("2");
      digitalWrite(6, !digitalRead(6));

      break;

    case 30345:
      Serial.println("3");
      digitalWrite(5, !digitalRead(5));

      break;

    case 30090:
      Serial.println("4");
      digitalWrite(4, !digitalRead(4));

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


//morse
void morsePrint(byte printByte)
{
  byte morseCharLength = printByte & MORSE_LENGTH_MASK;
  int currentDelay;

  for (byte i = 0; i < morseCharLength; i++)
  {
    if (printByte & 0b10000000)
    {
      currentDelay = DASHES * dotLengthMillis;
      //      Serial.print('-');
    }
    else
    {
      currentDelay = DOTS * dotLengthMillis;
      //      Serial.print('.');
    }
    // do action
    allPinWrite(HIGH);
    //    tone(A5, 800, currentDelay);
    delay(currentDelay);
    allPinWrite(LOW);

    //pause between dots or dashes
    delay(dotLengthMillis);

    printByte <<= 1;
  }

  //pause between characters (3 dot length, but we do one dot pause after last dot)

  delay(2 * dotLengthMillis);


}

void parseCodeToMorse(byte incomingByte)
{
  if ((incomingByte > 96) && (incomingByte < 123)) // small char
  {
    incomingByte = incomingByte - 32; // convert to BIG CHAR
  }

  if ((incomingByte > 64) && (incomingByte < 91)) // BIG CHAR
  {
    incomingByte = incomingByte - 65; // to morseCharCode adress

    morsePrint(morseCharCode[incomingByte]);
  }
  else if ((incomingByte > 47) && (incomingByte < 58))
  {
    morsePrint(morseNumCode[incomingByte]);
  }
  else if ((incomingByte = ' ') || (incomingByte = '/n'))
  {
    //pause between words (5 dot length, but we do two dot pause after last character)

    delay(3 * dotLengthMillis);
  }

}

void allPinWrite(bool state)
{
  for (int i = 4; i < 8; ++i)
  {
    digitalWrite(i, state);
  }
}
