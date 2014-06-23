#include <Wire.h>
#include <Strela.h>

byte xVal = 0;
byte yVal = 0;

byte xBias = 0;
byte yBias = 0;

int forwardBackward;
int leftRight;
unsigned long time;

bool connectionFail = true;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(1200);
  // while (!Serial) {
  ; // wait for serial port to connect. Needed for Leonardo only
  // }

  Strela.begin();
  time = millis();
  // set the data rate for the SoftwareSerial port

}

void loop() // run over and over
{
  // delay(1000);

  if ((millis() - time) > 60)
  {

    connectionFail = true;


  }

  recieveCommands();

  /*
      Serial.print("X =    ");
      Serial.println(leftRight);
      Serial.print("Y =    ");
      Serial.println(forwardBackward);
  */

  if (connectionFail)
  {
    Strela.motors(0, 0);

    
  }
  else
    Strela.motors(forwardBackward - leftRight, forwardBackward + leftRight);
    
Strela.ledWrite(L4, connectionFail);

}

void recieveCommands()
{

  byte recieveMsg;

  if ( Serial1.available())
  {
    time = millis();
    connectionFail = false;
    recieveMsg = Serial1.read();
    if (recieveMsg & 0b10000000)
    {
      yVal = 0b01111111 & recieveMsg;
      //      Serial.print("Y = ");
      //      Serial.println(yVal);
      forwardBackward = map(yVal, 127, 0, -255, 255) + 3;
      Strela.ledWrite(L1, HIGH);
      Strela.ledWrite(L2, LOW);

    }
    else
    {
      xVal = 0b01111111 & recieveMsg;
      //      Serial.print("X = ");
      //      Serial.println(xVal);
      leftRight = map(xVal, 127, 0, -255, 255) - 6;
      Strela.ledWrite(L1, LOW);
      Strela.ledWrite(L2, HIGH);
    }



  }

}
