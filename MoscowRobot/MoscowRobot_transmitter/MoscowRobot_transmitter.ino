#include <TimerOne.h> // скачать библиотеку http://playground.arduino.cc/Code/Timer1

#include <SoftwareSerial.h>

// не знаю, на какой скорости работает ваше rs-232 устройство, допустим на этой
#define RS232_SPEED 9600 // Если не 9600 - просто поменяйте

#define IR_SPEED 1200    // ИК приёмник быстро не умеет работать.

#define MANIPULATE 7 // сюда идёт анод светодиода. Используем Arduino Uno (это важно - PORTD)
#define FREQ 36000   // это частота ИК-приемника. Если у вас другой - меняйте, но рекомендую этот (частота совпадает хорошо)
#define MAN_FREQ (1000000.0/(FREQ*2)) // миллион микросекунд в секунде, должен быть float


uint8_t transmitInterval = 30; // ИК-приёмник не любит много данных без пауз
unsigned long time;

char recievedSymbol;

SoftwareSerial IRSerial(6, 5); // RX, TX. Ногу 6 не используем. К 5-й подключаем катод светодиода


void setup() {
  pinMode(MANIPULATE, OUTPUT);

  Timer1.initialize(MAN_FREQ); // 72000 Hz, 14 microseconds, 36 kHz*2
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  IRSerial.begin(IR_SPEED); 

  Serial.begin(RS232_SPEED);

  time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available())
    recievedSymbol = Serial.read(); // Предполагаю, что команда управления - один символ. Если не так - сообщите

  if ((millis() - time) > transmitInterval)
  {
    Serial.write(recievedSymbol); // Отправляем пришедший на RS-232 символ через ИК-свеотдиод раз в 30 мс
    time = millis();
  }

}

void timerIsr()
{
  //работаем быстро - напрямую с портами
  PORTD ^= _BV(MANIPULATE);
}

