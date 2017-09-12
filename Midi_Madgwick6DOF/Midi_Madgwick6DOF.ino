#include <MIDI.h>
// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с модулями IMU
#include <TroykaIMU.h>

// множитель фильтра
#define BETA 0.22f

// создаём объект для фильтра Madgwick
Madgwick filter;

// создаём объект для работы с акселерометром
Accelerometer accel;
// создаём объект для работы с гироскопом
Gyroscope gyro;

// переменные для данных с гироскопов, акселерометров
float gx, gy, gz, ax, ay, az;

// получаемые углы ориентации
float yaw, pitch, roll;

// переменная для хранения частоты выборок фильтра
float fps = 100;

unsigned long midiSendMillis = 0;

byte sendedNote = 0;

byte channel = 2;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, myMidi);

void setup()
{
  // открываем последовательный порт
  Serial.begin(115200);
  //  Serial.println("Begin init...");
  // инициализация акселерометра
  accel.begin();
  // инициализация гироскопа
  gyro.begin();
  // выводим сообщение об удачной инициализации
  //  Serial.println("Initialization completed");

  myMidi.begin(channel);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);

}

void loop()
{
  // запоминаем текущее время
  unsigned long startMillis = millis();

  // считываем данные с акселерометра в единицах G
  accel.readGXYZ(&ax, &ay, &az);
  // считываем данные с акселерометра в радианах в секунду
  gyro.readRadPerSecXYZ(&gx, &gy, &gz);
  // устанавливаем коэффициенты фильтра
  filter.setKoeff(fps, BETA);
  // обновляем входные данные в фильтр
  filter.update(gx, gy, gz, ax, ay, az);

  // получение углов yaw, pitch и roll из фильтра
  yaw =  filter.getYawDeg();
  pitch = filter.getPitchDeg();
  roll = filter.getRollDeg();
  
  // выводим полученные углы в serial-порт
  Serial.print("yaw: ");
  Serial.print(map(yaw, -180, 180, 0, 127));
  Serial.print("\t\t");
  Serial.print("pitch: ");
  Serial.print(map(pitch, -180, 180, 0, 127));
  Serial.print("\t\t");
  Serial.print("roll: ");
  Serial.println(map(roll, -180, 180, 0, 127));
  
  // вычисляем затраченное время на обработку данных
  unsigned long deltaMillis = millis() - startMillis;
  // вычисляем частоту обработки фильтра
  fps = 1000 / deltaMillis;

  if ((millis() - midiSendMillis) >= 100) {

    myMidi.sendNoteOff(sendedNote, 0, channel);     // Stop the note

    sendedNote = map(yaw, -180, 180, 0, 127);

    myMidi.sendNoteOn(sendedNote, 100, channel);    // Send a Note (pitch 42, velo 127 on channel 1)

    midiSendMillis = millis();

  }
  byte cutoff = map(pitch, -180, 180, 0, 127);

  byte resonance = map(roll, -180, 180, 0, 127);
  
  myMidi.sendControlChange(102, cutoff, channel);
  myMidi.sendControlChange(103, resonance, channel);
}
