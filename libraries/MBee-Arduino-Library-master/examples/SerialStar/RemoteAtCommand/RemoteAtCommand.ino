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
  * Скетч предназначен для демонстрации удаленного управления управления модулями MBee-868-x.0,
  * работающих под управлением программного обеспечения SerialStar.
  * Действия, производимые скетчем подробно описаны в комментариях к соответствующим строкам.
  * Потребуются 2 модуля MBee-868-x.0. Первый модуль соедининяется с платой Arduino c помощью 
  * XBee-shield или любого другого совместимого устройства. Если доступного шилда нет, то возможно 
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
  * На обоих модулях, после загрузки программного обеспечения SerialStar, должен быть произведен возврат 
  * к заводским настройкам одним из двух способов:
  * 1. Быстрое 4-х кратное нажатие "SYSTEM BUTTON" (замыкание вывода №36 модуля на землю).
  * 2. С помощью командного режима: +++, AT RE<CR>, AT CN<CR>.
  * 
  * Первый модуль должен быть предварительно настроен для работы в пакетном режиме с escape-
  * символами AP=2. Режим аппаратного управления потоком (CTS/RTS) должен быть отключен.
  * Последовательность для настройки: +++, AT AP2<CR>, AT CN<CR>.
  * 
  * Второй модуль установливается на плату MB-Tag, или любую другую, обеспечивающую его 
  * питанием 3,3В. В этом случае к выводу №9 должен быть подключен через последовательный резистор 1К 
  * контрольный светодиод. Никакие дополнительные предварительные настройки второго модуля не требуются.
  * 
  * Диагностические сообщения и результаты выводятся на консоль с помощью SoftwareSerial.
  * Для проверки работоспособности, рекомендуется подключить в выводам ssRX и ssTX 
  * преобразователь USB-UART, например MB-USBridge, и воспользоваться любой удобной
  * терминальной программой.
  */

#define CMD_PARAMETER_LEN 4 //Макисимальная длина поля параметра команд, которые будут использоваться в этом скетче.  

//Назначаем выводы TX/RX для SoftSerial.
uint8_t ssRX = 8;
uint8_t ssTX = 9;

SoftwareSerial nss(ssRX, ssTX);
SerialStar mbee = SerialStar();

uint8_t l5Cmd[] = {'L','5'}; //Команда запроса/установки режима линии L5 (вывод модуля №9).
uint8_t smCmd[] = {'S','M'}; //Команда запроса/установки режима сна.
uint8_t spCmd[] = {'S','P'}; //Команда запроса/установки интервала сна.

uint8_t cmdParam[CMD_PARAMETER_LEN]; //Параметр команды.

uint16_t remoteAddress = 0x0001; //Адрес модема, которому будут передаваться команды.

RemoteAtCommandRequest remoteAtRequest = RemoteAtCommandRequest(); //Команда удаленному модему. Для экономии памяти будем использовать один и тот же объект для отправки всех команд.
TxStatusResponse txStatus = TxStatusResponse(); //Локальный ответ со статусом команды.
RemoteAtCommandResponse remoteAtResponse = RemoteAtCommandResponse(); //Ответ удаленного модема со статусом команды.

void setup() 
{
  Serial.begin(9600);
  mbee.begin(Serial);
  nss.begin(9600);
  delay(500); //Задержка не обязательна и вставлена для удобства работы с терминальной программой.
}

