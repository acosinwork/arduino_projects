uint8_t ledCount = 5;
uint8_t leds[] = {
  A3,
  A4,
  A2,
  0,
  A0
};

uint8_t micro = A1;

unsigned long time = 0;

void setup() {
  // put your setup code here, to run once:

  for (int i = 0; i < ledCount; ++i) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - time > 50) {
    for (int i = 0; i < ledCount; ++i) {
      digitalWrite(leds[i], LOW);
    }
    int loudness = analogRead(micro);
    uint8_t k = map(loudness, 0, 600, 0, ledCount);
    digitalWrite(leds[k], HIGH);
    time = millis();
  }
}
