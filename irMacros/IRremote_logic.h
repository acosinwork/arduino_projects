// Функция для отправки последовательности команд
// принимает на вход номер последовательности
void sendButton(byte button)
{
  // Переменная для хранения бита переключения. Нужна в протоколах RC5, RC6
  int toggle = 0;

  // Перебираем в цикле всю последовательность команд
  for (int i = 0; i < IR_BUTTONS_COUNT; ++i) {

    // Сохраняем текущую команду во вспомогательные переменные
    decode_type_t decode_type = irSygnals[button][i].decode_type;
    unsigned int address = irSygnals[button][i].address;
    unsigned long codeValue = irSygnals[button][i].value;
    int codeLen = irSygnals[button][i].bits;

    // Если протокол команды известен
    if (decode_type != UNKNOWN) {

      // Если протокол команды RC5 или RC6
      if (decode_type == RC5 || decode_type == RC6) {
        // Инвертируем бит переключения,
        toggle ^= 1;
        // обнулим бит переключения в самом значении команды,
        codeValue = codeValue & ~(1 << (codeLen - 1));
        // и запишем в это место наш бит переключения... Вот такой уж протокол:)
        codeValue = codeValue | (toggle << (codeLen - 1));
        // Если протокол RC5,
        if (decode_type == RC5) {
          // отправим ИК-команду по протоколу RC5
          irsend.sendRC5(codeValue, codeLen);
          // Если протокол RC6,
        } else {
          // отправим ИК-команду по протоколу RC6
          irsend.sendRC6(codeValue, codeLen);
        }
      }
      // В зависимости от протокола команды, используем соответствующие
      // этому протоколу функции из библиотеки IRremote
      switch (decode_type)
      {
        case NEC:
          irsend.sendNEC(codeValue, codeLen);
          break;
        case SONY:
          irsend.sendSony(codeValue, codeLen);
          break;
        case PANASONIC:
          irsend.sendPanasonic(address, codeValue);
          break;
        case JVC:
          irsend.sendJVC(codeValue, codeLen, false);
          break;
        case SAMSUNG:
          irsend.sendSAMSUNG(codeValue, codeLen);
          break;
        case WHYNTER:
          irsend.sendWhynter(codeValue, codeLen);
          break;
        case LG:
          irsend.sendLG(codeValue, codeLen);
          break;
        case DISH:
          irsend.sendDISH(codeValue, codeLen);
          break;
        case SHARP:
          irsend.sendSharp(address, codeValue);
      }
      // Подождём выполнения команды бытовыми приборами
      delay(1000);
      // если команда неизвестна — выходим из функции.
    } else return;
  }
}

// Функция для сохранения принятой ИК-команды
// принимает на вход декодированную ИК-команду, номер текущей последовательности (т.е. номер нажатой кнопки)
// и позицию команды в текущей последовательности
void storeCode(decode_results *results, int buttonNumber, int pos)
{
  // Если позиция команды меньше, чем максимальное количество команд в последовательности
  if (pos < IR_BUTTONS_COUNT) {
    // Если протокол команды известен, и это не команда повтора команды
    if ((results->decode_type != UNKNOWN) && (results->value != REPEAT)) {
      // Сохраняем команду в массив хранения последовательностей команд
      irSygnals[buttonNumber][pos].decode_type = results->decode_type;
      irSygnals[buttonNumber][pos].address = results->address;
      irSygnals[buttonNumber][pos].value = results->value;
      irSygnals[buttonNumber][pos].bits = results->bits;
    }
  }
  // Пометим следующую команду как неизвестную, чтобы
  // остановится на этом месте в функции отправки последовательностей команд
  if (pos < IR_BUTTONS_COUNT - 1)
    irSygnals[buttonNumber][pos + 1].decode_type = UNKNOWN;
}

