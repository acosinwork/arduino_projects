#include <Arduino.h>
#include "LineSensor.h"

#define AVERAGE_COUNT_FOR_SHIFT_DIV 4 

uint8_t LineSensor::attach(uint8_t pin)
{

	if (_usedSensCount < maxSensorsCount) {

		_sensors[_usedSensCount]._pin = pin;

		pinMode(pin, INPUT);

		return ++_usedSensCount;
	} else
	    return 0;
}

uint8_t LineSensor::count()
{
	return _usedSensCount;
}


int LineSensor::readOnce(uint8_t sensor)
{
	return analogRead(_sensors[sensor]._pin);
}

int LineSensor::read(uint8_t sensor)
{
	unsigned int sum = 0;

	for (uint8_t i = 0; i < AVERAGE_COUNT_FOR_SHIFT_DIV; ++i)
		sum += analogRead(_sensors[sensor]._pin);

	for (uint8_t i = AVERAGE_COUNT_FOR_SHIFT_DIV; i > 1; i >>= 1) // shift to divide
		sum >>= 1;
	
	int result = sum;
	return result;
}


void LineSensor::setWhite(uint8_t sensor)
{
	_sensors[sensor]._whiteVal = read(sensor);
}

void LineSensor::setBlack(uint8_t sensor)
{
	_sensors[sensor]._blackVal = read(sensor);
}

uint8_t LineSensor::readPct(uint8_t sensor)
{
	int val = read(sensor);

	val = constrain(val, _sensors[sensor]._whiteVal, _sensors[sensor]._blackVal);

	return map(val, _sensors[sensor]._whiteVal, _sensors[sensor]._blackVal, 0, 100);
}
