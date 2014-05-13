// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object 
Serial.begin(9600);

} 
 
 
void loop() 
{ 
  for(pos = 20; pos < 160; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);          
  int sensorValue = analogRead(A3);
//  tone(12, 1000*(1/sensorValue));
  // print out the value you read:////
  
//  Serial.println(4800/(sensorValue - 20));
    // tell servo to go to position in variable 'pos' 
    delay(100);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 160; pos>=20; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                              
    
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
   int sensorValue = analogRead(A3);
//     tone(12, 1000*(1/sensorValue));
  // print out the value you read:
//  Serial.println(4800/(sensorValue - 20));
    delay(100);                       // waits 15ms for the servo to reach the position 
  } 
} 
