/**
  * Этот файл является частью библиотеки MBee-Arduino.
  * 
  * MBee-Arduino является бесплатным программным обеспечением. Вы можете 
  * распространять и/или модифицировать его без ограничений так, как это
  * предусмотрено GNU General Public License, начиная с версии 3.
  * 
  * Надеемся, что программные продукты, созданные с помощью данной библиотеки 
  * будут полезными, однако никакие гарантии, явные или подразумеваемые не
  * предоставляются.
  * \author </i> von Boduen. Special thanx to Andrew Rapp.
  */
      
#include <MBee.h>
#include <SoftwareSerial.h>

/**
  * Скетч предназначен для демонстрации приема и обработки пакета с данными о состоянии 
  * цифровых и аналоговых датчиков на удаленном модеме. Принимающий и передающий модули MBee-868-x.0 
  * работают под управлением ПО Serial Star.
  * Действия, производимые скетчем подробно описаны в комментариях к соответствующим строкам.
  * Потребуются 2 модуля MBee-868-x.0. Первый модуль соедининяется с платой Arduino c помощью 
  * XBee-shield или любого другого совместимого устройств. Если доступного шилда нет, то возможно 
  * соединение Arduino и модуля с помощью проводов.
  * ВНИМАНИЕ!!! Модуль MBee-868-x.0 имеет номинальное значение напряжения питания 3,3В. Если Ваша
  * плата Arduino имеет выходы с логическими уровнями 5В, то необходимо предусмотреть делитель 
  * напряжения между выходом TX Arduino и входом RX модуля (вывод №3 для всех моделей). К выводу TX
  * Arduino подключается резистор 2К, с которым соединен резистор 1К, второй вывод последнего
  * сажается на землю. Точка соединения резисторов соединяется с выводом №3 модуля. 
  * Вывод №2 модуля (TX), подключается ко входу RX Arduino через последовательный резистор 1К.
  * При использовании для питания модуля собственного источника 3,3В Arduino, необходимо помнить о том,
  * что модули могут потреблять в режиме передачи токи до 200 мА. Поэтому необходимо уточнять 
  * нагрузочные характеристики применяемой Вами платы Arduino. При коротких эфирных пакетах для 
  * компенсации недостаточного выходного тока источника 3,3В можно применить конденсаторы с емкостью
  * не менее 2200 мкФ, устанавливаемые параллельно питанию модуля.
  * Модуль должен быть предварительно настроен для работы в пакетном режиме с escape-
  * символами AP=2. Режим аппаратного управления потоком (CTS/RTS) должен быть отключен.
  * Последовательность для настройки: 
  * 1. +++
  * 2. AT AP2<CR>
  * 3. AT CN<CR>
  * 
  * Второй модуль должен быть установлен на плату MB-Tag, или любую другую, обеспечивающую 
  * питание 3,3В. На модуле необходимо включить режим периодического сна с помощью следующей  
  * последовательности команд:
  * 1. +++
  * 2. AT SM4<CR>
  * 3. AT CN<CR>
  * Все прочие настройки можно оставить "по умолчанию".
  * 
  * Диагностические сообщения и результаты выводятся на консоль с помощью SoftwareSerial.
  * Для проверки работоспособности, рекомендуется подключить в выводам ssRX и ssTX 
  * преобразователь USB-UART, например MB-USBridge, и воспользоваться любой удобной
  * терминальной программой.
  */

//Назначаем выводы TX/RX для SoftSerial.
uint8_t ssRX = 8;
uint8_t ssTX = 9;

SoftwareSerial nss(ssRX, ssTX);
SerialStar mbee = SerialStar();

RxIoSampleResponse ioSample = RxIoSampleResponse();

#define REQUESTED_PIN 34 //Номер вывода, используемый в примере запроса наличия в принятом пакете данных о его текущем состонии.

void setup() 
{ 
  Serial.begin(9600);
  mbee.begin(Serial);
  nss.begin(9600);
  delay(500); //Задержка не обязательна и вставлена для удобства работы с терминальной программой.
}

