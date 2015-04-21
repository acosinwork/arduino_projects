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


int LineSensor::readOnce(uint8_t sensor)
{
	return analogRead(_sensors[sensor]._pin);
}

int LineSensor::read(uint8_t sensor)
{
	unsigned long sum = 0;

	for (uint8_t i = 0; i < AVERAGE_COUNT_FOR_SHIFT_DIV; ++i)
		sum += analogRead(_sensors[sensor]._pin);

	for (uint8_t i = AVERAGE_COUNT_FOR_SHIFT_DIV; i > 1; i >>= 1) // shift to divide
		sum >>= sum;
	
	int result = sum;
	return result;
}

