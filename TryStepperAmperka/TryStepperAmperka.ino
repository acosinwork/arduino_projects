#include <StepperAmperka.h>

//StepperAmperka motor = StepperAmperka(200, 4, 7);
StepperAmperka motor = StepperAmperka(200, 4, 5, 6, 7);

void setup()
{
/*  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  */
//  motor.direction=1;
      motor.setSpeed(40);
}

void loop()
{

    
    motor.step(100);
    delay(1000);
    
//        motor.setSpeed(40);
        
            delay(1000);
}