void loop() 
{
  mbee.readPacket(); //Проверяем, если в буфере пакет.
  if(mbee.getResponse().isAvailable()) 
  {
    if(mbee.getResponse().getApiId() == IO_DATA_SAMPLE_API_FRAME) //Является ли принятый пакет, пакетом с данными о состоянии датчиков удаленного модема?
    {
      /**********************************************************************************************************/
      mbee.getResponse().getRxIoSampleResponse(ioSample); //Получаем пакет с данными.
      nss.println("");
      nss.print("Received packet with I/O samples from remote modem with address: ");
      nss.println(ioSample.getRemoteAddress(), HEX); //Печатаем адрес отправителя.
      nss.println("");

      /**********************************************************************************************************/
      nss.print("Temperature of the module is "); //Выводим температуру удаленного модема по показаниям встроенного датчика.
      if(ioSample.getTemperature() < 128) //Переводим число из дополнительного кода в прямой.
        nss.print(ioSample.getTemperature(), DEC);
      else
      {
        nss.print("-"); //Температура отрицательная.
        nss.print(256 - ioSample.getTemperature(), DEC);
      }
      nss.println("C.");
      
      /**********************************************************************************************************/
      nss.print("Supply voltage is "); //Печатаем измеренное напряжение источника питания удаленого модема.
      nss.print(float(ioSample.getVbatt()) / 51);
      nss.println("V.");
      
      /**********************************************************************************************************/
      if(ioSample.getSampleSize()) //Содержится ли в принятом пакете информация о состоянии линий ввода/вывода удаленного модема?
      {
        nss.print("There are(is) ");
        nss.print(ioSample.getSampleSize(), DEC);
        nss.println(" I/0 sample(s) in the packet.");
      }
      
      /**********************************************************************************************************/
      if(ioSample.isAvailable(REQUESTED_PIN)) //Пример запроса о наличии в принятом пакете данных о конкретном выводе модуля.
      {
        nss.print("Pin #");
        nss.print(REQUESTED_PIN);
        nss.println(" is sampled in the received packet.");
      }
      else
      {
        nss.print("Pin #");
        nss.print(REQUESTED_PIN);
        nss.println(" is NOT sampled in the received packet.");
      }
        
      /**********************************************************************************************************/  
      for(uint8_t i = 1; i <= ioSample.getSampleSize(); i++)// Выводим данные о текущем состоянии каждой линии ввода/вывода содержащейся а пакете.
      {
        nss.print("Pin #"); 
        nss.print(ioSample.getPin(i), DEC);
        nss.print(" is "); //Выводим идентификатор режима, на работу в котором настроена данная линия ввода/вывода.
        if((ioSample.getMode(i) & 0x7F) == IO_ADC) //Информация о текущем состоянии цифрового входа/выхода передается в старшем бите кода режима. Поэтому сбрасываем его, чтобы получить код режима.
        {
          nss.print("analog input. Voltage: ");
          nss.print(float(ioSample.getAnalog(i))* 2.5 /4096);
          nss.println("V.");
        }
        else if((ioSample.getMode(i) & 0x7F) == IO_DIGITAL_INPUT) 
        {
          nss.print("digital input. State: ");
          printDigitalPinState(ioSample.getDigital(i)); //Пример получения информации о текущем состоянии цифровой линии с помощью специальной функции.
         }
        else if((ioSample.getMode(i) & 0x7F) == IO_DIGITAL_OUTPUT_LO) 
        {
          nss.print("digital output with default LOW state. State: ");
          printDigitalPinState(ioSample.getMode(i) & 0x80); //Пример получения информации о текущем состоянии цифровой линии с помощью старшего бита кода режима. Такой способ работает намного быстрее, чем вызов функции getDigital().
        }
        else if((ioSample.getMode(i) & 0x7F) == IO_DIGITAL_OUTPUT_HI)
        {
          nss.print("digital output with default HIGH state. State: ");
          printDigitalPinState(ioSample.getMode(i) & 0x80);
        }
        
        else if((ioSample.getMode(i) & 0x7F) == IO_COUNTER_INPUT1)
        {
          nss.print("count input 1 with pullup. Pulse count: ");
          nss.print(ioSample.getCounter(i));
          nss.println(".");
        }
        else if((ioSample.getMode(i) & 0x7F) == IO_COUNTER_INPUT2)
        {
          nss.print("count input 2. Pulse count: ");
          nss.print(ioSample.getCounter(i));
          nss.println(".");
        }
        else if((ioSample.getMode(i) & 0x7F) == IO_WAKEUP_INPUT_FALLING_EDGE)
        {
          nss.print("wakeup(alarm) input with HIGH to LOW active front. State: ");
          printDigitalPinState(ioSample.getMode(i) & 0x80);
        }
        else if((ioSample.getMode(i) & 0x7F) == IO_WAKEUP_INPUT_RISING_EDGE)
        {
          nss.print("wakeup(alarm) input with LOW to HIGH active front. State: ");
          printDigitalPinState(ioSample.getMode(i) & 0x80);
        }
      }
    }
    else 
    {
      nss.print("Received packet is not I/O sample packet and has API ID = ");
      nss.print(mbee.getResponse().getApiId(), HEX);
    }    
  } 
  else if(mbee.getResponse().isError()) 
  {
    nss.print("Error reading packet.  Error code: ");  
    nss.println(mbee.getResponse().getErrorCode());
  }
  ioSample.reset(); //Инициализируем буфер для приема следующего пакета.
}

void printDigitalPinState(uint8_t state)
{
  if(state)
    nss.println("HIGH.");
  else
    nss.println("LOW.");
}
        
