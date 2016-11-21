// библитоека для работы I²C
#include <Wire.h>
// библиотека для работы с модулями IMU
#include <troyka-imu.h>
// библиотека для работы с GPRS устройством
#include <GPRS_Shield_Arduino.h>

// библиотека для эмуляции Serial порта
// она нужна для работы библиотеки GPRS_Shield_Arduino
#include <SoftwareSerial.h>

#define GPRS_BUFFER_LENGTH    46
#define TELEPHONE_NUM_LENGTH  12
#define ROTATE_SPEED_TRESHOLD 10

#define OUTCOMING_CALL_TIMEOUT 5000
#define OUTCOMING_CALL_TRESHOLD 500

#define INDICATOR_PIN 13

char fathersNumber[] = "+79091589013";

char incomingPhoneNumber[GPRS_BUFFER_LENGTH];

// создаём объект класса GPRS и передаём ему скорость 9600 бод
// с помощью него будем давать команды GPRS шилду
GPRS gprs(6, 5, 9600);


// создаём объект для работы с гироскопом
Gyroscope gyro;

unsigned long waitTime = 0;

unsigned long rotateTime = 0;

int rotateCounter = 0;

void setup()
{
  //настраиваем пин №13 в режим выхода,
  pinMode(INDICATOR_PIN, OUTPUT);
  // подаём на пин 13 «низкий уровень»
  // включаем GPRS шилд
  gprs.powerUpDown();
  // открываем последовательный порт для мониторинга действий в программе
  /*  Serial.begin(9600);
    while (!Serial) {
      // ждём, пока не откроется монитор последовательного порта
      // для того, чтобы отследить все события в программе
    }
  */
  // проверяем есть ли связь с GPRS устройством
  while (!gprs.init()) {
    digitalWrite(INDICATOR_PIN, !digitalRead(INDICATOR_PIN)); // завелся ли шилд?

    // если связи нет, ждём 1 секунду
    // и выводим сообщение об ошибке
    // процесс повторяется в цикле,
    // пока не появится ответ от GPRS устройства
    delay(1000);
    //    Serial.print("Init error\r\n");
  }
  digitalWrite(INDICATOR_PIN, LOW);

  /*  // вывод об удачной инициализации GPRS Shield
    Serial.println("GPRS init success");
    // сообщаем об ожидании звонка
    Serial.println("Wait to call ");
  */
  //Запускаем гироскоп
  gyro.begin();

}

void loop()
{

  if (gyroCall()) {
    gprs.callUp(fathersNumber);
    calling();
  }

  if (millis() - waitTime > 1000) {
    incomingCall();
    waitTime = millis();
  }
}

void incomingCall()
{
  // ожидаем звонка
  if (gprs.ifcallNow()) {
    // если поступает входящий звонок,
    // подаём на пин 13 «высокий уровень», чтобы
    // зажечь встроенный на Iskra светодиод
    digitalWrite(INDICATOR_PIN, HIGH);
    // выводим сообщение о входящем вызове
    //    Serial.println("Incoming call");

    gprs.isCallActive(incomingPhoneNumber);

    if (!strncmp(incomingPhoneNumber, fathersNumber, TELEPHONE_NUM_LENGTH)) {
      // по истечении 3 секунд берём трубку
      delay(3000);
      gprs.answer();
      // выводим сообщение о начале разговора
      delay(1000);
      //      Serial.println("Call a conversation");

      calling();

    } else {
      gprs.callEnd();
      //      Serial.println("Call over");
    }
  }
}

bool gyroCall()
{
  float bearRotate = abs(gyro.readX_DegPerSec()) + abs(gyro.readY_DegPerSec()) + abs(gyro.readZ_DegPerSec());

  if ((bearRotate > ROTATE_SPEED_TRESHOLD)) {// && (millis() - rotateTime < 3000)) {
    rotateTime = millis();
    ++rotateCounter;

    analogWrite(INDICATOR_PIN, map(rotateCounter, 0, OUTCOMING_CALL_TRESHOLD, 0, 255));

    //    Serial.println(rotateCounter);

  } else if (millis() - rotateTime > OUTCOMING_CALL_TIMEOUT) {
    digitalWrite(INDICATOR_PIN, LOW);
    rotateCounter = 0;
  }

  return (rotateCounter > OUTCOMING_CALL_TRESHOLD);
}

void calling()
{
  while (!gprs.ifcallEnd()) {
    // моргаем и ждём пока месть абонент не положит трубку
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(50);
    digitalWrite(INDICATOR_PIN, LOW);
    delay(200);
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(50);
    digitalWrite(INDICATOR_PIN, LOW);
  }
}


