#include <School.h>
#include <QuadDisplay.h>

#define RESISTORS 4
byte resistors[] = { 7, 8, 9, 10 };
byte buttons[] = { W4, W1, Z1, Z4 };
byte voltagePin = A0;
byte displayPin = W3_UP;

unsigned long lastTime;

void setup() {
  for (int i = 0; i < RESISTORS; ++i) {
    pinMode(buttons[i], INPUT);
    pinMode(resistors[i], OUTPUT);
    
  }

  lastTime = millis();
}

void loop() {

  for (int i = 0; i < RESISTORS; ++i) {
    boolean buttonIsPressed = !digitalRead(buttons[i]);
    digitalWrite(resistors[i], buttonIsPressed);
  }

  if (millis() - lastTime > 500) {
    float voltage = voltageRead(voltagePin);
    displayFloat(displayPin, voltage, 2, true);
    lastTime = millis();
  }
}
