
#include "MotorCalibration.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Strela.h>

#define MT_CB_DEFAULT_SPEED 255
#define MT_CB_CALIBRATION_START_BUTTON S1
#define MT_CB_LEFT_MOTOR_SLOW S2
#define MT_CB_RIGHT_MOTOR_SLOW S4
#define LOWER_BUTTON_NUM S1
#define LOWER_LED_NUM L1
#define MT_CB_CALIBR_STEP 1
#define MT_CB_TONE 5000


#define MT_CB_EEPROM_ADDR 127
#define SALT 21


namespace {

    void EEPROMWriteInt(int address, int value)
	{
	  EEPROM.write(address, lowByte(value));
	  EEPROM.write(address + 1, highByte(value));
	}

	int EEPROMReadInt(int address)
	{
	  byte lowByte = EEPROM.read(address);
	  byte highByte = EEPROM.read(address + 1);
	  return (highByte << 8) | lowByte;
	}

	int _calcHash(int speed1, int speed2)
	{
		return speed1 + speed2 + SALT; 
	}
	 
	void _writeCalibratedValue(int _speedMot[])
	{
		int adr = MT_CB_EEPROM_ADDR;
	    int spmSize = sizeof(_speedMot[0]);

		EEPROMWriteInt(adr, _speedMot[0]);
		EEPROMWriteInt(adr += spmSize, _speedMot[1]);
		EEPROMWriteInt(adr += spmSize, _calcHash(_speedMot[0], _speedMot[1]));
	}

	void _readCalibratedValue(int _speedMot[])
	{
	    
	    int eepromData[3];
	    int spmSize = sizeof(eepromData[0]);
	    int adr = MT_CB_EEPROM_ADDR;

	    for (int i = 0; i < 3; ++i)
	    	eepromData[i] = EEPROMReadInt(adr + spmSize*i);
	    
	    if (eepromData[3] == _calcHash(eepromData[0], eepromData[1])) {
	    	_speedMot[0] = eepromData[0];
	    	_speedMot[1] = eepromData[1];
	    } else
	        _speedMot[0] = _speedMot[1] = MT_CB_DEFAULT_SPEED;
	}

	bool _needCalibration()
	{

		bool result = false;

		unsigned long pressTime = millis();

		while (uDigitalRead(MT_CB_CALIBRATION_START_BUTTON)) {
			if ((millis() - pressTime) > 100) {
				result=true;
				tone(BUZZER, MT_CB_TONE, 20);
			}
		}

		if (result)
			delay(50);

		return result;
	}

	bool checkButtonClick(int button, bool* buttonWasUp)
	{
		bool leftButtonDown = uDigitalRead(button);
		bool result;

		if (result = leftButtonDown && *buttonWasUp)
		{
			uDigitalWrite(LOWER_LED_NUM, HIGH);
			tone(BUZZER, MT_CB_TONE, 20);
			delay(20);
			uDigitalWrite(LOWER_LED_NUM, LOW);
		}
		*buttonWasUp = !leftButtonDown;
		return result;
	}



	bool _calibrate(int _speedMot[])
	{
		bool leftButWasUp = true;
		bool rightButWasUp = true;

		int correction = 0;

		while (!(uDigitalRead(MT_CB_CALIBRATION_START_BUTTON) || uDigitalRead(MT_CB_CALIBRATION_START_BUTTON)))
		{
			drive(_speedMot[0], _speedMot[1]);

			if (checkButtonClick(MT_CB_LEFT_MOTOR_SLOW, &leftButWasUp))
			{
				if (_speedMot[1] < 255)
					_speedMot[1] += MT_CB_CALIBR_STEP;
				else
					_speedMot[0] -= MT_CB_CALIBR_STEP;
			}
			
			if (checkButtonClick(MT_CB_RIGHT_MOTOR_SLOW, &rightButWasUp))
			{
				if (_speedMot[0] < 255)
					_speedMot[0] += MT_CB_CALIBR_STEP;
				else
					_speedMot[1] -= MT_CB_CALIBR_STEP;
			}
			
		}

		tone(BUZZER, MT_CB_TONE, 50);
		delay(100);
		tone(BUZZER, MT_CB_TONE, 50);
	}


}

void MotorCalibration(int (&maxSpeed)[2])
{

	int currentMotorSpeed[2];

	_readCalibratedValue(currentMotorSpeed);

	if (_needCalibration()) {
		_calibrate(currentMotorSpeed);
		_writeCalibratedValue(currentMotorSpeed);
	}

	maxSpeed[0] = currentMotorSpeed[0];
	maxSpeed[1] = currentMotorSpeed[1];
}
