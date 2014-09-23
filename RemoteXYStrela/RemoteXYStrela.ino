#include <Wire.h>
#include <Strela.h>
#include "remotexy.h"



void setup()
{
  RemoteXY_Init ();



  // TODO you setup code

}

void loop()
{
  RemoteXY_Handler ();

  if (RemoteXY.connect_flag)
    drive(RemoteXY.joystick_1_x, RemoteXY.joystick_1_y);
  else
    drive(0, 0);
    
  RemoteXY.level_1=map(analogRead(A0), 0, 100, 0, 1023);


  // TODO you loop code
  // используйте структуру RemoteXY для передачи данных


}
