#include "GpioExpander.h"
#include <Wire.h>
#include <Arduino.h>

void GpioExpander::writeCmdPin(IOcommand command, uint8_t pin, bool sendStop)
{
    delay(0);
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.write(pin);
    Wire.endTransmission(sendStop);
}

void GpioExpander::writeCmdPin16Val(IOcommand command, uint8_t pin, uint16_t value, bool sendStop)
{
    delay(0);
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.write(pin);
    uint8_t temp;
    temp = (value >> 8) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    temp = value & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    Wire.endTransmission(sendStop);
}


void GpioExpander::writeCmd16BitData(IOcommand command, uint16_t data)
{
    delay(0);
    Wire.beginTransmission( _i2caddress ); // Address set on class instantiation
    Wire.write((uint8_t)command);
    uint8_t temp;
    temp = (data >> 8) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    temp = data & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    Wire.endTransmission();
}

void GpioExpander::writeCmd8BitData(IOcommand command, uint8_t data)
{
    delay(0);
    Wire.beginTransmission( _i2caddress ); // Address set on class instantiation
    Wire.write((uint8_t)command);
    Wire.write(data); // Data/setting to be sent to device
    Wire.endTransmission();
}

void GpioExpander::writeCmd(IOcommand command, bool sendStop)
{
    delay(0);
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.endTransmission(sendStop);
}

int GpioExpander::read16Bit()
{
    delay(0);
    int result = -1;
    uint8_t byteCount = 2;
    Wire.requestFrom(_i2caddress, byteCount);
    uint16_t counter = 0xff;
    while (Wire.available() < byteCount)
    {
        delay(0);
        if (!(--counter))
            return result;
    }
    result = Wire.read();
    result <<= 8;
    result |= Wire.read();
    return result;
}

uint32_t GpioExpander::read32bit()
{
    delay(0);
    uint32_t result = 0xffffffff; // https://www.youtube.com/watch?v=y73hyMP1a-E
    uint8_t byteCount = 4;
    Wire.requestFrom(_i2caddress, byteCount);
    uint16_t counter = 0xff;
    while (Wire.available() < byteCount)
    {
        delay(0);
        if (!(--counter))
            return result;
    }
    for (uint8_t i = 0; i < 3; ++i) {
      result = Wire.read();
      result <<= 8;
    }
    result |= Wire.read();
    return result;
}

GpioExpander::GpioExpander(uint8_t i2caddress)
{
    _i2caddress = i2caddress;
}

void GpioExpander::digitalWritePort(uint16_t value)
{
    writeCmd16BitData(DIGITAL_WRITE_HIGH, value);
    writeCmd16BitData(DIGITAL_WRITE_LOW, ~value);
}

void GpioExpander::digitalWrite(int pin, bool value)
{
    uint16_t sendData = 1<<pin;
    if (value) {
        writeCmd16BitData(DIGITAL_WRITE_HIGH, sendData);
    } else {
        writeCmd16BitData(DIGITAL_WRITE_LOW, sendData);
    }
}

int GpioExpander::digitalReadPort()
{
    writeCmd(DIGITAL_READ, false);
    return read16Bit();
}

int GpioExpander::digitalRead(int pin)
{
    int result = digitalReadPort();
    if (result >= 0) {
        result = ((result & (1<<pin))? 1 : 0); //:)
    }
    return result;
}

void GpioExpander::pinMode(int pin, uint8_t mode)
{
    uint16_t sendData = 1<<pin;
    if (mode == INPUT) {
        writeCmd16BitData(PORT_MODE_INPUT, sendData);
    } else if (mode == OUTPUT) {
        writeCmd16BitData(PORT_MODE_OUTPUT, sendData);
    } else if (mode == INPUT_PULLUP) {
        writeCmd16BitData(PORT_MODE_PULLUP, sendData);
    } else if (mode == INPUT_PULLDOWN) {
        writeCmd16BitData(PORT_MODE_PULLDOWN, sendData);
    }

}

void GpioExpander::analogWrite_16(int pin, uint16_t pulseWidth)
{
    writeCmdPin16Val(ANALOG_WRITE, (uint8_t)pin, pulseWidth, true);
}

void GpioExpander::analogWrite(int pin, uint8_t pulseWidth)
{
    uint16_t val = map(pulseWidth, 0, 255, 0, 65535);
    writeCmdPin16Val(ANALOG_WRITE, (uint8_t)pin, val, true);
}

int GpioExpander::analogRead(int pin)
{
    writeCmdPin(ANALOG_READ, (uint8_t)pin, true);
    return read16Bit();
}

void GpioExpander::pwmFreq(uint16_t freq)
{
    writeCmd16BitData(PWM_FREQ, freq);
}

void GpioExpander::adcSpeed(uint8_t speed)
{
    // speed must be < 8. Smaller is faster, but dirty
    writeCmd8BitData(ADC_SPEED, speed);
}

void GpioExpander::changeAddr(uint8_t newAddr)
{
    writeCmd8BitData(CHANGE_I2C_ADDR, newAddr);
    _i2caddress = newAddr;
}

void GpioExpander::saveAddr()
{
    writeCmd(SAVE_I2C_ADDR);
}

void GpioExpander::reset()
{
    writeCmd(RESET);
}

uint32_t GpioExpander::getUID()
{
    writeCmd(WHO_AM_I);
    return read32bit();
}
