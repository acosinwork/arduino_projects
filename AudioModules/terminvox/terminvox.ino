#define BUTTON 9
#define PHOTO A4
#define AUDIO 4

void setup() {
}

void loop() {
  // put your main code here, to run repeatedly:
  bool isButtonDown = !digitalRead(BUTTON);
  if (isButtonDown)
    tone(AUDIO, map(analogRead(PHOTO), 85, 600, 50, 10000));
  else
    noTone(AUDIO);
}
