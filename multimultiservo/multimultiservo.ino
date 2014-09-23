#include <Wire.h>
#include <Multiservo.h>

#define SERVO_COUNT 18

XServo servo[SERVO_COUNT];

int pos = 0;

void setup(void)
{
  Wire.begin();

  for (int i = 0; i < SERVO_COUNT; ++i)
  {
    servo[i].attach(i);
    servo[i].write(0);
  }
}

void loop(void)
{
  for (pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
  { // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
}
