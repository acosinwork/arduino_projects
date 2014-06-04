/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/

String serialString;

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup()
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(7, INPUT);

/*  Serial.begin(9600);
  while (!Serial.available() > 0) {
    for (pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
    { // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
*/
}

void loop()
{
  if (digitalRead(7))
 
   myservo.write(180);
   else
   myservo.write(0);
   
//   delay(100);

/*  while (Serial.available() > 0)
  {

    char n = Serial.read();

    if (n == '\n')
    {
      int degree = serialString.toInt();
      if ((degree >= 0) && (degree <= 180))
      {
        myservo.write(degree);
        Serial.println(degree);
        serialString = "";
      }

    }
    else
      serialString += (char)n;


  }*/
}
