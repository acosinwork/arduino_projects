#include <School.h>

#define START 0
#define EXPERIMENT 1
#define STOP 2
#define LAST 3
#define LENGTH 25.4

unsigned long startTime, stopTime, middleTime;

float middleValue;
int state = START;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  unsigned long sum = 0;
  int counter = 0;
  while (!Serial)
  {
    sum += (analogRead(Z1) + analogRead(Z2) + analogRead(Z3));
    ++counter;
  }

  float middleValue = 5.0 * sum / (3 * counter * 1023.0);
}

void loop() {
  // put your main code here, to run repeatedly:
  float currentValue = (voltageRead(Z1) + voltageRead(Z2) + voltageRead(Z3)) / 3.0;
  Serial.println(currentValue); //+analogRead(Z1))/4);

  if ((state == START) && (abs(currentValue - middleValue) > 1.2 * middleValue))
  {
    state = EXPERIMENT;
    startTime = millis();
  }
  if ((state == EXPERIMENT) && (abs(voltageRead(Z1) - middleValue) > 1.2 * middleValue))
  {
    state = STOP;
    stopTime = millis();
  }

  if (state == STOP)
    Serial.println(startTime - stopTime);
  while (state == STOP)

    ;;
  //  delay(1);
}


