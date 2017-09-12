
#include <Wire.h>
// библиотека для работы с модулями IMU
#include <TroykaIMU.h>
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, myMidi);

// создаём объект для работы с акселерометром
Accelerometer accel;

float oldR = 0;
float filtrR = 0;
bool negativeDelta = true;
unsigned long lastTime = 0;
unsigned long minTime = 0;
unsigned long lastClockTime = 0;
unsigned long timeClockPeriod = 0;
unsigned long lastAverageTime = 0;
unsigned long oldTimeClockPeriod = 0;

float oldDelta = 0;
//byte ppq = 24

void setup()
{
  // открываем последовательный порт
  Serial.begin(115200);
  // выводим сообщение о начале инициализации
  //  Serial.println("Accelerometer init...");
  // инициализация акселерометра
  accel.begin();
  // устанавливаем чувствительность акселерометра
  // 2g — по умолчанию, 4g, 8g
  accel.setRange(RANGE_2G);
  // выводим сообщение об удачной инициализации
  //  Serial.println("Initialization completed");
  myMidi.begin(4);
}

void loop()
{
  if (millis() - lastAverageTime >= 10) {
    lastAverageTime = millis();
    float x = accel.readAX();
    float y = accel.readAY();
    float z = accel.readAZ();
    float r = sqrt(x * x + y * y + z * z);

    filtrR = filtrR * 0.9 + r * 0.1;

    float delta = filtrR - oldR;

    oldDelta = 0.7 * oldDelta + delta * 0.3;

    if (delta < 0)
      negativeDelta = true;

    if (negativeDelta && (delta > 0.9))
    {
    // lastClockTime = millis();
      negativeDelta = false;
      unsigned long newTime = millis();
      timeClockPeriod = newTime - lastTime;
      lastTime = newTime;
      uint16_t bpm = 60000 / timeClockPeriod;
      if (bpm > 70 && bpm < 200)
        oldTimeClockPeriod = timeClockPeriod;
    }
    oldR = filtrR;
  }

  Serial.println(60000 / oldTimeClockPeriod);

  if ((millis() - lastClockTime) >= oldTimeClockPeriod / 24 ) {
    lastClockTime = millis();
    myMidi.sendRealTime(midi::MidiType::Clock);
  }
}
