//servo

#include <SoftwareServo.h> 

SoftwareServo myservo;

byte servoPin=A0; // Вот сюда подключаем серву. Если не сюда - возьмите другой пин

//My

int ledDelay = 20;

unsigned long lastPressTime;
unsigned long betweenPressDelay=5000;
byte lastButton;



void buttonPress(byte currentButton)
{
  if (currentButton != lastButton)
  {
    lastButton = currentButton;
    betweenPressDelay = millis() - lastPressTime;
  }
  lastPressTime = millis();
}

// TOUCH
#include <mpr121.h>
#include <i2c.h>
#define MPR121_R 0xB5	// ADD pin is grounded
#define MPR121_W 0xB4	// So address is 0x5A
#define MPR121addr    0x5a
int StateOfTouch1 = 0;
int StateOfTouch2 = 0;
int StateOfTouch3 = 0;
int StateOfTouch4 = 0;
int StateOfTouch5 = 0;
int StateOfTouch6 = 0;
int StateOfTouch7 = 0;
int StateOfTouch8 = 0;
int StateOfTouch9 = 0;
int StateOfTouch10 = 0;
int StateOfTouch11 = 0;
int StateOfTouch12 = 0;
int irqpin = 2;  // D2
uint16_t touchstatus;
volatile int GetTouchSignal = 0;
unsigned char reg = 0;
//int TouchedSensor = 0;
unsigned char status1 = 0;
unsigned char status2 = 0;
long previousMillis = 0;
long interval = 50;

//LEDS

unsigned char Prescaler = 0;
#define PrescalerOverflowValue 4
ISR(TIMER2_OVF_vect)
{
  if (Prescaler < PrescalerOverflowValue)
    Prescaler++;
  else {
    Prescaler = 0;
    Multiplex();
  }
}


