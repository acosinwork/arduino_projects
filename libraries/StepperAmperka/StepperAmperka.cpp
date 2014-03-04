/*
  Stepper.cpp - - Stepper library for Wiring/Arduino - Version 0.4
  
  Original library     (0.1) by Tom Igoe.
  Two-wire modifications   (0.2) by Sebastian Gassner
  Combination version   (0.3) by Tom Igoe and David Mellis
  Bug fix for four-wire   (0.4) by Tom Igoe, bug fix from Noah Shibley  

  Drives a unipolar or bipolar stepper motor using  2 wires or 4 wires

  When wiring multiple stepper motors to a microcontroller,
  you quickly run out of output pins, with each motor requiring 4 connections. 

  By making use of the fact that at any time two of the four motor
  coils are the inverse  of the other two, the number of
  control connections can be reduced from 4 to 2. 

  A slightly modified circuit around a Darlington transistor array or an L293 H-bridge
  connects to only 2 microcontroler pins, inverts the signals received,
  and delivers the 4 (2 plus 2 inverted ones) output signals required
  for driving a stepper motor.

  The sequence of control signals for 4 control wires is as follows:

  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  1  1  0
     3  0  1  0  1
     4  1  0  0  1

  The sequence of controls signals for 2 control wires is as follows
  (columns C1 and C2 from above):

  Step C0 C1
     1  0  1
     2  1  1
     3  1  0
     4  0  0

  The circuits can be found at 
 
http://www.arduino.cc/en/Tutorial/Stepper
 
 
 */


#include "Arduino.h"
#include "StepperAmperka.h"

/*
 *   constructor for default configuration
 *   Sets number of steps in motor.
 */

 
StepperAmperka::StepperAmperka(int number_of_steps)
{
  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor
  
  // Arduino pins for the motor control connection:
  this->motor_pin_1 = 4;
  this->motor_pin_2 = 5;
  this->motor_pin_3 = 6;
  this->motor_pin_4 = 7;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  pinMode(this->motor_pin_3, OUTPUT);
  pinMode(this->motor_pin_4, OUTPUT);

  // pin_count is used by the stepMotor() method:  
  this->stepType = FULL_STEP;  
  
    digitalWrite(this->motor_pin_2, HIGH);
    digitalWrite(this->motor_pin_3, HIGH);

}

/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

StepperAmperka::StepperAmperka(int number_of_steps, unsigned char motor_pin_1, unsigned char motor_pin_2, unsigned char motor_pin_3, unsigned char motor_pin_4, unsigned char stepType)
{
  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor
  
  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  pinMode(this->motor_pin_3, OUTPUT);
  pinMode(this->motor_pin_4, OUTPUT);

  // pin_count is used by the stepMotor() method:  
  this->stepType = constrain(stepType, 0, 2);  
  
  if (this->stepType==FULL_STEP) {
    digitalWrite(this->motor_pin_2, HIGH);
    digitalWrite(this->motor_pin_3, HIGH);
  }
}

/*
  Sets the speed in revs per minute

*/
void StepperAmperka::setSpeed(long whatSpeed)
{
  if (stepType==HALF_STEP) 
  {
    this->step_delay = 60L * 1000L / (this->number_of_steps * 2L) / whatSpeed;
  }
  else 
  {
    this->step_delay = 60L * 1000L / this->number_of_steps / whatSpeed;
  }
}

/*
  Moves the motor steps_to_move steps.  If the number is negative, 
   the motor moves in the reverse direction.
 */
void StepperAmperka::step(int steps_to_move)
{  
  int steps_left = abs(steps_to_move);  // how many steps to take
  
  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) {this->direction = 1;}
  if (steps_to_move < 0) {this->direction = 0;}
    
    
  // decrement the number of steps, moving one step each time:
  while(steps_left > 0) {
  
  // move only if the appropriate delay has passed:
  if (millis() - this->last_step_time >= this->step_delay) {
      // get the timeStamp of when you stepped:
      this->last_step_time = millis();
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1) {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      } 
      else { 
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;

      // WAVE_DRIVE, FULL_STEP, HALF_STEP 
      if (stepType==HALF_STEP) {
      // step the motor to step number 0, 1, 2, 3, 4, 5, 6, 7, 8:
        stepMotor(this->step_number % 8);
      }
      else {
      // step the motor to step number 0, 1, 2, or 3:
      stepMotor(this->step_number % 4);
      }
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void StepperAmperka::stepMotor(int thisStep)
{
  if (this->stepType == FULL_STEP) {
    switch (thisStep) {
      case 3: /* 01 */
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 2: /* 11 */
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 1: /* 10 */
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 0: /* 00 */
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_4, LOW);
      break;
    } 
  }
  if (this->stepType == WAVE_DRIVE) {
    switch (thisStep) {
      case 0:    // 1010
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 1:    // 0110
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 2:    //0101
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:    //1001
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
    } 
  }
    if (this->stepType == HALF_STEP) {
    switch (thisStep) {
      case 0:    // 1, 1, 0, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 1:    // 1, 1, 1, 1
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 2:    //0, 0, 1, 1
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:    //1, 0, 1, 1
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, HIGH);
      break;
      
      case 4:    // 1, 0, 0, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 5:    // 1, 0, 1, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 6:    //0, 0, 1, 0
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 7:    //1, 1, 1, 0
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      }
    }
}

/*
  version() returns the version of the library:
*/
int StepperAmperka::version(void)
{
  return 4;
}
