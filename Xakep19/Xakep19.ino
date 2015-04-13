#include <QuadDisplay.h>
#define DISPLAY_PIN 7
#define BUTTON_COUNT 4

int buttons[] = {A1, Z1, Y1, X1};

void setup() {
}

void loop() {
  // префикс «0b» означает, что целое число за ним записано в
  // в двоичном коде. Хватит одного байта (англ. byte, 8 бит)
  // для хранения комбинации состояний 8 кнопок. Мы используем
  // всего четыре кнопки. Их состояние будем хранить в младших
  // 4-х битах, их ещё называют «младшей тетрадой».
  byte binary = 0b00000000;
  for (int i = 0; i < BUTTON_COUNT; ++i) {
    boolean isUp = digitalRead(buttons[i]);
    // если текущая кнопка нажата
    if (!isUp)
      // устновим бит (англ. set bit), соответствующий этой
      // кнопке.
      bitSet(binary, i);
  }  
  // выведем получившееся число на дисплей
  displayInt(DISPLAY_PIN, binary);
  delay(100);
}