unsigned char CurrentLED = 1;
unsigned char LEDValues[8][3];
bool LEDMoveDir[8][3];
void Multiplex(void)
{
  PORTD &= 0b00000011; // Control pin 0-5
  PORTB &= 0b11101110; // Control pin 6-7

  analogWrite(9, LEDValues[CurrentLED][0]);
  analogWrite(10, LEDValues[CurrentLED][1]);
  analogWrite(11, LEDValues[CurrentLED][2]);

  switch (CurrentLED)
  {
    case 0:
      //digitalWrite(2, 1); // Turn on LED 1
      PORTD |= 0b00000100;
      break;
    case 1:
      //digitalWrite(3, 1); // Turn on LED 1
      PORTD |= 0b00001000;
      break;
    case 2:
      //digitalWrite(4, 1); // Turn on LED 1
      PORTD |= 0b00010000;
      break;
    case 3:
      //digitalWrite(5, 1); // Turn on LED 1
      PORTD |= 0b00100000;
      break;
    case 4:
      //digitalWrite(6, 1); // Turn on LED 1
      PORTD |= 0b01000000;
      break;
    case 5:
      //digitalWrite(7, 1); // Turn on LED 1
      PORTD |= 0b10000000;
      break;
    case 6:
      //digitalWrite(8, 1); // Turn on LED 1
      PORTB |= 0b00000001;
      break;
    case 7:
      //digitalWrite(12, 1); // Turn on LED 1
      PORTB |= 0b00010000;
      break;
  }

  CurrentLED++;
  if (CurrentLED > 7)
    CurrentLED = 0;
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

int Red, Green, Blue;
int Hue1, Hue2, Hue3, Hue4, Hue5, Hue6, Hue7, Hue8, Saturation, Value;


void setup(void) {
  //SERVO
  myservo.attach(servoPin);
  
  //Set the pin we want the ISR to toggle for output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(2, 0); // Turn off LED 4
  digitalWrite(3, 0); // Turn off LED 4
  digitalWrite(4, 0); // Turn off LED 4
  digitalWrite(5, 0); // Turn off LED 4
  digitalWrite(6, 0); // Turn off LED 4
  digitalWrite(7, 0); // Turn off LED 4
  digitalWrite(8, 0); // Turn off LED 4
  digitalWrite(12, 0); // Turn off LED 4
  digitalWrite(9, 1);
  digitalWrite(10, 1);
  digitalWrite(11, 1);

  setPwmFrequency(9, 8);
  setPwmFrequency(10, 8);
  setPwmFrequency(11, 8);
  analogWrite(9, 255);
  analogWrite(10, 255);
  analogWrite(11, 255);

  LEDValues[0][0] = 100;
  LEDValues[0][1] = 0;
  LEDValues[0][2] = 20;

  LEDValues[1][0] = 0;
  LEDValues[1][1] = 30;
  LEDValues[1][2] = 144;

  LEDValues[2][0] = 66;
  LEDValues[2][1] = 245;
  LEDValues[2][2] = 3;

  LEDValues[3][0] = 30;
  LEDValues[3][1] = 30;
  LEDValues[3][2] = 200;

  //Start up the serial port
  Serial.begin(19200);

  //Signal the program start
  Serial.println("RGB LED Controller");

  // Enable Timer 2 interrupt (also used for PWM though)
  // This interrupt is divided by a prescaler, and takes care of the multiplexing
  TIMSK2 = 1 << TOIE2;

  Hue1 = 0;
  Hue2 = 10;
  Hue3 = 20;
  Hue4 = 30;
  Hue5 = 40;
  Hue6 = 50;
  Hue7 = 60;
  Hue8 = 70;
  Saturation = 255;
  Value = 255;

  //TOUCH
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  i2cInit();
  Serial.println("setting device up..");
  delay(100);
  mpr121QuickConfig();
  ///TOUCH
}

unsigned char incomingByte = 0;
unsigned char getNumPos = 0;
unsigned char tempNum = 0;
unsigned char LEDSetNum = 0;
unsigned char LEDSetColor = 0;

void loop(void)
{
  delay(ledDelay);
  
  HSVtoRGB(&Red, &Green, &Blue, Hue1, Saturation, Value);

  LEDValues[0][0] = constrain(Red, 0, 255);
  LEDValues[0][1] = constrain(Green, 0, 255);
  LEDValues[0][2] = constrain(Blue, 0, 255);

  Hue1++;
  if (Hue1 > 359)
    Hue1 = 0;

  HSVtoRGB(&Red, &Green, &Blue, Hue2, Saturation, Value);

  LEDValues[1][0] = constrain(Red, 0, 255);
  LEDValues[1][1] = constrain(Green, 0, 255);
  LEDValues[1][2] = constrain(Blue, 0, 255);

  Hue2++;
  if (Hue2 > 359)
    Hue2 = 0;

  HSVtoRGB(&Red, &Green, &Blue, Hue3, Saturation, Value);

  LEDValues[2][0] = constrain(Red, 0, 255);
  LEDValues[2][1] = constrain(Green, 0, 255);
  LEDValues[2][2] = constrain(Blue, 0, 255);

  Hue3++;
  if (Hue3 > 359)
    Hue3 = 0;

  HSVtoRGB(&Red, &Green, &Blue, Hue4, Saturation, Value);

  LEDValues[3][0] = constrain(Red, 0, 255);
  LEDValues[3][1] = constrain(Green, 0, 255);
  LEDValues[3][2] = constrain(Blue, 0, 255);

  Hue4++;
  if (Hue4 > 359)
    Hue4 = 0;

  HSVtoRGB(&Red, &Green, &Blue, Hue5, Saturation, Value);

  LEDValues[4][0] = constrain(Red, 0, 255);
  LEDValues[4][1] = constrain(Green, 0, 255);
  LEDValues[4][2] = constrain(Blue, 0, 255);

  Hue5++;
  if (Hue5 > 359)
    Hue5 = 0;

  HSVtoRGB(&Red, &Green, &Blue, Hue6, Saturation, Value);

  LEDValues[5][0] = constrain(Red, 0, 255);
  LEDValues[5][1] = constrain(Green, 0, 255);
  LEDValues[5][2] = constrain(Blue, 0, 255);

  Hue6++;
  if (Hue6 > 359)
    Hue6 = 0;

  HSVtoRGB(&Red, &Green, &Blue, Hue7, Saturation, Value);

  LEDValues[6][0] = constrain(Red, 0, 255);
  LEDValues[6][1] = constrain(Green, 0, 255);
  LEDValues[6][2] = constrain(Blue, 0, 255);

  Hue7++;
  if (Hue7 > 359)
    Hue7 = 0;

  HSVtoRGB(&Red, &Green, &Blue, Hue8, Saturation, Value);

  LEDValues[7][0] = constrain(Red, 0, 255);
  LEDValues[7][1] = constrain(Green, 0, 255);
  LEDValues[7][2] = constrain(Blue, 0, 255);
  
  //My
  Serial.println(constrain(Red, 0, 255));
  Serial.println(constrain(Green, 0, 255));
  Serial.println(constrain(Blue, 0, 255));
///My

  Hue8++;
  if (Hue8 > 359)
    Hue8 = 0;


  //TOUCH

  unsigned char MPR_Query = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    Read_MPR121();
  }

  //   delayMicroseconds(100);
  ////TOUCH
  
  //My
  if (betweenPressDelay<4000)
  {
    ledDelay=betweenPressDelay/100;
  }
  
  //servo
  SoftwareServo::refresh();

}