void loop() 
{
  nss.println("Sending L5=4 command to the remote modem with queue option."); //Посылаем команду установки линии L5 в режим цифрового выхода с низким уровнем "по умолчанию".
  remoteAtRequest.setRemoteAddress(remoteAddress); //Адрес удаленного модема.
  remoteAtRequest.setApplyChanges(false); //Измененный параметр не будет применен и сохранен сразу после выполнения команды, а помещен в очередь.
  remoteAtRequest.setCommand(l5Cmd); //Устанавливаем указатель на константу с символами кода команды. 
  cmdParam[0] = 4; //Устанавливаем значения параметра команды.
  remoteAtRequest.setCommandValue(&cmdParam[0]); //Устанавливаем указатель на переменную с параметром команды.
  remoteAtRequest.setCommandValueLength(1); //Длина поля параметра команды 1 байт.
  sendRemoteAtCommand();

  /**********************************************************************************************************/
  nss.println("Sending SM=4 command with the same options."); //Посылаем команду установки режима периодического сна.
  remoteAtRequest.setCommand(smCmd); //Меняем указатель на константу с символами кода новой команды. 
  cmdParam[0] = 0x04; //Изменяем значение параметра команды.
  sendRemoteAtCommand();

  /**********************************************************************************************************/
  nss.println("Sending SP=300 command with set, apply and save options."); //Посылаем команду установки интервала сна, равным 3 секундам. Все ранее изменненые параметры сохраняем и применяем.
  remoteAtRequest.setApplyChanges(true);
  remoteAtRequest.setCommand(spCmd); //Изменяем указатель на константу с символами кода команды. 
  cmdParam[0] = 0x00; //Изменяем значение параметра команды.
  cmdParam[1] = 0x00;
  cmdParam[2] = 0x01; //Число 300 (0x12C). Интервал сна задается в десятках миллисекунд.
  cmdParam[3] = 0x2C;
  remoteAtRequest.setCommandValueLength(4); //Корректируем длину поля параметра команды.
  sendRemoteAtCommand();
  
  /**********************************************************************************************************/
  nss.println(" "); 
  nss.print("Now modem with address 0x");
  nss.print(remoteAddress >> 8, HEX); 
  nss.print(remoteAddress & 0xFF, HEX); 
  nss.println(" is sleeping device with waking interval 3 s."); 
  nss.println(" "); 

  /**********************************************************************************************************/
  nss.println("Sending SP request command from remote mode."); //Посылаем команду запроса установки интервала сна, с учетом того, что модем уже работает в спящем режиме.
  remoteAtRequest.setSleepingDevice(true); //Пакет будет буферизирован, а не сразу отправлен в эфир.
  remoteAtRequest.setCommandValueLength(0); //Если длина параметра равна 0, то это значит, что команда запрашивает параметр, а не устанавливает его.
  sendRemoteAtCommand();

  delay(3100); //Пауза за время которой спящий модем должен получить предыдущую команду и ответить на нее.
  getRemoteResponse(1); //В этом вызове должны получить пакет с состоянием датчиков на удаленном модеме. Отправка этого пакета производится им автоматически в режиме периодического сна. Буферизированная команда отправляется спящему модему только в ответ на принятый от него пакет.
  if(mbee.getResponse().getApiId() == IO_DATA_SAMPLE_API_FRAME)
  {
    nss.println("Packet with sensor data received from remote modem.");
    getRemoteResponse(1); //Вызываем функцию во второй раз, чтобы получить ответ на команду, переданную из буфера.
  }

  /**********************************************************************************************************/
  nss.println(" "); 
  nss.print("Let's flashing LED on L5 now.");

  /**********************************************************************************************************/
  nss.println("Sending L5=5 command with apply only option."); //Посылаем команду установить высокий уровень на линии L5. При этом сохранения изменений не делаем, чтобы не уменьшать ресурс флеш памяти.
  remoteAtRequest.setAcknowledge(false); //Для повышения пропускной способности можно отключить подтверждение получения команды от удаленным модемом.
  remoteAtRequest.setSaveChanges(false); //Изменения будут сразу применены без сохранения.
  remoteAtRequest.setSleepingDevice(true); //Команда предназначается спящему модему.
  remoteAtRequest.setCommand(l5Cmd); //Изменяем  указатель на константу с символами кода команды. 
  cmdParam[0] = 5; //Команда установки на L5 высокого уровня.
  remoteAtRequest.setCommandValueLength(1); //Корректируем длину поля параметра команды.
  sendRemoteAtCommand();
  delay(3100); //Пауза за время которой спящий модем должен получить предыдущую команду.
  mbee.readPacket(); //Вызов данной функции необходим, чтобы очистить приемный буфер UART от пакета с данными от датчиков удаленного модема.

  /**********************************************************************************************************/
  nss.println("Send L5 OFF command.");
  cmdParam[0] = 4; //Переводим L5 в низкий уровень.
  sendRemoteAtCommand();
  delay(3100);
  mbee.readPacket();
  
  /**********************************************************************************************************/
  nss.println("Send L5 ON command.");
  cmdParam[0] = 5;
  sendRemoteAtCommand();
  delay(3100);
  mbee.readPacket();
  
    /**********************************************************************************************************/
  nss.println("Send L5 OFF command.");
  cmdParam[0] = 4;
  sendRemoteAtCommand();
  delay(3100);
  mbee.readPacket();
    
  remoteAtRequest.setDefault(); //Приводим все поля команды для передачи к значениям "по умолчанию" для демонстрации наличия этой функции. Таким образом упрощается повоторное использование объекта для отправки последующих команд.
  nss.println("Test is finished. Press \"RESET BUTTON\" to repeat. Do not forget return default parameters on the remote modem! Quickly press \"SYSTEM BUTTON\" more than 4 times on it.");
  while(1); //Останавливаем скетч и ждем нажатия кнопки "RESET".
}

