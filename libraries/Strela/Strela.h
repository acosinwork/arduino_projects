/*
 * Strela.h
 *
 *
 */

 
#ifndef STRELA_H
#define STRELA_H

#include <inttypes.h> 

#define GPUX_TWI_ADDR            0x20
#define LCD_TWI_ADDR             0x21
#define WIRE_IO_CONFIG_MODE      0x03
#define WIRE_IO_CONFIGURATION    0x0F
#define WIRE_OUTPUT_WRITE_MODE   0x01
#define WIRE_INPUT_READ_MODE     0x00


// IO-pin defines

#define MOTOR_ENABLE_12_PIN      10
#define MOTOR_ENABLE_34_PIN      9
#define MOTOR_DIRECTION_34_PIN   4
#define BUZZER                   12

#define P1  11
#define P2  13
#define P3  5
#define P4  6

#define P5  A3
#define P6  A2
#define P7  A1
#define P8  A0

#define P9  8
#define P11 A4
#define P12 A5

#define IR_PIN P12

//I2C-expander IO pins

#define P10 30 //NUM_DIGITAL_PINS //100500 // ERROR, всегда вернёт LOW



//Buttons
#define S1 0
#define S2 1
#define S3 2
#define S4 3
//Leds
#define L1 0
#define L2 1
#define L3 2
#define L4 3
//Motors
#define M1 0
#define M2 1
 
class Strela_t
{
    public:
        void begin();
        
        void motors(
            int motorSpeed_1,
            int motorSpeed_2);
            
        void motorSpeed(
            uint8_t mot,
            int speed);
        
        void motorConnection(
            bool direction_1,
            bool direction_2);
            
        void ledWrite(
            uint8_t ld,
            bool state);    
            
        bool buttonRead(uint8_t btn);
        
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
            
        void _setPWM9(uint8_t pwm);
        void _setPWM10(uint8_t pwm);
        
        
        
};
extern Strela_t Strela; 

#endif
