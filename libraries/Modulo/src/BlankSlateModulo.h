#ifndef MOD_IO_H
#define MOD_IO_H

#include <inttypes.h>
#include "BaseModulo.h"

/// A general purpose I/O module for digital input, digital output, analog input, and PWM output.
class BlankSlateModulo : public BaseModulo {
public:
    BlankSlateModulo();
    BlankSlateModulo(uint16_t deviceID);

    /// Disables the output on the specified pin and returns the pin's value
    bool getDigitalInput(uint8_t pin);

    /// Reads the digital inputs from all 8 pins. Does not enable/disable outputs on any pins.
    uint8_t getDigitalInputs();

    /// Disables the output on the specified pin and performs an analog read.
    float getAnalogInput(uint8_t pin);

    /// Sets the pin direction to either output or input
    void setDirection(uint8_t pin, bool output);

    /// Sets the pin directions for all 8 pins simultaneously
    void setDirections(uint8_t outputs);

    /// Enables the output and sets the output value on the specified pin.
    void setDigitalOutput(uint8_t pin, bool value);

    /// Set the digital outputs on all 8 pins. Does not enable or disable outputs on any pins.
    bool setDigitalOutputs(uint8_t values);

    /// Enable the output and set the PWM duty cycle on the specified pin.
    /// Pins 0-4 have hardware PWM support. Pins 5-7 only have software PWM
    /// which has more jitter, especially at high frequencies.
    void setPWMValue(uint8_t pin, float value);

    /// Sets whether a pullup is enabled on the specified pin.
    void setPullup(uint8_t pin, bool enable=true);

    /// Set whether the pullup is enabled on all 8 pins.
    void setPullups(uint8_t pullups);

    /// Set the frequency for PWM signals on the specified pin.
    void setPWMFrequency(uint8_t pin, uint16_t value);

private:

};

#endif