void sendRemoteAtCommand() 
{
  mbee.send(remoteAtRequest); //Отправляем команду на удаленный модуль.
  if(remoteAtRequest.getFrameId())
    getLocalResponse(50);
  if((remoteAtRequest.getFrameId() == 0) || (txStatus.isSuccess() && remoteAtRequest.getSleepingDevice() == false)) //Ждем ответного пакета от удаленного модема только если локальный ответ выключен с помощью frameId = 0, команда отправлена(буферизирована) и не предназначается спящему модему. 
    getRemoteResponse(100);
}

void getLocalResponse(uint16_t timeout)
{
  if(mbee.readPacket(timeout)) //Ждем локального ответа со статусом команды.
  {
    if(mbee.getResponse().getApiId() == TRANSMIT_STATUS_API_FRAME)//Проверяем, является ли принятый пакет локальным ответом на AT-команду удаленному модему. 
    {
      mbee.getResponse().getTxStatusResponse(txStatus);
      if(txStatus.isSuccess()) 
      {
        nss.print("Command [");
        nss.print((char)remoteAtRequest.getCommand()[0]);
        nss.print((char)remoteAtRequest.getCommand()[1]);
        if(remoteAtRequest.getSleepingDevice())
          nss.println("] was successfully buffered!");
        else
          nss.println("] was successfully transmitted!");
      }
      else if(txStatus.getStatus() == ERROR_COMMAND_STATUS)
        nss.println("Error!Unsufficient memory.");
      else if(txStatus.getStatus() == INVALID_CODE_COMMAND_STATUS)
        nss.println("Error!Invalid command code.");
      else if(txStatus.getStatus() == INVALID_PARAMETER_COMMAND_STATUS)
        nss.println("Error!Invalid parameter value.");
      else if(txStatus.getStatus() == TX_FAILURE_COMMAND_STATUS)
        nss.println("Error!TX failure due to CCA.");          
    }
  }
}

void getRemoteResponse(uint16_t timeout)
{
  if(mbee.readPacket(timeout)) //Ждем ответа от удаленного модуля со статусом команды.
  {
    if(mbee.getResponse().getApiId() == REMOTE_AT_COMMAND_RESPONSE_API_FRAME)  //Проверяем, не является ли полученный фрейм ответом на команду удаленным модемом.
    {
      mbee.getResponse().getRemoteAtCommandResponse(remoteAtResponse);
      if(remoteAtResponse.isOk()) 
      {
        nss.print("Command [");
        nss.print((char)remoteAtResponse.getCommand()[0]);
        nss.print((char)remoteAtResponse.getCommand()[1]);
        nss.println("] was successfully received by remote modem.");
        if(remoteAtResponse.getValueLength() > 0) 
        {
          nss.print("Command parameter length is ");
          nss.print(remoteAtResponse.getValueLength(), DEC);
          nss.println(" byte.");
          nss.print("Read from remote modem: ");
          for(int i = 0; i < remoteAtResponse.getValueLength(); i++) 
          {
            nss.print(remoteAtResponse.getValue()[i], HEX);
            nss.print(" ");
          }
          nss.println("");
        }
      }
      else 
      {
        nss.print("Command returned error code: ");
        nss.println(remoteAtResponse.getStatus(), HEX);
      }
    } 
    else 
    {
      nss.print("Expected remote modem response but got: 0x");
      nss.print(mbee.getResponse().getApiId(), HEX);
      nss.println(".");
    }    
  }
  else
  {
    if(mbee.getResponse().isError()) 
    {
      nss.print("Error reading packet.  Error code: ");  
      nss.print(mbee.getResponse().getErrorCode());
      nss.println(".");
    }
    else 
      nss.println("No response from remote modem.");  
  }
}

