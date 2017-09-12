#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, myMidi);


void setup() {
  // put your setup code here, to run once:
  myMidi.begin(4);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

  myMidi.sendRealTime(midi::MidiType::Clock);
  delay(64-(analogRead(A4)>>4));
}
