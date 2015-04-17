#ifndef ROBOT_H
#define ROBOT_H

#include <inttypes.h>
#include <Strela.h>

class Robot {

	public:
		void Robot();

/*
		void forvard(int speed);

		void backvard(int speed);

		void stop();

		void acceleration(int8_t accel);
*/
		virtual void run();

	protected:
		begin();

    private:

    	int _motorMaxSpeed[2];

/*
    	uint8_t _acceleration;

    	uint8_t _currentSpeed;
*/

};

#endif