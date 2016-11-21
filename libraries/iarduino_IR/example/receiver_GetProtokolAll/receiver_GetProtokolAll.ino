// Библиотека iarduino_IR для работы с ИК приёмопередатчиками разработана     http://iarduino.ru
// Для считывания данных с ИК-пульта предлагаем воспользоваться ИК-приёмником http://iarduino.ru/shop/Sensory-Datchiki/ik-priemnik-trema-modul.html

// ПРИМЕР ПОЗВОЛЯЕТ ПОЛУЧИТЬ ПОЛНУЮ ИНФОРМАЦИЮ О ТИПЕ ПРОТОКОЛА ПЕРЕДАЧИ ДАННЫХ

#include <iarduino_IR_RX.h>                                                 // Подключаем библиотеку для работы с ИК-приёмником
iarduino_IR_RX IR(7);                                                       // Объявляем объект IR, с указанием вывода к которому подключён ИК-приёмник
void setup(){
  Serial.begin(9600);                                                       // Инициируем передачу данных в монитор последовательного порта, на скорости 9600 бит/сек
  IR.begin();                                                               // Инициируем работу с ИК-приёмником
}
void loop(){
  if(IR.check()){                                                           // Если в буфере имеются данные, принятые с пульта (была нажата кнопка)
                                                                               Serial.println("====================================");
    Serial.print("Data     = "); Serial.print(IR.data, HEX);                   Serial.println("\t");
    Serial.print("Length   = "); Serial.print(IR.length);                      Serial.println("\tbits");
                                                                               Serial.println("====================================");
                                                                               Serial.println("PROTOCOL:");
    Serial.print("String   = "); Serial.print(IR.protocol());                  Serial.println("\t");
    Serial.print("Coding   = "); switch(IR.protocol(0)){case IR_PAUSE_LENGTH: Serial.print("pause length"); break; case IR_PULSE_LENGTH: Serial.print("pulse length"); break; case IR_BIPHASIC: Serial.print("biphasic"); break; case IR_BIPHASIC_INV: Serial.print("biphasic invert"); break; case IR_NRC: Serial.print("NRC"); break; case IR_RS5: Serial.print("RS5"); break; case IR_RS5X: Serial.print("RS5X"); break; case IR_RS6: Serial.print("RS6"); break; default: Serial.print("Undefined"); break;} Serial.println("\t");
    Serial.print("F        = "); Serial.print(IR.protocol(1));                 Serial.println("\tkHz");
                                                                               Serial.println("====================================");
                                                                               Serial.println("PACKET:");
    Serial.print("Inform   = "); Serial.print(IR.protocol(2));                 Serial.println("\tbits");
    Serial.print("Repet    = "); Serial.print(IR.protocol(3));                 Serial.println("\tbits");
    Serial.print("RepType  = "); switch(IR.protocol(19)){case 0: Serial.print("No"); break; case 1: Serial.print("inverse bits"); break; case 2: Serial.print("identichen"); break; case 3: Serial.print("unikalen"); break; default: Serial.print("undefined"); break;} Serial.println("\t");
    Serial.print("Pause    = "); Serial.print(IR.protocol(4));                 Serial.println("\tms");
                                                                               Serial.println("====================================");
                                                                               Serial.println("BIT START:");
    Serial.print("Pulse    = "); Serial.print(IR.protocol(5));                 Serial.println("\tmks");
    Serial.print("Pause    = "); Serial.print(IR.protocol(6));                 Serial.println("\tmks");
    Serial.print("Present  = "); Serial.print(IR.protocol(16)?"true":"false"); Serial.println("\t");
                                                                               Serial.println("====================================");
                                                                               Serial.println("BIT STOP:");
    Serial.print("Pulse    = "); Serial.print(IR.protocol(7));                 Serial.println("\tmks");
    Serial.print("Pause    = "); Serial.print(IR.protocol(8));                 Serial.println("\tmks");
    Serial.print("Present  = "); Serial.print(IR.protocol(17)?"true":"false"); Serial.println("\t");
                                                                               Serial.println("====================================");
                                                                               Serial.println("BIT TOGGLE:");
    Serial.print("Pulse    = "); Serial.print(IR.protocol(9));                 Serial.println("\tmks");
    Serial.print("Pause    = "); Serial.print(IR.protocol(10));                Serial.println("\tmks");
    Serial.print("Position = "); Serial.print(IR.protocol(11));                Serial.println("\tbit");
    Serial.print("Present  = "); Serial.print(IR.protocol(18)?"true":"false"); Serial.println("\t");
                                                                               Serial.println("====================================");
                                                                               Serial.println("BITS DATA:");
    Serial.print("PulseMax = "); Serial.print(IR.protocol(12));                Serial.println("\tmks");
    Serial.print("PulseMin = "); Serial.print(IR.protocol(13));                Serial.println("\tmks");
    Serial.print("PauseMax = "); Serial.print(IR.protocol(14));                Serial.println("\tmks");
    Serial.print("PauseMin = "); Serial.print(IR.protocol(15));                Serial.println("\tmks");
                                                                               Serial.println("====================================");
                                                                               Serial.println("####################################");
  }
}
