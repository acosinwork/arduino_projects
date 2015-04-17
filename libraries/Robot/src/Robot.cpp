#include "Robot.h"
#include <Strela.h>

#include "utility/MotorCalibration.h"

#define DEFAULT_ACCEL 50

void Robot::Robot()
{
	strelaInitialize();

	MotorCalibration(_motorMaxSpeed);

	/*TODO*/

//	Robot::acceleration(DEFAULT_ACCEL);
}

/*

void acceleration(uint8_t accel)
{
	_acceleration = accel;
}

*/