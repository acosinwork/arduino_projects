/* Strela.cpp
 * Strela library 
 *
 *
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include <Arduino.h>
#include <Wire.h> 

#include <StrelaSimple.h>


ISR(TIMER4_OVF_vect)          // interrupt service routine for software PWM
{
   PORTB |= _BV(5); //pin 9 HIGH

}

ISR(TIMER4_COMPD_vect)          // interrupt service routine for software PWM
{
    PORTB &= ~_BV(5); //pin 9 LOW

}

namespace {
    
    uint8_t lcdPinMode = 0;
    uint8_t ledState = 0;
    uint8_t lcdPinState =0;
    
}



void uPinMode(uint8_t pin, uint8_t mode)
{

    if (pin < NUM_DIGITAL_PINS) // standart IO
        {
            pinMode( pin, mode );
            return;
        }
        
      
    if ( pin > Q7)  //we can,t change pinMode on GPUX on addr 0x20 or more
    {
        return;
    }
    else 
    {
        if (mode != OUTPUT)
            lcdPinMode |= 1 << ( _LC(pin) );
        else
            lcdPinMode &= ~(1 << ( _LC(pin) ));       
         
            Wire.begin();
            Wire.beginTransmission(LCD_TWI_ADDR);
            Wire.write(WIRE_IO_CONFIG_MODE); // i/o config mode
            Wire.write(lcdPinMode); // 
            Wire.endTransmission();            
    }                
}

void twiWriteOut(uint8_t address, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(WIRE_OUTPUT_WRITE_MODE); // output write mode
    Wire.write(~value);
    Wire.endTransmission();
}

uint8_t twiReadIn(uint8_t address, uint8_t pin)
{
    
    Wire.beginTransmission(address);
    Wire.write(WIRE_INPUT_READ_MODE); // input read mode
    Wire.endTransmission();
 
    uint8_t n = Wire.requestFrom(address, (uint8_t)1);
    // TODO: is reading without wait is right?
    uint8_t state = Wire.read();
    
    return !(state & (1 << pin));
}



void uDigitalWrite(uint8_t pin, uint8_t val)
{
    if (pin < NUM_DIGITAL_PINS) // standart IO
    {
        digitalWrite( pin, val );
        return;
    }
    
    if ( pin > LAST_PIN)  //NOT ON PIN
    {
        return;
    }
    
    if ( pin > Q7)  //led or button
    {
        if ( pin > LS3 )
        {
            uint8_t curPin = pin - LS0;
            (val) ? ledState |= 1 << curPin : ledState &= ~(1 << curPin);
            twiWriteOut(GPUX_TWI_ADDR, ledState);
        }
        return;
    }   
    
    else // pin>=Q0 && pin<=Q7
    {
        uint8_t curPin = pin - Q0;
        (val) ? lcdPinState |= 1 << curPin : lcdPinState &= ~(1 << curPin);
        twiWriteOut(LCD_TWI_ADDR, lcdPinState);
    }        
}

uint8_t uDigitalRead(uint8_t pin)
{
    if (pin < NUM_DIGITAL_PINS) // standart IO
        return digitalRead( pin );
    
    if ( pin > LAST_PIN)  //NOT ON PIN
    {
        return LOW;
    }
    
    if ( pin > Q7)  //led or button
    {
        uint8_t curPin = pin - LS0;
        return twiReadIn(GPUX_TWI_ADDR, curPin);
    }   
    
    else // pin>=Q0 && pin<=Q7
    {
        uint8_t curPin = pin - Q0;
        return twiReadIn(LCD_TWI_ADDR, curPin);
    }        
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

void Strela_t::drive(
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


