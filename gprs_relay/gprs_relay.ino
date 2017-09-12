// библиотека для работы с GPRS устройством
#include <GPRS_Shield_Arduino.h>

// библиотека для эмуляции Serial порта
// она нужна для работы библиотеки GPRS_Shield_Arduino
#include <SoftwareSerial.h>

// длина сообщения  --- команды чуть поменяются --- везде будет цифра в данном варианте
#define MESSAGE_LENGTH 160
// текст сообщения о включении розетки
#define MESSAGE_ON1  "On1"
// текст сообщения о выключении розетки
#define MESSAGE_OFF1  "Off1"
// текст сообщения о состоянии розетки
#define MESSAGE_STATE1  "State1"

// текст сообщения о включении розетки
#define MESSAGE_ON2  "On2"
// текст сообщения о выключении розетки
#define MESSAGE_OFF2  "Off2"
// текст сообщения о состоянии розетки
#define MESSAGE_STATE2  "State2"

// текст сообщения о включении розетки
#define MESSAGE_ON3  "On3"
// текст сообщения о выключении розетки
#define MESSAGE_OFF3  "Off3"
// текст сообщения о состоянии розетки
#define MESSAGE_STATE3  "State3"

// пин, к которому подключено реле
#define RELAY1 5

// Пины на вторую и третью реле -- там по шилду смотри
#define RELAY2 6
#define RELAY3 7

unsigned long resetTime = 86400000; //перезагружаем раз в сутки

// текст сообщения
char message[MESSAGE_LENGTH];
// номер, с которого пришло сообщение
char phone[16];
// дата отправки сообщения
char datetime[24];

bool stateRelay[3] = {false, false, false}; // тут переделал на массив для трех состояний реле менять не надо

// создаём объект класса GPRS и передаём ему скорость 9600 бод;
// с помощью него будем давать команды GPRS шилду
GPRS gprs(9600);

void setup()
{
  // настраиваем пин реле в режим выхода,
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  // подаём на пин реле «низкий уровень» (размыкаем реле)
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);
  // включаем GPRS-шилд
  gprs.powerUpDown();
  // открываем последовательный порт для мониторинга действий в программе
  //Serial.begin(9600);

  // Убрал это. Василий
  /*
    while (!Serial) {
      // ждём, пока не откроется монитор последовательного порта
      // для того, чтобы отследить все события в программе
    }
  */
  // проверяем, есть ли связь с GPRS-устройством
  init();
  // вывод об удачной инициализации GPRS Shield
  //Serial.println("GPRS init success");
  //  digitalWrite(RELAY, HIGH);
}

void loop()
{
  // если пришло новое сообщение
  if (gprs.ifSMSNow()) {
    // читаем его
    // если есть хотя бы одно непрочитанное сообщение,
    // читаем его
    gprs.readSMS(message, phone, datetime);

    // выводим номер, с которого пришло смс
    //Serial.print("From number: ");
    //Serial.println(phone);

    // выводим дату, когда пришло смс
    //Serial.print("Datetime: ");
    //Serial.println(datetime);

    // выводим текст сообщения
    //Serial.print("Recieved Message: ");
    //Serial.println(message);
    // вызываем функцию изменения состояния реле
    // в зависимости от текста сообщения
    setRelay(phone, message);
  }
  if (millis() > resetTime)
    software_Reset()

  }

