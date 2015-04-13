#ifndef ROBOT_H
#define ROBOT_H

#include <inttypes.h>

class Robot {

	public:
		void Robot();

		void forvard(int speed);

		void backvard(int speed);

		void stop();

		virtual void run();

	protected:
		begin();

};

#endif