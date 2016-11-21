
#include <Arduino.h>

#include "QuadDisplay2.h"

#include "pins_arduino.h"


#ifndef _VARIANT_ARDUINO_DUE_X_

#define MACRO_DIGITAL_WRITE(a,b) (fastDigitalWrite(a,b))

    //Fast version of digitalWrite, without using turnOffPWM
    //
void fastDigitalWrite(uint8_t pin, uint8_t val)
{
      //uint8_t timer = digitalPinToTimer(pin);
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint8_t *out;

    if (port == NOT_A_PIN) return;

    // If the pin that support PWM output, we need to turn it off
    // before doing a digital write.
    // DELETED. Timer is already off here . Amperka
    // if (timer != NOT_ON_TIMER) turnOffPWM(timer);

    out = portOutputRegister(port);

    uint8_t oldSREG = SREG;
    cli();

    if (val == LOW) {
        *out &= ~bit;
    } else {
        *out |= bit;
    }

    SREG = oldSREG;
}


#else

#define MACRO_DIGITAL_WRITE(a,b) (digitalWrite(a,b))

#endif

const static uint8_t numerals[] = {QD_0, QD_1, QD_2, QD_3, QD_4, QD_5, QD_6, QD_7, QD_8, QD_9};

QuadDisplay::QuadDisplay()
{
 pin_sck = 13;
 pin_di = 11;
 pin_cs = 10;
}

QuadDisplay::~QuadDisplay()
{
 //
}

void QuadDisplay::begin(uint8_t sck, uint8_t di, uint8_t cs)
{
    pin_sck = sck;
    pin_di = di;
    pin_cs = cs;
    MACRO_DIGITAL_WRITE(pin_sck, LOW);
    MACRO_DIGITAL_WRITE(pin_di, LOW);
    MACRO_DIGITAL_WRITE(pin_cs, HIGH);
    pinMode(pin_sck, OUTPUT);
    pinMode(pin_di, OUTPUT);
    pinMode(pin_cs, OUTPUT);
}

void QuadDisplay::end()
{
    MACRO_DIGITAL_WRITE(pin_di, LOW);
    MACRO_DIGITAL_WRITE(pin_sck, LOW);
    MACRO_DIGITAL_WRITE(pin_cs, LOW);
    pinMode(pin_sck, INPUT);
    pinMode(pin_di, INPUT);
    pinMode(pin_cs, INPUT);
}

void QuadDisplay::beginWrite()
{
    MACRO_DIGITAL_WRITE(pin_sck, LOW);
    MACRO_DIGITAL_WRITE(pin_di, LOW);
    MACRO_DIGITAL_WRITE(pin_cs, LOW);
}

void QuadDisplay::writeData(uint8_t data, uint8_t n)
{
    for (uint8_t i = n; i > 0; i--)
    {
	MACRO_DIGITAL_WRITE(pin_di, (data & 1));
	MACRO_DIGITAL_WRITE(pin_sck, HIGH);
	MACRO_DIGITAL_WRITE(pin_sck, LOW);
        data >>= 1;
    }
}

void QuadDisplay::writeData(uint32_t data, uint8_t n)
{
    for (uint8_t i = n; i > 0; i--)
    {
	MACRO_DIGITAL_WRITE(pin_di, (data & 1));
	MACRO_DIGITAL_WRITE(pin_sck, HIGH);
	MACRO_DIGITAL_WRITE(pin_sck, LOW);
        data >>= 1;
    }
}

void QuadDisplay::endWrite()
{
    // FIXME: here we're actually pushing the dot of least significant digit
    // normally this should happen in writeData(), but it doesn't. why? we're pushing through all bits.
    MACRO_DIGITAL_WRITE(pin_di, HIGH);
    MACRO_DIGITAL_WRITE(pin_sck, HIGH);
    MACRO_DIGITAL_WRITE(pin_sck, LOW);
    //
    MACRO_DIGITAL_WRITE(pin_cs, HIGH);
    MACRO_DIGITAL_WRITE(pin_di, LOW);
    MACRO_DIGITAL_WRITE(pin_sck, LOW);
}

void QuadDisplay::displayDigits(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
{
    beginWrite();
    writeData(digit1);
    writeData(digit2);
    writeData(digit3);
    writeData(digit4);
    /* // rev G1
    writeData(digit2);
    writeData(digit1);
    writeData(digit4);
    writeData(digit3);
    */
    endWrite();
}

void QuadDisplay::displaySegments(uint32_t digits)
{
    beginWrite();
    writeData(digits);
    endWrite();
}

void QuadDisplay::displayClear()
{
    displayDigits(QD_NONE, QD_NONE, QD_NONE, QD_NONE);
}


void QuadDisplay::displayInt(int val, bool padZeros, uint8_t dots)
{
    uint8_t digits[4] = {0xff, 0xff, 0xff, 0xff};

    if (!padZeros && !val)
        digits[3] = numerals[0];
    else {
        bool negative = val < 0;
        val = abs(val);


        int8_t i;
        for (i = 4; i--; ) {
            uint8_t digit = val % 10;
            digits[i] = (val || padZeros) ? numerals[digit] : 0xff;

            val /= 10;
            if (!val && !padZeros)
                break;
        }

        if (negative)
            digits[max(0, i-1)] = QD_MINUS;

        for (i = 4; i--; ) {
            if (dots & (1 << i))
                digits[4 - i] &= QD_DOT;
        }
    }

    displayDigits(digits[0], digits[1], digits[2], digits[3]);
}

void QuadDisplay::displayFloat(float val, uint8_t precision, bool padZeros)
{
    uint8_t dot = 0x1;
    while (precision) {
        val *= 10;
        --precision;
        dot <<= 1;
    }

    displayInt((int)val, padZeros, dot);
}

void QuadDisplay::displayTemperatureC(int val, bool padZeros)
{

    uint8_t digits[4] = {0xff, 0xff, QD_DEGREE, QD_C};
    
    if (!padZeros && !val)
        digits[1] = numerals[0];
    else {
        bool negative = val < 0;
        val = abs(val);

        int8_t i;
        for (i = 2; i--; ) {
            uint8_t digit = val % 10;
            digits[i] = (val || padZeros) ? numerals[digit] : 0xff;

            val /= 10;
            if (!val && !padZeros)
                break;
        }

        if (negative)
            digits[max(0, i-1)] = QD_MINUS;
    }
    displayDigits(digits[0], digits[1], digits[2], digits[3]);
}

