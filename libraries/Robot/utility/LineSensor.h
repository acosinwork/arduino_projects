#include <Arduino.h>
#include <vector>

#define ROLE_WHITE = 0
#define ROLE_BLACK = 1
#define MAX_SENSORS_COUNT 8

/*
class LineSensor
{

	public:

		LineSensor();

		setPin(uint8_t pin);

		int read();
		
		int readOnce();

	private:

	uint8_t _pin;
	uint16_t _blackVal;
	uint16_t _whiteVal;
	
};
*/


class LineSensor
	{
	public:
		LineSensor();

		uint8_t attach(uint8_t pin);

		int read(uint8_t sensor);
		int readOnce(uint8_t sensor);



	private:

		struct Sensor{
		    uint8_t _pin;
    	    uint16_t _blackVal;
	        uint16_t _whiteVal;
	        bool role;
		};

		const maxSensorsCount = MAX_SENSORS_COUNT;
		LineSensor::Sensor _sensors[MAX_SENSORS_COUNT];
		uint8_t _usedSensCount = 0;
	
};
