#define POMODORO_TIME 25

#define LED_R 10
#define LED_Y 3
#define LED_G 9
#define LED_B 4
#define BUZZER 2
#define BUTTON 8

#define LED_COUNT 4

int leds[] = {LED_R,
              LED_G,
              LED_Y,
              LED_B
             };

unsigned long pomodoroTimeInMillis;
unsigned long ledSwitchTime;
unsigned long lastTime = 0;
bool pomodoroState = false;

void setup() {
  Serial.begin(9600);
  pomodoroTimeInMillis = POMODORO_TIME * 60000;
  ledSwitchTime = pomodoroTimeInMillis / LED_COUNT;

  for (int i = 0; i < LED_COUNT; ++i) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {

  if (checkClick()) {
    lastTime = millis();
    pomodoroState = !pomodoroState;

    for (int i = 0; i < LED_COUNT; ++i) {
      digitalWrite(leds[i], LOW);
    }
  }

  if (pomodoroState) {
    unsigned long elapsedTime = millis() - lastTime;
    if (elapsedTime < pomodoroTimeInMillis) {
      unsigned long currentLed = elapsedTime / ledSwitchTime;
      digitalWrite(leds[currentLed], HIGH);
    } else {
      finish();
    }
  }
}

// Функция для проверки клика кнопки
bool checkClick()
{
  // Переменная для хранения предыдущего состояния кнопки
  static bool buttonWasUp = true;
  bool result = false;

  // Читаем текущее состояние кнопки
  bool buttonIsUp = digitalRead(BUTTON);

  // Если кнопка была не нажата, а теперь нажата
  // значит был клик
  if (buttonWasUp && !buttonIsUp) {
    delay(100);
    result = true;
  }
  buttonWasUp = buttonIsUp;
  return result;
}

void finish() {
  bool toggle = true;
  for (int i = 0; i < 10; i++) {
    tone(BUZZER, 500 + toggle * 500);
    for (int i = 0; i < LED_COUNT; ++i) {
      digitalWrite(leds[i], toggle);
      toggle = !toggle;
    }
    toggle = !toggle;
    delay(100);
  }
  noTone(BUZZER);
  pomodoroState = false;
}