void HSVtoRGB( int  *r, int *g, int *b, int h, int s, int v )
{
  int lightCorrection;
  int f;
  long p, q, t;

  //my

  if ((millis()-lastPressTime)<5000)
  {
  lightCorrection = map((millis()-lastPressTime), 0, 5000, 0, 255);
  }
  else lightCorrection=255;
  
  myservo.write(map(lightCorrection, 0, 255, 0, 179));
  
  ///my


  if ( s == 0 )
  {
    *r = *g = *b = v-lightCorrection;
    return;
  }
  f = ((h % 60) * 255) / 60;
  h /= 60;
  p = (v * (256 - s)) / 256;
  q = (v * ( 256 - (s * f) / 256 )) / 256;
  t = (v * ( 256 - (s * ( 256 - f )) / 256)) / 256;
  
  
    switch ( h ) {
    case 0:
      *r = v-lightCorrection;
      *g = t-lightCorrection;
      *b = p-lightCorrection;
      break;
    case 1:
      *r = q-lightCorrection;
      *g = v-lightCorrection;
      *b = p-lightCorrection;
      break;
    case 2:
      *r = p-lightCorrection;
      *g = v-lightCorrection;
      *b = t-lightCorrection;
      break;
    case 3:
      *r = p-lightCorrection;
      *g = q-lightCorrection;
      *b = v-lightCorrection;
      break;
    case 4:
      *r = t-lightCorrection;
      *g = p-lightCorrection;
      *b = v-lightCorrection;
      break;
    default:
      *r = v-lightCorrection;
      *g = p-lightCorrection;
      *b = q-lightCorrection;
      break;
  }

/*
  switch ( h ) {
    case 0:
      *r = v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
    default:
      *r = v;
      *g = p;
      *b = q;
      break;
  }
*/
}





//TOUCH

void Read_MPR121()
{
  status1 = mpr121Read(0x00);
  status2 = mpr121Read(0x01);
  CheckStatus();
}
void  CheckStatus()
{
  if ((status1 & 0x01) == 0x01)
  {
    StateOfTouch1 = 1;
  }
  else
  {
    if (StateOfTouch1 == 1)
    {
      Serial.println("channel 0 has pressed..");
      StateOfTouch1 = 0;

      buttonPress(0);
    }
  }
  if ((status1 & 0x02) == 0x02)
  {
    StateOfTouch2 = 1;
  }
  else
  {
    if (StateOfTouch2 == 1)
    {
      Serial.println("channel 1 has pressed..");
      StateOfTouch2 = 0;

      buttonPress(1);
    }
  }
  if ((status1 & 0x04) == 0x04)
  {
    StateOfTouch3 = 1;
  }
  else
  {
    if (StateOfTouch3 == 1)
    {
      Serial.println("channel 2 has pressed..");
      StateOfTouch3 = 0;

      buttonPress(0);
    }
  }
  if ((status1 & 0x08) == 0x08)
  {
    StateOfTouch4 = 1;
  }
  else
  {
    if (StateOfTouch4 == 1)
    {
      Serial.println("channel 3 has pressed..");
      StateOfTouch4 = 0;

      buttonPress(3);
    }
  }
  if ((status1 & 0x10) == 0x10)
  {
    StateOfTouch5 = 1;
    //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch5 == 1)
    {
      Serial.println("channel 4 has pressed..");
      StateOfTouch5 = 0;

      buttonPress(4);
    }
  }
  if ((status1 & 0x20) == 0x20)
  {
    StateOfTouch6 = 1;
    //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch6 == 1)
    {
      Serial.println("channel 5 has pressed..");
      StateOfTouch6 = 0;

      buttonPress(5);
    }
  }
  if ((status1 & 0x40) == 0x40)
  {
    StateOfTouch7 = 1;
    //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch7 == 1)
    {
      Serial.println("channel 6 has pressed..");
      StateOfTouch7 = 0;

      buttonPress(6);
    }
  }
  if ((status1 & 0x80) == 0x80)
  {
    StateOfTouch8 = 1;
    //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch8 == 1)
    {
      Serial.println("channel 7 has pressed..");
      StateOfTouch8 = 0;

      buttonPress(7);
    }
  }
  if ((status2 & 0x01) == 0x01)
  {
    StateOfTouch9 = 1;
  }
  else
  {
    if (StateOfTouch9 == 1)
    {
      Serial.println("channel 8 has pressed..");
      StateOfTouch9 = 0;

      buttonPress(8);
    }
  }
  if ((status2 & 0x02) == 0x02)
  {
    StateOfTouch10 = 1;
  }
  else
  {
    if (StateOfTouch10 == 1)
    {
      Serial.println("channel 9 has pressed..");
      StateOfTouch10 = 0;

      buttonPress(9);
    }
  }
  if ((status2 & 0x04) == 0x04)
  {
    StateOfTouch11 = 1;
  }
  else
  {
    if (StateOfTouch11 == 1)
    {
      Serial.println("channel 10 has pressed..");
      StateOfTouch11 = 0;

      buttonPress(10);
    }
  }
  if ((status2 & 0x08) == 0x08)
  {
    StateOfTouch12 = 1;
  }
  else
  {
    if (StateOfTouch12 == 1)
    {
      Serial.println("channel 11 has pressed..");
      StateOfTouch12 = 0;

      buttonPress(11);
    }
  }
}
byte mpr121Read(uint8_t address)
{
  byte data;

  i2cSendStart();
  i2cWaitForComplete();

  i2cSendByte(MPR121_W);	// write 0xB4
  i2cWaitForComplete();

  i2cSendByte(address);	// write register address
  i2cWaitForComplete();

  i2cSendStart();

  i2cSendByte(MPR121_R);	// write 0xB5
  i2cWaitForComplete();
  i2cReceiveByte(TRUE);
  i2cWaitForComplete();

  data = i2cGetReceivedByte();	// Get MSB result
  i2cWaitForComplete();
  i2cSendStop();

  cbi(TWCR, TWEN);	// Disable TWI
  sbi(TWCR, TWEN);	// Enable TWI

  return data;
}

