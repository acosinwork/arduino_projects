
#include <Arduino.h>
#include <inttypes.h> 

#define W4     4
#define W3     SCL
#define W3_UP  SDA
#define W2     SCL
#define W2_UP  SDA
#define W1     0
#define W1_UP  1

#define X4     8
#define X4_UP  9
#define X3     7
#define X2     6
#define X1     5

#define Y4     A1
#define Y3     A0
#define Y2     12
#define Y2_UP  13
#define Y1     10
#define Y1_UP  11

#define Z4     A5
#define Z3     A4
#define Z2     A3
#define Z1     A2


float voltageRead(uint8_t pin);
float analogToVoltage(int val);