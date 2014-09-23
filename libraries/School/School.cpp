
#include <School.h>
#include <Arduino.h>


#define VOLT_MULT (5.0/1023)

float voltageRead(uint8_t pin)
{
	return analogToVoltage(analogRead(pin));
}

float analogToVoltage(int val)
{
	return VOLT_MULT*val;
}