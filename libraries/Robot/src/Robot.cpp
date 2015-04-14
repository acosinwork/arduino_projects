#include "Robot.h"

#include <Strela.h>

#define DEFAULT_ACCEL 50

void Robot::Robot()
{
	strelaInitialize();

	Robot::acceleration(DEFAULT_ACCEL);
}



void acceleration(uint8_t accel)
{
	_acceleration = accel;
}

