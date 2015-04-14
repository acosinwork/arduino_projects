
#include "MotorCalibration.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Strela.h>

#define DEFAULT_SPEED 255
#define CALIBRATION_START_BUTTON S1

void MotorCalibration::MotorCalibration(int (&maxSpeed)[2])
{

    speedMot[0] = DEFAULT_SPEED;
    speedMot[1] = DEFAULT_SPEED;
    
	if (_needCalibration()) {
		_calibrate();
	}

}

bool MotorCalibration::_needCalibration()
{

	bool result = true;
    
    uint8_t counter = 3;
	while (--counter) {
		result = result && uDigitalRead(CALIBRATION_START_BUTTON);
		delay(10);
	}

	return result;
}



