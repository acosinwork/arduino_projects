#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 13
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);

void setup() {

  Serial.begin(9600);

//  Serial.println("Color View Test!");

  if (tcs.begin()) {
//    Serial.println("Found sensor");
  } else {
//    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  digitalWrite(redpin, HIGH);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }

}


void loop() {

  analogWrite(13, (analogRead(A0)>>2));

  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(200);  // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED
  byte rgb[3];

  rgb[0] = (red  & 0xff)?(red  & 0xff):1;
  rgb[1] = (green  & 0xff)?(green & 0xff):1;
  rgb[2] = (blue  & 0xff)? (blue  & 0xff):1;

  Serial.write(rgb[0]);
  Serial.write(rgb[1]);
  Serial.write(rgb[2]);
  Serial.print('\0');

  //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );
  /*
    analogWrite(redpin, gammatable[(int)r]);
    analogWrite(greenpin, gammatable[(int)g]);
    analogWrite(bluepin, gammatable[(int)b]);
  */
}

