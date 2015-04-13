#include <QuadDisplay.h>
#include <School.h>

#define TRESHOLD 0.03

bool positive = true;
unsigned int showTime = 0;
unsigned long lastChangeTime = 0;
unsigned long displayTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float result = voltageRead(A0);

  if (result < 0 + TRESHOLD) {
    isRising(true);
    digitalWrite(2, HIGH);
  }
  else if (result > 5 - TRESHOLD) {
    isRising(false);
    digitalWrite(2, LOW);
  }

  Serial.println(result);//voltageRead(Y1));
/*
  if (millis() - displayTime > 500)
  {
    displayTime = millis();
    displayInt(W2_UP, showTime);
  }
  */
}

void isRising(bool isIt)
{
  if (isIt != positive)
  {
    showTime = millis() - lastChangeTime;
    lastChangeTime = millis();
    positive = !positive;
  }
}

