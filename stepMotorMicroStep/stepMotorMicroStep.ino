#define E1    5;
#define E2    6;
#define H1    4;
#define H2    7;

// Step Mode:
#define WAWE_DRIVE  0;
#define FULL_STEP   1;
#define HALF_STEP   2;




int delayTime = 20;

void doStep(boolean E1State, boolean H1State, boolean E2State, boolean H2State)
{
  digitalWrite(E1, E1State);
  digitalWrite(H1, H1State);
  digitalWrite(E2, E2State);
  digitalWrite(H2, H2State);
  delay(delayTime);
}

void setup() {
  for (int i = 4; i < 8; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //Однофазный режим
  /*
  doStep(1, 1, 0, 0);
  doStep(0, 0, 1, 1);
  doStep(1, 0, 0, 0);
  doStep(0, 0, 1, 0);
  */

  //Полнофазный режим
  /*
  doStep(1, 1, 1, 1);
  doStep(1, 0, 1, 1);
  doStep(1, 0, 1, 0);
  doStep(1, 1, 1, 0);
  */

  //Полушаговый режим
  /*
  doStep(1, 1, 0, 0);
  doStep(1, 1, 1, 1);
  doStep(0, 0, 1, 1);
  doStep(1, 0, 1, 1);

  doStep(1, 0, 0, 0);
  doStep(1, 0, 1, 0);
  doStep(0, 0, 1, 0);
  doStep(1, 1, 1, 0);
  */
  
  doStep(1, 1, 0, 0);
  analogWrite(E1, 221); //cos 30
  analogWrite(E2, 127); //sin 30
  delay(delayTime);
  doStep(1, 1, 1, 1);
  analogWrite(E1, 127);
  delay(delayTime);
  doStep(0, 0, 1, 1);
  analogWrite(E1, 127);
  delay(delayTime);
  doStep(1, 0, 1, 1);
  analogWrite(E2, 127);
  delay(delayTime);

  doStep(1, 0, 0, 0);
  analogWrite(E2, 127);
  delay(delayTime);  
  doStep(1, 0, 1, 0);
  analogWrite(E1, 127);
  delay(delayTime);
  doStep(0, 0, 1, 0);
  analogWrite(E1, 127);
  delay(delayTime);
  doStep(1, 1, 1, 0);
  analogWrite(E2, 127);
  delay(delayTime);
  
  

}
