// Include the Modulo library and the Wire library, which it depends on.
#include "Modulo.h"
#include "Wire.h"

// Create an object that represents each modulo
MotorDriverModulo motorDriver;
JoystickModulo joystick;

// The setup function is run once, when the program starts
void setup() {

}

// The loop function is run constantly
void loop() {
    // Always call Modulo.loop() at the top of the loop function. It
    // communicates with the modulos and executes callbacks if any events
    // have occured.
    Modulo.loop();

    // Get the horizontal and vertical position if the joystick
    float hpos = joystick.getHPos();
    float vpos = joystick.getVPos();

    // Set the speed of motor A. 
    motorDriver.setMotorA(vpos + hpos);
    motorDriver.setMotorB(vpos - hpos);
}
