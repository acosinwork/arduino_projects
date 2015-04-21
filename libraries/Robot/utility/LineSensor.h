#include <Arduino.h>

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
//		LineSensor();

		uint8_t attach(uint8_t pin);

		int read(uint8_t sensor);
		int readOnce(uint8_t sensor);

		void setWhite(uint8_t sensor);
		void setBlack(uint8_t sensor);

		uint8_t readPct(uint8_t sensor);
		


		uint8_t count();

	private:

		struct Sensor{
		    uint8_t _pin;
    	    uint16_t _blackVal;
	        uint16_t _whiteVal;
	        bool role;
		};

		const uint8_t maxSensorsCount = MAX_SENSORS_COUNT;
		LineSensor::Sensor _sensors[MAX_SENSORS_COUNT];
		uint8_t _usedSensCount = 0;
	
};
