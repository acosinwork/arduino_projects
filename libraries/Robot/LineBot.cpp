#include <inttypes.h>
#include <LineBot.h>
#include <Arduino.h>

#include <Strela.h>


void LineBot::begin{
	Robot::begin();


	for (int i=0; i< 10; ++i)
	{
		tone(BUZZER)
	}


}