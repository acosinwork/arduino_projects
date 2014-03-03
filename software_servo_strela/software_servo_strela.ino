

#include <SoftwareServo.h> 

SoftwareServo myservo;  // create servo object to control a servo 

int potpin = 0;  // analog pin used to connect the potentiometer
int val=0;    // variable to read the value from the analog pin 

void setup() 
{ 
  myservo.attach(6);  // attaches the servo on pin 2 to the servo object 
  pinMode(9, OUTPUT);
} 

void loop() 
{ 
//  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
 if (val>=180) {
   val=0;
 };
  val += 10;     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(100);                           // waits for the servo to get there 

  SoftwareServo::refresh();
  
  analogWrite(9, 50);
  
} 
