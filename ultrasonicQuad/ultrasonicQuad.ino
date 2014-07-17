#include <QuadDisplay.h>
//Отображаем количество секунд, прошедшее со старта скетча
#define DISPLAY_PIN A5
#define COUNTER 10

#include <Ultrasonic.h>

int trigpin = 4;//appoint trigger pin
int echopin = 5;//appoint echo pin

int vcc = 4;
int gnd = 7;


Ultrasonic ultrasonic(trigpin, echopin);

void setup() {

  pinMode(vcc, OUTPUT);
  pinMode(gnd, OUTPUT);

  digitalWrite(vcc, HIGH);
  digitalWrite(gnd, LOW);


}

void loop() {

  float cmdistance;

  long microsec;

  for (int i = 0; i < COUNTER; ++i)
  {
    microsec = ultrasonic.timing();

    cmdistance += ultrasonic.CalcDistance(microsec, Ultrasonic::CM); //this result unit is centimeter
    delay(20);
  }

  cmdistance = cmdistance / COUNTER;
 
  displayFloat(DISPLAY_PIN, cmdistance, 1);


}
