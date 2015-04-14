#ifndef MOTOR_CALIBRATION_H
#define MOTOR_CALIBRATION_H


#include <inttypes.h>

class MotorCalibration
{
	public:
	void MotorCalibration(int (&maxSpeed)[2]);


//    void calibration();
    private:

    	uint8_t speed[2];

    	bool MotorCalibration::_needCalibration();

    	
};


#endif