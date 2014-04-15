#include <Strela.h>

Strela myRobot;

void setup() {
  // put your setup code here, to run once:3
  myRobot.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  myRobot.motors(200,-30);

}
