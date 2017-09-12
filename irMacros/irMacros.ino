// Подключаем код для работы ИК-приёмника и передатчика
#include "IRremote_header.h"
// Подключаем код для работы с приложением remoteXY
#include "remoteXY_header.h"
// Подключаем код с логикой работы с энергонезависимой памятью
#include "eeprom_logic.h"
// Подключаем код с логикой работы с ИК-командами
#include "IRremote_logic.h"

// Переменная для хранения состояния "запись ИК-сигналов"
bool recordState = false;

// Переменная для хранения состояния кнопки записи
bool recButtonLastState = false;

// Переменная для хранения номера последеней нажатой кнопки на телефоне
byte buttonLast = 0;

void setup() {
  // Инициализируем remoteXY
  RemoteXY_Init ();
  // Запускаем работу ИК-приёмника (IRremote_header.h)
  irrecv.enableIRIn();
  // Если у нас есть записанные комбинации ИК-сигналов - достаём их (eeprom_logic.h)
  eepromDataInit();
}

void loop() {
  // Опрашиваем remoteXY. Не пришли ли новые команды по bluetooth?
  RemoteXY_Handler ();
  // Запоминаем состояние кнопки record на смартфоне.
  bool recButtonState = RemoteXY.record;
  // Если состояние кнопки изменилось...
  if (recButtonLastState != recButtonState) {
    // ... запомним новое состояние этой кнопки.
    recButtonLastState = recButtonState;
    // Если кнопка record нажата...
    if (recButtonState) {
      // меняем состояние "запись ИК-сигналов" на противоположное
      recordState = !recordState;
    }
    // Если мы вошли в состояние записи...
    if (recordState) {
      // ... зажгём красный "светодиод" на смартфоне.
      RemoteXY.recMode_r = 255;
    // Если мы вышли из состояния записи
    } else {
      // ... сохраним записанные данные ИК-пульта
      saveAll();
      // и потушим светодиод
      RemoteXY.recMode_r = 0;
    }
  }
// Если на смартфоне была нажата новая кнопка...
  if (RemoteXY.buttons != buttonLast) {
    // ... сохраним её значение.
    buttonLast = RemoteXY.buttons;
    // Еслим мы в состоянии записи — сохраним записанные данные ИК-пульта,
    // если нет — отправим ИК-сигналы, соответствующие этой кнопке (IRremote_logic.h)
    recordState ? saveAll() : sendButton(buttonLast);
  }
// Если мы в состоянии записи, и пришла новая команда с ИК-пульта...
  if ((recordState) && (irrecv.decode(&results))) {
    // сохраним команду с ИК-пульта (IRremote_logic.h) в последовательность команд текущей кнопки,
    // и увеличиваем счётчик принятых команд
    storeCode(&results, buttonLast, macroPos++);
    // перезапустим ИК-приёмник
    irrecv.resume();
  }
}
// Функция для сохраняем данных с ИК-пульта
void saveAll() {
  // Если была принята хотя бы одна команда с ИК-пульта
  if (macroPos > 0) {
    // записываем принятую последовательность команд (eeprom_logic.h)
    saveIRToEEPROM();
    // обнуляем счётчик команд
    macroPos = 0;
  }
}

