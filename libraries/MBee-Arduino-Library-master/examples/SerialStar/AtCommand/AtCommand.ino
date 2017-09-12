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
  * Скетч предназначен для демонстрации локального управления модулями MBee-868-x.0,
  * работающих под управлением программного обеспечения SerialStar.
  * Действия, производимые скетчем подробно описаны в комментариях к соответствующим строкам.
  * Модуль может быть соединен с платой Arduino c помощью XBee-shield или любого другого совмести-
  * мого устройств. Если доступного шилда нет, то возможно соединение Arduino и модуля с помощью
  * проводов. 
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
  * На модуле, после загрузки программного обеспечения SerialStar, должен быть произведен возврат 
  * к заводским настройкам одним из двух способов:
  * 1. Быстрое 4-х кратное нажатие "SYSTEM BUTTON" (замыкание вывода №36 модуля на землю).
  * 2. С помощью командного режима: +++, AT RE<CR>, AT CN<CR>.
  * 
  * После этого, на модеме необходимо включить пакетный режим с escape-символами AP=2. Режим аппаратного 
  * управления потоком (CTS/RTS) должен быть отключен.
  * Последовательность для настройки: +++, AT AP2<CR>, AT CN<CR>.
  * После окончания теста программа останавливается. Повтор осуществляется нажатием кнопки "RESET".
  * 
  * Диагностические сообщения и результаты выводятся на консоль с помощью SoftwareSerial.
  * Для проверки работоспособности, рекомендуется подключить в выводам ssRX и ssTX 
  * преобразователь USB-UART, например MB-USBridge, и воспользоваться любой удобной
  * терминальной программой.
  */

#define CMD_PARAMETER_LEN 2 //Длина поля параметра команды.   

//Назначаем выводы TX/RX для SoftSerial.
uint8_t ssRX = 8;
uint8_t ssTX = 9;

SoftwareSerial nss(ssRX, ssTX);
SerialStar mbee = SerialStar();

uint8_t myCmd[] = {'M','Y'}; //Команда запроса/установки собственного адреса (идентификатора) модема.
uint8_t txCmd[] = {'T','X'}; //Команда запроса/установки адреса для передачи "по умолчанию".
uint8_t frCmd[] = {'F','R'}; //Команда программного сброса модема.
uint8_t acCmd[] = {'A','C'}; //Команда применения и сохранения сделанных изменений.

uint8_t cmdParam[CMD_PARAMETER_LEN]; //Параметр команды.

AtCommandRequest atRequest = AtCommandRequest(); //Локальная команда модему. Команда "По умолчанию" устанавливается тип API-фрейма "apply and save" (AT_COMMAND_API_FRAME).
AtCommandResponse atResponse = AtCommandResponse(); //Ответ модема.
ModemStatusResponse modemResetStatus = ModemStatusResponse(); //Статус модема после рестарта.

void setup() 
{ 
  Serial.begin(9600);
  mbee.begin(Serial);
  nss.begin(9600);
  delay(500); //Задержка не обязательна и вставлена для удобства работы с терминальной программой.
}

