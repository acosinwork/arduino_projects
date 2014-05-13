/* Strela.cpp
 * Strela library 
 *
 *
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include <Arduino.h>
#include <Wire.h> 

#include <Strela.h>


Strela_t Strela;


ISR(TIMER4_OVF_vect)          // interrupt service routine for software PWM
{
   PORTB |= _BV(5); //pin 9 HIGH

}

ISR(TIMER4_COMPD_vect)          // interrupt service routine for software PWM
{
    PORTB &= ~_BV(5); //pin 9 LOW

}


void Strela_t::begin()
{
    // Variables initialize
    
    _currentMotorSpeed_1 = 0;
    _currentMotorSpeed_2 = 0;
    _pwmCounter = 0;
    _motorConnection_1 = 0;
    _motorConnection_2 = 0;

    // Configure I2C i/o chip
    Wire.begin();
    Wire.beginTransmission(GPUX_TWI_ADDR);
    Wire.write(WIRE_IO_CONFIG_MODE);
    Wire.write(WIRE_IO_CONFIGURATION); 
    Wire.endTransmission();
    
    // Configure Pin

    DDRE |= _BV(2);           //pinMode(PE2, OUTPUT);
    DDRD |= _BV(4);           //pinMode(4, OUTPUT);
    DDRB |= _BV(5) | _BV(6);  //pinMode(9,10, OUTPUT);
    
    
    // Configure software PWM4D Timer to fast PWM
    TCCR4A |= (1<<COM4B1); //enable communicate pin 10 and OCR4
    TCCR4D &= ~(1<<WGM41) & ~(1<<WGM40); 
    TC4H =0;
    
}

void Strela_t:: motorConnection(
            bool direction_1,
            bool direction_2)
{
    _motorConnection_1=direction_1;
    _motorConnection_2=direction_2;    
}

void Strela_t::_setMotorDirections(
            bool direction_1,
            bool direction_2)
{
    bool direction = direction_1;
    if (direction ^ _motorConnection_1)
        PORTD |= _BV(4);  //digitalWrite(4, HIGH)
    else
        PORTD &= ~_BV(4); //digitalWrite(4, LOW)
        
    direction = direction_2;
    if (direction ^ _motorConnection_2)
        PORTE |= _BV(2);
    else
        PORTE &= ~_BV(2);
        
}

void Strela_t::motors(
            int motorSpeed_1,
            int motorSpeed_2)
{ 
    _setMotorDirections((motorSpeed_1 < 0), (motorSpeed_2 < 0));

    _currentMotorSpeed_1 = abs(motorSpeed_1);
    _currentMotorSpeed_2 = abs(motorSpeed_2);
    
    //Motor 1 on PWM works on interrupts
    
    if (_currentMotorSpeed_1 >= 255)
    {
        TIMSK4 &= ~(1<<TOIE4) & ~(1<<OCIE4D); //disable interrupts on TIMER4 OVF and OCR4D
        PORTB |= _BV(5); //pin 9 HIGH
    } 
    else if (_currentMotorSpeed_1 == 0)
    {
        TIMSK4 &= ~(1<<TOIE4) & ~(1<<OCIE4D);  //disable interrupts on TIMER4 OVF and OCR4D
        PORTB &= ~_BV(5); //pin 9 LOW
    }
    else 
    {
        TCCR4C &= ~(1<<COM4D1);  //disable PWM on Pin 6
 
        //set PWM period
        OCR4D = _currentMotorSpeed_1;
        //enable interrupts on TIMER4 OVF and OCR4D
        TIMSK4 |= (1<<TOIE4)|(1<<OCIE4D);
    }
    
    //Motor2 PWM is hardware 
    
    if (_currentMotorSpeed_2 >= 255)
    {
        //disable communicate pin 10 with OCR4 and PWM
        TCCR4A &= ~(1<<PWM4B) & ~(1<<COM4B1);
        PORTB |= _BV(6); //pin 10 HIGH
    } 
    else if (_currentMotorSpeed_2 == 0)
    {
        //disable communicate pin 10 with OCR4 and PWM
        TCCR4A &= ~(1<<PWM4B) & ~(1<<COM4B1); 
        PORTB &= ~_BV(6); //pin 10 LOW
    }
    else 
    {   
        //set PWM period
        OCR4B = _currentMotorSpeed_2;
        TCCR4A |= (1<<PWM4B)|(1<<COM4B1); //enable communicate pin 10 with OCR4 and enable PWM
    }
}

void Strela_t::motorSpeed(
            uint8_t mot,
            int speed)
{
/*    if (mot = M1)
    {
        Strela_t.motors(
    }
    */
}

bool Strela_t::buttonRead(uint8_t btn)
{
    Wire.beginTransmission(GPUX_TWI_ADDR);
    Wire.write(WIRE_INPUT_READ_MODE); // input read mode
    Wire.endTransmission();
 
    int n = Wire.requestFrom(GPUX_TWI_ADDR, 1);
    // TODO: is reading without wait is right?
    int state = Wire.read();
 
    return !(state & (1 << (3 - btn)));
}

void Strela_t::ledWrite(
            uint8_t ld,
            bool state)
{
    Wire.beginTransmission(GPUX_TWI_ADDR);
    Wire.write(WIRE_OUTPUT_WRITE_MODE); // output write mode
    Wire.write(~(state << (7 - ld)));
    Wire.endTransmission();
}


