#define RECIEVE_PIN  Z1 // датчик освещённости принимает данные
#define SEND_BIT_INTERVAL 50 // длительность передачи одного бита
// отличить фоновый свет от данных поможет порог (англ. treshold)
int treshold;

void setup() {
  Serial.begin(9600);
  while (!Serial) // пока Serial Monitor не открыт, настроим порог
    treshold = 0.8 * analogRead(RECIEVE_PIN);
}

void loop() {
  // падение напряжения датчика ниже порога означает, что светодиод
  // передатчика горит. Это старт передачи нового символа
  if (analogRead(RECIEVE_PIN) < treshold)
    // примем (англ. recieve) символ и передадим его в компьютер
    Serial.print(recieve());
}

char recieve()
{
  // выдержим паузу, чтобы оказаться посередине передачи первого бита 
  delay(SEND_BIT_INTERVAL * 1.5);
  // префикс «0b» означает, что целое число за ним записано в двоичном
  // коде. 
  char message = 0b00000000;
  // символ состосит из 8 бит. Для каждого бита сивола...
  for (int i = 0; i < 8; ++i) {
    // ... определим: если светодиод передатчика горит
    if (analogRead(RECIEVE_PIN) < treshold)
      // устанавливаем текущий бит (англ. set bit) в единицу...
      bitSet(message, i);
    // ... и ждём следующий бит
    delay(SEND_BIT_INTERVAL);
  }
  return message; // символ получен. Возвращаем его значение
}