void setRelay(char f_phone[], char f_message[])
{
  if (strcmp(f_message, MESSAGE_ON1) == 0) {
    // если сообщение — с текстом «On1»,
    // выводим сообщение в Serial
    // и подаём на замыкаем реле
    //Serial.println("OK! Power is On 1");
    digitalWrite(RELAY1, HIGH);
    stateRelay[0] = true;
    // на номер, с которого пришёл запрос,
    // отправляем смс с текстом о включении питания
    gprs.sendSMS(f_phone, "Power is On 1");
  } else if (strcmp(f_message, MESSAGE_OFF1) == 0) {
    // если пришло сообщение с текстом «Off 1»,
    // выводим сообщение в Serial
    // и размыкаем реле
    //Serial.println("OK! Power is Off 1");
    digitalWrite(RELAY1, LOW);
    stateRelay[0] = false;
    // на номер, с которого пришёл запрос
    // отправляем смс с текстом о выключении питания
    gprs.sendSMS(f_phone, "Power is Off 1");
  } else if (strcmp(f_message, MESSAGE_STATE1) == 0) {
    // если пришло сообщение с текстом «State 1»,
    // отправляем сообщение с состоянием реле
    if (stateRelay[0]) {
      //Serial.println("State: Power is On 1");
      gprs.sendSMS(f_phone, "Power is On 1");
    } else {
      //Serial.println("State: Power is Off 1");
      gprs.sendSMS(f_phone, "Power is Off 1");
    }
  } else if (strcmp(f_message, MESSAGE_ON2) == 0) {
    // если сообщение — с текстом «On2»,
    // выводим сообщение в Serial
    // и подаём на замыкаем реле
    //Serial.println("OK! Power is On 2");
    digitalWrite(RELAY2, HIGH);
    stateRelay[1] = true;
    // на номер, с которого пришёл запрос,
    // отправляем смс с текстом о включении питания
    gprs.sendSMS(f_phone, "Power is On 2");
  } else if (strcmp(f_message, MESSAGE_OFF2) == 0) {
    // если пришло сообщение с текстом «Off 2»,
    // выводим сообщение в Serial
    // и размыкаем реле
    //Serial.println("OK! Power is Off 2");
    digitalWrite(RELAY2, LOW);
    stateRelay[1] = false;
    // на номер, с которого пришёл запрос
    // отправляем смс с текстом о выключении питания
    gprs.sendSMS(f_phone, "Power is Off 2");
  } else if (strcmp(f_message, MESSAGE_STATE2) == 0) {
    // если пришло сообщение с текстом «State 2»,
    // отправляем сообщение с состоянием реле
    if (stateRelay[1]) {
      //Serial.println("State: Power is On 2");
      gprs.sendSMS(f_phone, "Power is On 2");
    } else {
      //Serial.println("State: Power is Off 2");
      gprs.sendSMS(f_phone, "Power is Off 2");
    }
  } else if (strcmp(f_message, MESSAGE_ON3) == 0) {
    // если сообщение — с текстом «On3»,
    // выводим сообщение в Serial
    // и подаём на замыкаем реле
    //Serial.println("OK! Power is On 3");
    digitalWrite(RELAY3, HIGH);
    stateRelay[2] = true;
    // на номер, с которого пришёл запрос,
    // отправляем смс с текстом о включении питания
    gprs.sendSMS(f_phone, "Power is On 3");
  } else if (strcmp(f_message, MESSAGE_OFF3) == 0) {
    // если пришло сообщение с текстом «Off 3»,
    // выводим сообщение в Serial
    // и размыкаем реле
    //Serial.println("OK! Power is Off 3");
    digitalWrite(RELAY3, LOW);
    stateRelay[2] = false;
    // на номер, с которого пришёл запрос
    // отправляем смс с текстом о выключении питания
    gprs.sendSMS(f_phone, "Power is Off 3");
  } else if (strcmp(f_message, MESSAGE_STATE3) == 0) {
    // если пришло сообщение с текстом «State 3»,
    // отправляем сообщение с состоянием реле
    if (stateRelay[2]) {
      //Serial.println("State: Power is On 3");
      gprs.sendSMS(f_phone, "Power is On 3");
    } else {
      //Serial.println("State: Power is Off 3");
      gprs.sendSMS(f_phone, "Power is Off 3");
    }
  } else {
    // если сообщение содержит неизвестный текст,
    // отправляем сообщение с текстом об ошибке
    //Serial.println("Error... unknown command!");
    gprs.sendSMS(f_phone, "Error...unknown command!");
  }
}

void init() {
  while (!gprs.init()) {
    // если связи нет, ждём 1 секунду
    // и выводим сообщение об ошибке;
    // процесс повторяется в цикле,
    // пока не появится ответ от GPRS устройства
    delay(1000);
    //Serial.print("Init error\r\n");
  }
}

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  asm volatile ("  jmp 0");
}