void mpr121Write(unsigned char address, unsigned char data)
{
  i2cSendStart();
  i2cWaitForComplete();

  i2cSendByte(MPR121_W);// write 0xB4
  i2cWaitForComplete();

  i2cSendByte(address);	// write register address
  i2cWaitForComplete();

  i2cSendByte(data);
  i2cWaitForComplete();

  i2cSendStop();
}

void mpr121QuickConfig(void)
{
  Serial.println("config MPR121..");
  // Section A
  // This group controls filtering when data is > baseline.
  mpr121Write(MHD_R, 0x01);
  mpr121Write(NHD_R, 0x01);
  mpr121Write(NCL_R, 0x00);
  mpr121Write(FDL_R, 0x00);
  mpr121Write(MHD_F, 0x01);
  mpr121Write(NHD_F, 0x01);
  mpr121Write(NCL_F, 0xFF);
  mpr121Write(FDL_F, 0x02);
  mpr121Write(ELE0_T, TOU_THRESH);
  mpr121Write(ELE0_R, REL_THRESH);
  mpr121Write(ELE1_T, TOU_THRESH);
  mpr121Write(ELE1_R, REL_THRESH);
  mpr121Write(ELE2_T, TOU_THRESH);
  mpr121Write(ELE2_R, REL_THRESH);
  mpr121Write(ELE3_T, TOU_THRESH);
  mpr121Write(ELE3_R, REL_THRESH);
  mpr121Write(ELE4_T, TOU_THRESH);
  mpr121Write(ELE4_R, REL_THRESH);
  mpr121Write(ELE5_T, TOU_THRESH);
  mpr121Write(ELE5_R, REL_THRESH);
  mpr121Write(ELE6_T, TOU_THRESH);
  mpr121Write(ELE6_R, REL_THRESH);
  mpr121Write(ELE7_T, TOU_THRESH);
  mpr121Write(ELE7_R, REL_THRESH);
  mpr121Write(ELE8_T, TOU_THRESH);
  mpr121Write(ELE8_R, REL_THRESH);
  mpr121Write(ELE9_T, TOU_THRESH);
  mpr121Write(ELE9_R, REL_THRESH);
  mpr121Write(ELE10_T, TOU_THRESH);
  mpr121Write(ELE10_R, REL_THRESH);
  mpr121Write(ELE11_T, TOU_THRESH);
  mpr121Write(ELE11_R, REL_THRESH);

  mpr121Write(ATO_CFGU, 0xC9);	// USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   mpr121Write(ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
  mpr121Write(ATO_CFGL, 0x82);  // Target = 0.9*USL = 0xB5 @3.3V
  mpr121Write(ATO_CFGT, 0xb5);
  mpr121Write(ATO_CFG0, 0x1B);
  mpr121Write(ELE_CFG, 0x8c);
  Serial.println("config MPR121 Done!");
}

void  checkInterrupt(void)
{
  int IRQ = 0;
  IRQ = digitalRead(irqpin);
  if (IRQ == 0)
  {
    GetTouchSignal = 1;
  }
  else
  {
    GetTouchSignal = 0;
  }
}

