/*
 * Strela.h
 *
 *
 */
 
#ifndef STRELA_H
#define STRELA_H

#include <inttypes.h> 

#define RESOLUTION 65536    // Timer3 is 16 bit

#define GPUX_TWI_ADDR            0x20
#define WIRE_IO_CONFIG_MODE      0x03
#define WIRE_IO_CONFIGURATION    0x0F
#define WIRE_OUTPUT_WRITE_MODE   0x01
#define WIRE_INPUT_READ_MODE     0x00

#define MOTOR_ENABLE_12_PIN      10
#define MOTOR_ENABLE_34_PIN      9
#define MOTOR_DIRECTION_34_PIN   4



enum BUTTON
{
    S1,
    S2,
    S3,
    S4    
};

enum LED
{
    L1,
    L2,
    L3,
    L4    
};

enum MOTOR
{
    M1,
    M2
};
 
class Strela_t
{
    public:
        void begin();
        
        void motors(
            int motorSpeed_1,
            int motorSpeed_2);
            
        void motorSpeed(
            MOTOR mot,
            int speed);
        
        void motorConnection(
            bool direction_1,
            bool direction_2);
            
        void ledWrite(
            LED ld,
            bool state);    
            
        bool buttonRead(BUTTON btn);
        
        void onTimerInterrupt();

    
    private:
        uint8_t _currentMotorSpeed_1;
        uint8_t _currentMotorSpeed_2;
        uint8_t _pwmCounter;
        bool _motorConnection_1;
        bool _motorConnection_2;
        void _setMotorDirections(
            bool direction_1,
            bool direction_2);
        
        
};
extern Strela_t Strela; 

#endif
