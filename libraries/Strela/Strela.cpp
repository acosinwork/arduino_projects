/* Strela.cpp
 * Strela library 
 *
 *
 */
 
#include <Strela.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//#include <inttypes.h> 

#include <Arduino.h>
#include <Wire.h>

Strela_t Strela;

ISR(TIMER3_OVF_vect)          // interrupt service routine for software PWM
{
    Strela.onTimerInterrupt();
}

void Strela_t::onTimerInterrupt()
{
    if (_currentMotorSpeed_1 < _pwmCounter)
        PORTB &= ~_BV(6); //pin 10 LOW
    else
        PORTB |= _BV(6); //pin 10 HIGH

    if (_currentMotorSpeed_2 < _pwmCounter)
        PORTB &= ~_BV(5); //pin 9 LOW
    else
        PORTB |= _BV(5); //pin 9 HIGH

    ++_pwmCounter;
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
/*    
    pinMode(MOTOR_ENABLE_12_PIN, OUTPUT);
    pinMode(MOTOR_ENABLE_34_PIN, OUTPUT);
    pinMode(MOTOR_DIRECTION_34_PIN, OUTPUT);
*/
    DDRE |= _BV(2);           //pinMode(PE2, OUTPUT);
    DDRD |= _BV(4);           //pinMode(4, OUTPUT);
    DDRB |= _BV(5) | _BV(6);  //pinMode(9,10, OUTPUT);
    
    // Configure software PWM Timer
    TCCR3A = 0;                 // clear control register A 
    TCCR3B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer
    
    unsigned char clockSelectBits;
    long cycles = 255;//(F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
    clockSelectBits = _BV(CS10);

    /*
    if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
    else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
    else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
    else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
    else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
    else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
*/
    ICR3 = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode

    TCCR3B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    TCCR3B |= clockSelectBits;                                                     // reset clock select register

    TIMSK3 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
    sei();                                                   // ensures that interrupts are globally enabled
//    TCCR3B |= clockSelectBits;  // 
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
        PORTE |= _BV(2);
    else
        PORTE &= ~_BV(2);
        
    direction = direction_2;
    if (direction ^ _motorConnection_2)
        PORTD |= _BV(4);  //digitalWrite(4, HIGH)
    else
        PORTD &= ~_BV(4); //digitalWrite(4, LOW)
        
}

void Strela_t::motors(
            int motorSpeed_1,
            int motorSpeed_2)
{ 
    _setMotorDirections((motorSpeed_1 < 0), (motorSpeed_2 < 0));

    _currentMotorSpeed_1 = constrain( abs (motorSpeed_1), 0, 255);
    _currentMotorSpeed_2 = constrain( abs (motorSpeed_2), 0, 255);
}

void Strela_t::motorSpeed(
            MOTOR mot,
            int speed)
{
    //TODO: todo
}

bool Strela_t::buttonRead(BUTTON btn)
{
    Wire.beginTransmission(GPUX_TWI_ADDR);
    Wire.write(WIRE_INPUT_READ_MODE); // input read mode
    Wire.endTransmission();
 
    int n = Wire.requestFrom(GPUX_TWI_ADDR, 1);
    // TODO: is reading without wait is right?
    int state = Wire.read();
 
    return state & (1 << (3 - btn));
}

void Strela_t::ledWrite(
            LED ld,
            bool state)
{
    Wire.beginTransmission(GPUX_TWI_ADDR);
    Wire.write(WIRE_OUTPUT_WRITE_MODE); // output write mode
    Wire.write(~(state << (7 - ld)));
    Wire.endTransmission();
}


