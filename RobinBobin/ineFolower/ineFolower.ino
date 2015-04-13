#include <Wire.h>
#include <Strela.h>

#define LINE_R P7
#define LINE_L P8
#define BLINK_TIME 100;

int sensorsLR[] = {LINE_L, LINE_R};
int whiteBlackLR[2][2];


unsigned long time;

void setup() {
  // put your setup code here, to run once:
  /*  for (int sensorNumber = 0; sensorNumber < 2; ++sensorNumber) {

      while (!uDigitalRead(S1 + sensorNumber*2))
      {
        // Определим белое
      uDigitalWrite(L1+ sensorNumber*2, HIGH);
      whiteBlackLR[sensorNumber]

    }

    }
    */

  // Определим белое
  boolean ledState = HIGH;
  uDigitalWrite(L1, ledState);
  while (!uDigitalRead(L1))
  {
    if (millis() - time > BLINK_TIME)
    {
      ledState = !ledState;
      uDigitalWrite(L1, ledState);
      time = millis();
    }
  }


}

void loop() {
  // put your main code here, to run repeatedly:

}
