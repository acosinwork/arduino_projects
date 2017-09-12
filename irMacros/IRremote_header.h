// Библиотека для работы с ИК-приёмником и передатчиком
#include <IRremote.h>

// Имя пина, которому подключен приёмник
#define RECV_PIN 3
// Имя пина, к которому подключен передатчик
#define SEND_PIN 9

// Максимальная длина последовательности комманд ИК-пульта
#define IR_BUTTONS_COUNT 15
// Максимальное количество таких последовательностей
// Должно быть равно количеству кнопок в remoteXY
#define MACROS_COUNT 5

// Создаём объект для ИК-приёмника
IRrecv irrecv(RECV_PIN);

// Создаём объект для ИК-передатчика
IRsend irsend;

// Создаём объект для декодированных команд с ИК-пульта
decode_results results;

// Структура для хранения команд с ИК-пульта
struct irData {
  // Тип принятой команды: UNKNOWN, NEC, SONY, RC5, ...
  decode_type_t          decode_type = UNKNOWN;
  // Адрес устройства. Используется в пультах Panasonic и Sharp
  unsigned int           address = 0;
  // Значение декодированной ИК-команды
  unsigned long          value = 0;
  // Длина команды в битах
  int                    bits = 0;
};

// Массив для хранения последовательностей ИК-команд
irData irSygnals[MACROS_COUNT][IR_BUTTONS_COUNT];
// Счётчик принятых команд в текущей последовательности
byte macroPos = 0;

