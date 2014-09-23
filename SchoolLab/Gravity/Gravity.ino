
#include <Scool.h>

#define TRESHOLD   10
#define MID_A_VAL  511
int data[4];

unsigned long times[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int a = analogRead(Z4);
  while (abs(analogRead(Z4) - a) < TRESHOLD)
    ;
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < 4; ++i) {
    times[i] = getMaxValueTime(A2 + i);
  }

  for (int i = 0; i < 4; ++i)
    Serial.println(times[i]);

  while (true)
    ;
  //  float allData = 0;
  /*
    for (int i = 0; i < 4; ++i)
      data[i] = MID_A_VAL + abs(analogRead(A2 + i) - MID_A_VAL);



    Serial.println(analogToVoltage(maximum(data, 4)));
    //  delay(1);
   */
}

int maximum(int *array, int arrayCount)
{
  int curMax = 0;
  for (int i = 0; i < arrayCount; ++i) {
    if (array[i] > curMax)
      curMax = array[i];
  }

  return curMax;
}

unsigned long getMaxValueTime(int aInput)
{
//  int maxVal;
//  unsigned long maxValTime = millis();
  
  int prevVal = hallVal(aInput);
  while ((hallVal(aInput)) >= prevVal+TRESHOLD)
    prevVal = hallVal(aInput);

  return micros();
}

int hallVal(int analogInput)
{
  return abs(analogRead(analogInput) - MID_A_VAL);
}


