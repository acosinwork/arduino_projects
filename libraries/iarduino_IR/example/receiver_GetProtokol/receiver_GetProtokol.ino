// Библиотека iarduino_IR для работы с ИК приёмопередатчиками разработана     http://iarduino.ru
// Для считывания данных с ИК-пульта предлагаем воспользоваться ИК-приёмником http://iarduino.ru/shop/Sensory-Datchiki/ik-priemnik-trema-modul.html

// ПРИМЕР ПОЗВОЛЯЕТ ПОЛУЧИТЬ ПРОТОКОЛ ПЕРЕДАЧИ ДАННЫХ ЛЮБОГО ПУЛЬТА

#include <iarduino_IR_RX.h>                      // Подключаем библиотеку для работы с ИК-приёмником
iarduino_IR_RX IR(7);                            // Объявляем объект IR, с указанием вывода к которому подключён ИК-приёмник
void setup(){
  Serial.begin(9600);                            // Инициируем передачу данных в монитор последовательного порта, на скорости 9600 бит/сек
  IR.begin();                                    // Инициируем работу с ИК-приёмником
}
void loop(){
  if(IR.check()){                                // Если в буфере имеются данные, принятые с пульта (была нажата кнопка)
    Serial.print("Protocol = "); Serial.println(IR.protocol());
    Serial.print("Coding = ");
    switch(IR.protocol(0)){
      case IR_PAUSE_LENGTH:      Serial.println("pauseLength");     break;
      case IR_PULSE_LENGTH:      Serial.println("pulseLength");     break;
      case IR_BIPHASIC:          Serial.println("biphasic");        break;
      case IR_BIPHASIC_INV:      Serial.println("biphasic invert"); break;
      case IR_NRC:               Serial.println("NRC");             break;
      case IR_RS5:               Serial.println("RS5");             break;
      case IR_RS5X:              Serial.println("RS5X");            break;
      case IR_RS6:               Serial.println("RS6");             break;
      default:                   Serial.println("undefined");       break;
    }                            Serial.println("================");
  }
}
