

#include <Wire.h>
#include <Strela.h>
#include <notes.h>
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 



//int note[] = {262, 262, 392, 523, 392, 784};
int note[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
byte noteLed[] = {0, 1, 2, 3, 2, 3};

int duration[] = {100, 100, 100, 300, 100, 300};

bool motorEnable=false;

void setup() {
    myservo.attach(11);  // attaches the servo on pin 9 to the servo object 

  // put your setup code here, to run once:
  Strela.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Strela.buttonRead(S1))
  {

    for (int i = 0; i < 4; ++i)
    {
      Strela.ledWrite(i, true);
      delay(100);
      Strela.ledWrite(i, false);

    }
  }
  if (Strela.buttonRead(S2))
  {

    for (int thisNote = 0; thisNote < 6; thisNote ++) {
      // play the next note:
      Strela.ledWrite(noteLed[thisNote], true);
      tone(BUZZER, note[thisNote], duration[thisNote]);

      // hold the note:
      delay(duration[thisNote]);
      Strela.ledWrite(noteLed[thisNote], false);

    }
  }

  if (Strela.buttonRead(S3))
  {
    
    delay(1000);
 /*   motorEnable = !motorEnable;
    if (motorEnable)
    
    Strela.motors(50, 255);
    else*/
    for (int i=0; i<256;++i)
    {
    Strela.motors(i, -i);
    analogWrite(6,i);
    delay(10);
    }
//    delay(500);

  }
    if (Strela.buttonRead(S4))
  {
  for(pos = 20; pos < 160; pos += 10)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);          

    delay(100);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 160; pos>=20; pos-=10)     // goes from 180 degrees to 0 degrees 
  {                              
    
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 

    delay(100);                       // waits 15ms for the servo to reach the position 
  } 
    myservo.write(90);              // tell servo to go to position in variable 'pos' 

  
  }

}