void loop()
{
  nss.println("Get TX parameter from MBee."); //Посылаем команду запроса адреса на передачу.
  atRequest.setCommand(txCmd);  
  sendAtCommand();
  nss.println(" "); 
  
  nss.println("Get MY parameter from MBee."); //Посылаем команду запроса собственного адреса.
  atRequest.setCommand(myCmd); 
  sendAtCommand(); //Посылаем API-фрейм с командой.
  nss.println(" ");
  

  /**********************************************************************************************************/
  nss.println("Apply new MY = 0x1234.");
  atRequest.setApiId(AT_COMMAND_IMMEDIATE_APPLY_API_FRAME); //Устанавливаем тип фрейма, предназначенный для изменения и применения параметра без сохранения его в энергонезависимой памяти.
  atRequest.setCommand(myCmd);
  cmdParam[0] = 0x12; //Устанавливаем старший байт параметра MY;
  cmdParam[1] = 0x34; //Устанавливаем младший байт параметра MY;
  atRequest.setCommandValue(cmdParam); //Устанавливаем указатель на поле с новым значением параметром.
  atRequest.setCommandValueLength(sizeof(cmdParam)); //Устанавливаем длину поля параметра.
  sendAtCommand(); 
    
  nss.println("Get changed MY parameter."); //Проверяем установку нового значения параметра MY.
  atRequest.setCommandValueLength(0); //Если команда передается с длиной поля параметра, равной 0, то это означает, что команда запрашивает параметр.
  sendAtCommand(); 

  nss.println("Reset modem."); //Сбрасываем модем, чтобы продемонстрировать то, что параметр MY не был сохранен в энергонезависимой памяти, а только применен.
  atRequest.setCommand(frCmd);
  sendAtCommand();
   
  delay(500);
  nss.println("Check for return to the saved parameter MY."); //Проверяем отмену изменений.
  atRequest.setCommand(myCmd);
  sendAtCommand(); 
  nss.println(" ");
  
  
  /**********************************************************************************************************/
  nss.println("Let's apply and save new MY = 0x5678.");
  atRequest.setApiId(AT_COMMAND_API_FRAME); //Устанавливем тип фрейм для изменения, применения и сохранения параметра в энергонезависимой памяти.
  cmdParam[0] = 0x56; //Устанавливаем старший байт параметра MY;
  cmdParam[1] = 0x78; //Устанавливаем младший байт параметра MY;
  atRequest.setCommandValueLength(sizeof(cmdParam));
  sendAtCommand();

  nss.println("Get changed MY parameter."); //Проверяем установку нового значения параметра MY.
  atRequest.setCommandValueLength(0);
  sendAtCommand(); 

  nss.println("Reset modem."); //Сбрасываем модем, чтобы продемонстрировать то, что параметр MY был сохранен в энергонезависимой памяти после применения.
  atRequest.setCommand(frCmd);
  sendAtCommand(); 

  nss.println("Check for saving new parameter MY."); //Проверяем сохранение измененного параметра.
  atRequest.setCommand(myCmd);
  sendAtCommand();
  nss.println(" ");
  

  /**********************************************************************************************************/
  nss.println("Now just put new MY = 0x9ABC in queue. Neither apply nor save.");
  atRequest.setApiId(AT_COMMAND_QUEUE_PARAMETER_VALUE_API_FRAME); //Устанавливем тип фрейм для помещения параметра в очередь.
  cmdParam[0] = 0x9A; //Устанавливаем старший байт параметра MY;
  cmdParam[1] = 0xBC; //Устанавливаем младший байт параметра MY;
  atRequest.setCommandValueLength(sizeof(cmdParam));
  sendAtCommand(); 

  nss.println("Get unchanged MY parameter."); //Проверяем то, что действующий параметр MY не изменился а новое значение параметра поставлено в очередь.
  atRequest.setCommandValueLength(0);
  sendAtCommand(); 

  nss.println("Apply and save by AC command.");
  atRequest.setCommand(acCmd);
  sendAtCommand();

  nss.println("Reset modem."); //Сбрасываем модем, чтобы продемонстрировать то, что параметр MY был сохранен в энергонезависимой памяти.
  atRequest.setCommand(frCmd);
  sendAtCommand(); 

  nss.println("Check for saving new parameter MY."); //Проверяем сохранение измененного параметра.
  atRequest.setCommand(myCmd);
  sendAtCommand();
  nss.println(" ");

  nss.println("Test is finished. Press \"RESET BUTTON\" to repeat.");
  while(1); //Останавливаем скетч и ждем нажатия кнопки "RESET".
}

void sendAtCommand() 
{
  mbee.send(atRequest); //Отправляем команду на модуль.
  if(mbee.readPacket(500)) //Ждем ответа 0,5 секунды.
  {
    //Пакет получен. Проверяем, является ли принятый пакет ответом на AT-команду.
    if((mbee.getResponse().getApiId() == AT_COMMAND_RESPONSE_IMMEDIATE_APPLY_API_FRAME) || (mbee.getResponse().getApiId() == AT_COMMAND_RESPONSE_API_FRAME) || (mbee.getResponse().getApiId() == AT_COMMAND_RESPONSE_QUEUE_PARAMETER_VALUE_API_FRAME))
    {
      mbee.getResponse().getAtCommandResponse(atResponse);
      if(atResponse.isOk()) 
      {
        nss.print("Command [");
        nss.print((char)atResponse.getCommand()[0]);
        nss.print((char)atResponse.getCommand()[1]);
        nss.println("] was successful!");
        if(atResponse.getValueLength() > 0) 
        {
          nss.print("Command parameter length is ");
          nss.print(atResponse.getValueLength(), DEC);
          nss.println(" byte.");
          nss.print("Read parameter: ");
          for(int i = 0; i < atResponse.getValueLength(); i++) 
          {
            nss.print(atResponse.getValue()[i], HEX);
            nss.print(" ");
          }
          nss.println("");
        }
      } 
      else 
      {
        nss.print("Command return error code: ");
        nss.println(atResponse.getStatus(), HEX);
      }
    }
    else
    { 
      mbee.getResponse().getModemStatusResponse(modemResetStatus);
      if((mbee.getResponse().getApiId() == MODEM_STATUS_API_FRAME)  && (modemResetStatus.getStatus() == 0x01))
      {
        nss.println("Got modem reset status OK.");
      }
      else
      {
        nss.print("Expected AT response but got: 0x");
        nss.print(mbee.getResponse().getApiId(), HEX);
        nss.println(".");
      }
    }   
  }
  else 
  {
    if(mbee.getResponse().isError()) //Ответ на AT-команду не получен.
    {
      nss.print("Error reading packet.  Error code: ");  
      nss.print(mbee.getResponse().getErrorCode());
      nss.println(".");
    } 
    else 
    {
      nss.println("No response from local modem.");  
    }
  }
}

