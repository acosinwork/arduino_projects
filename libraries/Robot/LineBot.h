#ifndef LINEBOT_H
#define LINEBOT_H

#include <inttypes.h>
#include <Robot.h>

#include <utility/LineSensor.h>


class LineBot : public Robot
{
	public:
		LineBot();

		LineSensor sensors;

		void begin();

};

#endif